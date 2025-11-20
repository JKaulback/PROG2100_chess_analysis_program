#include "uci_communication.h"
#include <windows.h>
#include <iostream>

UCICommunication::UCICommunication()
    : inputHandle_(nullptr)
    , outputHandle_(nullptr)
{}

void UCICommunication::initialize(void* inputHandle, void* outputHandle) {
    inputHandle_ = inputHandle;
    outputHandle_ = outputHandle;
}

bool UCICommunication::sendCommand(const std::string& command) {
    HANDLE handle = static_cast<HANDLE>(inputHandle_);
    if (!handle)
        return false;
    
    DWORD bytesWritten;
    std::string fullCommand = command + "\n";
    
    if (WriteFile(handle, fullCommand.c_str(), fullCommand.length(), 
            &bytesWritten, nullptr)) {
        std::cout << "Sent: " << command << std::endl;
        return true;
    }
    
    std::cerr << "Failed to send command: " << command << std::endl;
    return false;
}

std::string UCICommunication::readResponseLine() {
    std::string response;
    
    HANDLE handle = static_cast<HANDLE>(outputHandle_);
    if (!handle)
        return "";
    
    while (true) {
        char ch;
        DWORD bytesRead;
        
        if (ReadFile(handle, &ch, 1, &bytesRead, nullptr) && bytesRead == 1) {
            response += ch;
            
            if (ch == '\n')
                break;
        } else
            break;
    }
    
    return response;
}

bool UCICommunication::hasDataAvailable() const {
    HANDLE handle = static_cast<HANDLE>(outputHandle_);
    if (!handle)
        return false;
    
    DWORD bytesAvailable = 0;
    // PeekNamedPipe checks if data is available without reading it
    if (PeekNamedPipe(handle, nullptr, 0, nullptr, &bytesAvailable, nullptr))
        return bytesAvailable > 0;
    
    return false;
}

bool UCICommunication::initializeProtocol() {
    // Send UCI command
    if (!sendCommand("uci")) {
        std::cerr << "Failed to send UCI command" << std::endl;
        return false;
    }
    
    // Wait for uciok
    if (!waitForResponse("uciok")) {
        std::cerr << "Did not receive uciok response" << std::endl;
        return false;
    }
    
    // Send isready
    if (!sendCommand("isready")) {
        std::cerr << "Failed to send isready command" << std::endl;
        return false;
    }
    
    // Wait for readyok
    if (!waitForResponse("readyok")) {
        std::cerr << "Did not receive readyok response" << std::endl;
        return false;
    }
    
    return true;
}

bool UCICommunication::configureEngine() {
    // Set MultiPV to 4 for multiple principal variations
    if (!sendCommand("setoption name MultiPV value 4")) {
        std::cerr << "Failed to set MultiPV option" << std::endl;
        return false;
    }
    
    return true;
}

bool UCICommunication::waitForResponse(const std::string& expectedResponse) {
    while (true) {
        std::string line = readResponseLine();
        if (line.empty())
            return false; // No more data available
        
        if (line.find(expectedResponse) != std::string::npos)
            return true;
    }
}