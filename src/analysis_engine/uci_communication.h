#pragma once

#include <string>
#include <memory>

/**
 * Handles low-level communication with UCI engines
 * Responsible for sending commands and reading responses
 */
class UCICommunication {
public:
    UCICommunication();
    ~UCICommunication() = default;
    
    // Non-copyable
    UCICommunication(const UCICommunication&) = delete;
    UCICommunication& operator=(const UCICommunication&) = delete;
    
    /**
     * Initialize communication with the given handles
     * @param inputHandle Handle to write commands to engine
     * @param outputHandle Handle to read responses from engine
     */
    void initialize(void* inputHandle, void* outputHandle);
    
    /**
     * Send a command to the engine
     * @param command The UCI command to send
     * @return true if successful, false otherwise
     */
    bool sendCommand(const std::string& command);
    
    /**
     * Read a single line of response from the engine
     * @return The response line (may be empty if no data available)
     */
    std::string readResponseLine();
    
    /**
     * Check if data is available to read without blocking
     * @return true if data is available, false otherwise
     */
    bool hasDataAvailable() const;
    
    /**
     * Initialize the UCI protocol with the engine
     * Sends uci command and waits for uciok, then sends isready and waits for readyok
     * @return true if successful, false otherwise
     */
    bool initializeProtocol();
    
    /**
     * Configure engine options (like MultiPV)
     * @return true if successful, false otherwise
     */
    bool configureEngine();
    
private:
    void* inputHandle_;
    void* outputHandle_;
    
    bool waitForResponse(const std::string& expectedResponse);
};