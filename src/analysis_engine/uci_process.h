#pragma once

#include <string>
#include <memory>

/**
 * Handles low-level process management for UCI engines
 * Responsible for creating processes, managing pipes, and cleanup
 */
class UCIProcess {
public:
    UCIProcess();
    ~UCIProcess();
    
    // Non-copyable
    UCIProcess(const UCIProcess&) = delete;
    UCIProcess& operator=(const UCIProcess&) = delete;
    
    /**
     * Start the UCI engine process
     * @param enginePath Path to the engine executable
     * @return true if successful, false otherwise
     */
    bool startEngine(const std::string& enginePath);
    
    /**
     * Stop the engine process and cleanup resources
     */
    void stopEngine();
    
    /**
     * Check if the engine process is running
     */
    bool isRunning() const;
    
    /**
     * Get handles for communication
     */
    void* getInputHandle() const { return inputHandle_; }
    void* getOutputHandle() const { return outputHandle_; }
    void* getProcessHandle() const { return processHandle_; }
    
private:
    void* processHandle_;
    void* inputHandle_;
    void* outputHandle_;
    
    void cleanup();
};