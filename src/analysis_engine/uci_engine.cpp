#include "uci_engine.h"
#include <iostream>
#include <chrono>

UCIEngine::UCIEngine(const std::string& enginePath)
    : enginePath_(enginePath)
    , process_(std::make_unique<UCIProcess>())
    , communication_(std::make_unique<UCICommunication>())
    , state_(EngineState::Disconnected)
    , enabled_(false)
{}

UCIEngine::~UCIEngine() {
    if (enabled_)
        disable();
}

void UCIEngine::enable() {
    if (enabled_)
        return; // Already enabled
    
    state_ = EngineState::Connecting;
    
    if (!initializeEngine()) {
        state_ = EngineState::Error;
        return;
    }
    
    // Only set enabled to true AFTER initialization succeeds
    enabled_ = true;
    state_ = EngineState::Ready;
    
    // Start analysis thread
    analysisThread_ = std::make_unique<std::thread>(&UCIEngine::analysisThreadFunction, this);
}

void UCIEngine::disable() {
    if (!enabled_)
        return; // Already disabled
    
    enabled_ = false;
    state_ = EngineState::Disconnected;
    
    // Wait for thread to finish
    if (analysisThread_ && analysisThread_->joinable())
        analysisThread_->join();
    
    // Stop the engine process
    process_->stopEngine();
    
    // Reset components
    analysisThread_ = nullptr;
}

bool UCIEngine::isEnabled() const {
    return enabled_;
}

void UCIEngine::setPosition(const std::string& startFen, const std::vector<std::string>& moves) {
    // Check if engine is enabled
    if (!enabled_)
        return;
    
    // Validate FEN
    const std::string standardStartPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::string fenToUse = 
        startFen.empty() ?
        standardStartPos :
        startFen;

    if (!startFen.empty() && !UCIAnalysisParser::isValidFEN(startFen)) {
        std::cerr << "Invalid starting FEN string: " << startFen << std::endl;
        return;
    }
    
    // Set the requested starting FEN and moves - the analysis thread will detect the change
    {
        std::lock_guard<std::mutex> lock(analysisMutex_);
        requestedStartFen_ = startFen;
        if (!moves.empty()) {
            requestedMoves_ = moves;
        }
    }
}

EngineAnalysis UCIEngine::pollAnalysis() {
    EngineAnalysis result;
    
    // Get current state
    result.state = state_;
    
    // Get latest analysis data
    {
        std::lock_guard<std::mutex> lock(analysisMutex_);
        // Build FEN from start position and moves for display
        result.fen = currentStartFen_; // Could build actual current FEN if needed
        result.rawInfo = currentAnalysis_.rawInfo;
        result.hasResult = currentAnalysis_.hasResult;
        result.lines = currentAnalysis_.lines;
    }
    
    return result;
}

bool UCIEngine::initializeEngine() {
    // Start the engine process
    if (!process_->startEngine(enginePath_)) {
        std::cerr << "Failed to start engine process" << std::endl;
        return false;
    }
    
    // Initialize communication
    communication_->initialize(process_->getInputHandle(), process_->getOutputHandle());
    
    // Initialize UCI protocol
    if (!communication_->initializeProtocol()) {
        std::cerr << "Failed to initialize UCI protocol" << std::endl;
        return false;
    }
    
    // Configure engine options
    if (!communication_->configureEngine()) {
        std::cerr << "Failed to configure engine" << std::endl;
        return false;
    }
    
    return true;
}

void UCIEngine::analysisThreadFunction() {
    while (true) {
        // Exit if disabled
        if (!enabled_)
            break;
        
        EngineState currentState = state_;
        
        // Exit if disconnected or error
        if (currentState == EngineState::Disconnected || currentState == EngineState::Error)
            break;

        
        // Check for and handle any position change requests
        handlePositionTransition();
        
        // Read engine output while analyzing
        if (currentState == EngineState::Analyzing) {
            // Only attempt to read if data is available to avoid blocking
            if (communication_->hasDataAvailable())
                readEngineOutput();
        }
        
        // Sleep based on current state (stale state is acceptable, will refresh next iteration)
        std::this_thread::sleep_for(
            currentState == EngineState::Analyzing ? 
            std::chrono::milliseconds(10) : 
            std::chrono::milliseconds(100));
    }
}

void UCIEngine::handlePositionTransition() {
    // Check if there's a position change to handle
    std::string requestedStartFen;
    std::vector<std::string> requestedMoves;
    std::string currentStartFen;
    std::vector<std::string> currentMoves;
    {
        std::lock_guard<std::mutex> lock(analysisMutex_);
        requestedStartFen = requestedStartFen_;
        requestedMoves = requestedMoves_;
        currentStartFen = currentStartFen_;
        currentMoves = currentMoves_;
    }
    
    // If no change requested or already analyzing this position, nothing to do
    if (requestedStartFen.empty() || 
            (requestedStartFen == currentStartFen && requestedMoves == currentMoves))
        return;
    
    // Need to transition to new position - stop current analysis if running
    if (state_ == EngineState::Analyzing)
        stopCurrentAnalysis();
    
    // Update current position and clear results
    {
        std::lock_guard<std::mutex> lock(analysisMutex_);
        currentStartFen_ = requestedStartFen;
        currentMoves_ = requestedMoves;
        currentAnalysis_ = EngineAnalysis();
    }
    // Start analysis on new position
    startAnalysisForPosition(requestedStartFen, requestedMoves);
}

void UCIEngine::stopCurrentAnalysis() {
    state_ = EngineState::Stopping;
    communication_->sendCommand("stop");
    
    // Wait for stop to complete by reading output until we see "bestmove"
    std::string line;
    do
        line = communication_->readResponseLine();
    while (!line.empty() && line.find("bestmove") == std::string::npos);
}

void UCIEngine::startAnalysisForPosition(const std::string& startFen, const std::vector<std::string>& moves) {
    const std::string standardStartpos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    
    // Send ucinewgame if this is the start of a new game (no moves and at standard starting position)
    if (moves.empty() && startFen == standardStartpos)
        communication_->sendCommand("ucinewgame");
    
    // Build position command: "position startpos moves e2e4 e7e5 ..."
    // or: "position fen <fen> moves e2e4 e7e5 ..."
    std::string positionCommand;
    if (startFen == standardStartpos)
        positionCommand = "position startpos";
    else
        positionCommand = "position fen " + startFen;
    
    // Append moves if any
    if (!moves.empty()) {
        positionCommand += " moves";
        for (const auto& move : moves)
            positionCommand += " " + move;
    }
    
    communication_->sendCommand(positionCommand);
    communication_->sendCommand("go infinite");
    state_ = EngineState::Analyzing;
}

void UCIEngine::readEngineOutput() {
    // Peek first to avoid blocking if no data available
    if (!communication_->hasDataAvailable())
        return;
    
    // Read one line and process it
    std::string output = communication_->readResponseLine();
    if (!output.empty())
        parseEngineOutput(output);
}

void UCIEngine::parseEngineOutput(const std::string& output) {
    // Process single line directly (readResponseLine always returns one line)
    std::string line = UCIAnalysisParser::trim(output);
    
    // Early return if empty
    if (line.empty())
        return;
    
    // Ignore lines containing "currmove"
    if (UCIAnalysisParser::shouldIgnoreLine(line))
        return;
    
    // Check if it's an info line first (no lock needed for this check)
    bool isInfoLine = UCIAnalysisParser::isInfoLine(line);
    
    if (!isInfoLine) {
        // Not an info line - just update rawInfo
        std::lock_guard<std::mutex> lock(analysisMutex_);
        currentAnalysis_.rawInfo = line;
        return;
    }
    
    // This is an info line - do parsing work outside the lock
    AnalysisLine analysisLine = UCIAnalysisParser::parseAnalysisLine(line);
    
    // Update shared state (critical section)
    std::lock_guard<std::mutex> lock(analysisMutex_);
    currentAnalysis_.rawInfo = line;
    currentAnalysis_.hasResult = true;
    
    // If we see multipv 1, clear all existing lines (new batch/depth)
    if (analysisLine.multipv == 1)
        currentAnalysis_.lines.clear();
    
    // Merge into currentAnalysis_.lines by multipv (up to 4)
    bool found = false;
    for (auto& existing : currentAnalysis_.lines) {
        if (existing.multipv == analysisLine.multipv) {
            existing = analysisLine;
            found = true;
            break;
        }
    }
    if (!found && currentAnalysis_.lines.size() < 4)
        currentAnalysis_.lines.push_back(analysisLine);
}