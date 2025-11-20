#include "uci_process.h"
#include <windows.h>
#include <iostream>

UCIProcess::UCIProcess() : 
    processHandle_(nullptr), inputHandle_(nullptr), outputHandle_(nullptr)
{}

UCIProcess::~UCIProcess() {
    stopEngine();
}

bool UCIProcess::startEngine(const std::string& enginePath) {
    if (isRunning())
        return true; // Already running
    
    STARTUPINFOW si = {};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = {};
    
    HANDLE hInputRead, hInputWrite, hOutputRead, hOutputWrite;
    SECURITY_ATTRIBUTES sa = { sizeof(sa), nullptr, TRUE };
    
    // Create pipes for communication
    if (!CreatePipe(&hInputRead, &hInputWrite, &sa, 0) ||
            !CreatePipe(&hOutputRead, &hOutputWrite, &sa, 0)) {
        std::cerr << "Failed to create pipes for UCI engine" << std::endl;
        return false;
    }
    
    // Set up startup info to use our pipes
    si.hStdInput = hInputRead;
    si.hStdOutput = hOutputWrite;
    si.hStdError = hOutputWrite;
    si.dwFlags = STARTF_USESTDHANDLES;
    
    // Convert engine path to wide string for Windows API
    std::wstring widePath(enginePath.begin(), enginePath.end());
    
    // Create the engine process
    if (!CreateProcessW(nullptr, const_cast<wchar_t*>(widePath.c_str()),
                       nullptr, nullptr, TRUE, 0, nullptr, nullptr,
                       &si, &pi)) {
        std::cerr << "Failed to start UCI engine process: " << enginePath << std::endl;
        CloseHandle(hInputRead);
        CloseHandle(hInputWrite);
        CloseHandle(hOutputRead);
        CloseHandle(hOutputWrite);
        return false;
    }
    
    // Store the handles we need and close the ones we don't
    processHandle_ = pi.hProcess;
    inputHandle_ = hInputWrite;   // We write to this
    outputHandle_ = hOutputRead;  // We read from this
    
    // Close handles we don't need
    CloseHandle(hInputRead);      // Engine reads from this
    CloseHandle(hOutputWrite);    // Engine writes to this
    CloseHandle(pi.hThread);      // Don't need thread handle
    
    return true;
}

void UCIProcess::stopEngine() {
    cleanup();
}

bool UCIProcess::isRunning() const {
    if (!processHandle_)
        return false;
    
    DWORD exitCode;
    if (GetExitCodeProcess(static_cast<HANDLE>(processHandle_), &exitCode))
        return exitCode == STILL_ACTIVE;
    
    return false;
}

void UCIProcess::cleanup() {
    // Close communication handles
    if (inputHandle_) {
        CloseHandle(static_cast<HANDLE>(inputHandle_));
        inputHandle_ = nullptr;
    }
    
    if (outputHandle_) {
        CloseHandle(static_cast<HANDLE>(outputHandle_));
        outputHandle_ = nullptr;
    }
    
    // Terminate and close process handle
    if (processHandle_) {
        HANDLE handle = static_cast<HANDLE>(processHandle_);
        TerminateProcess(handle, 0);
        WaitForSingleObject(handle, 1000); // Wait up to 1 second for clean exit
        CloseHandle(handle);
        processHandle_ = nullptr;
    }
}