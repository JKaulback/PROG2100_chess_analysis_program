#include "chess_analysis_program.h"
#include "../core/fen_loader.h"
#include <sstream>
#include <vector>
namespace GOCfg = Config::GameOver;

ChessAnalysisProgram::ChessAnalysisProgram() : 
    board{}, gameState{board}, fenStateHistory{}, moveValidator{},
    inputHandler{*this}, gameStateAnalyzer{}, 
    uciEngine{std::make_unique<UCIEngine>("src/analysis_engine/stockfish.exe")},
    gui{std::make_unique<ChessGUI>(*this)}, currentGameState{GameState::IN_PROGRESS}
    {
    // Initialize board to starting position first
    board.resetToStartingPosition();
    setCurrentPlayer('w');
    setCastlingRights(true, true, true, true);
    clearEnPassantTarget();
    setHalfmoveClock(0);
    setFullmoveClock(1);
    
    // Generate the starting FEN and set it as the starting position
    FENPositionTracker tempTracker;
    tempTracker.recordPosition(board, gameState);
    std::string startingFEN = tempTracker.getCurrentPosition();
    
    // Ensure we have a valid starting position before setting it
    if (startingFEN.empty() || startingFEN.length() < 10) {
        // Fallback to standard starting position
        startingFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    }
    
    fenStateHistory.setStartingPosition(startingFEN);
    
    // Try to load initial position from FEN file (this will override if file exists)
    FENLoader::loadFromFile("initial_position.fen", *this);
}

ChessAnalysisProgram::~ChessAnalysisProgram() 
{}

void ChessAnalysisProgram::run() {
    // Main loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        inputHandler.handleInput(*gui); // Input handler processes input through controller
        gui->draw(); // GUI only renders
    }
}

// Move validation and execution methods (Controller coordination)
bool ChessAnalysisProgram::attemptMove(const ChessMove& move) {
    // 1. Validate the move using the validator
    MoveResult validationResult = moveValidator.validateMove(board, gameState, move);

    // 2. If valid move, execute the move and switch turns
    if (isValidMoveResult(validationResult)) {
        // Update ChessGameState
        gameState.makeMove(move);

        // Update the board state
        if (validationResult == MoveResult::VALID_CASTLE_KINGSIDE || validationResult == MoveResult::VALID_CASTLE_QUEENSIDE)
            board.executeCastling(move);
        else if (validationResult == MoveResult::VALID_EN_PASSANT)
            board.executeEnPassant(move);
        else if (validationResult == MoveResult::VALID_PROMOTION) {
            // Set the piece to promote to
            char promoteTo =
                (getCurrentPlayer() == 'w') ?
                'q' :
                'Q';
            board.executePromotion(move, promoteTo);
        } else
            board.executeBasicMove(move);
        
        // Record position AFTER making the move
        fenStateHistory.record(board, gameState, move.toAlgebraicNotation());

        // Update UCI engine position
        setUCIEnginePosition();

        // After successful move, analyze the new game state
        currentGameState = gameStateAnalyzer.analyzeGameState(board, gameState, fenStateHistory);
        return true;
    }

    // 3. Move was invalid - return false
    return false;
}

// All valid moves from MoveResult
bool ChessAnalysisProgram::isValidMoveResult(MoveResult result) const  {
    return result == MoveResult::VALID || 
           result == MoveResult::VALID_CASTLE_KINGSIDE || 
           result == MoveResult::VALID_CASTLE_QUEENSIDE ||
           result == MoveResult::VALID_EN_PASSANT ||
           result == MoveResult::VALID_PROMOTION;
}

// Maps UI string to to currentGameState using Config 
std::string ChessAnalysisProgram::getGameOverString() const {
    std::string gameOverText;   
    switch (currentGameState) {
        case GameState::WHITE_WIN:
            gameOverText = GOCfg::WHITE_WIN_STRING;
            break;
        case GameState::BLACK_WIN:
            gameOverText = GOCfg::BLACK_WIN_STRING;
            break;
        case GameState::STALEMATE:
            gameOverText = GOCfg::STALEMATE_STRING;
            break;
        case GameState::DRAW_50_MOVES:
            gameOverText = GOCfg::DRAW_50_MOVES_STRING;
            break;
        case GameState::DRAW_THREEFOLD_REPETITION:
            gameOverText = GOCfg::DRAW_THREEFOLD_REPETITION_STRING;
            break;
        case GameState::DRAW_INSUFFICIENT_MATERIAL:
            gameOverText = GOCfg::DRAW_INSUFFICIENT_MATERIAL_STRING;
            break;
        default:
            gameOverText = GOCfg::ERROR_STRING;
            break;
    }
    return gameOverText;
}

void ChessAnalysisProgram::undoMove() {
    // Check if we have moves to undo (need at least 2 positions)
    if (fenStateHistory.getPositionHistory().size() > 1) {
        fenStateHistory.undoMove();
        PositionState targetState = fenStateHistory.getCurrentPositionState();
        
        if (!targetState.fenString.empty()) {
            applyPositionState(targetState);
            
            // Update game state analysis after undo
            currentGameState = gameStateAnalyzer.analyzeGameState(board, gameState, fenStateHistory);
            
            // Update UCI engine position if enabled
            if (isUCIEngineEnabled()) {
                setUCIEnginePosition();
            }
        }
    }
}

void ChessAnalysisProgram::redoMove() {
    // Check if we have moves to redo
    if (fenStateHistory.isRedoAvailable()) {
        fenStateHistory.redoMove();
        PositionState targetState = fenStateHistory.getCurrentPositionState();
        
        if (!targetState.fenString.empty()) {
            applyPositionState(targetState);
            
            // Update game state analysis after redo
            currentGameState = gameStateAnalyzer.analyzeGameState(board, gameState, fenStateHistory);
            
            // Update UCI engine position if enabled
            if (isUCIEngineEnabled()) {
                setUCIEnginePosition();
            }
        }
    }
}

void ChessAnalysisProgram::applyFen(const std::string& fenString) {
    FENLoader::applyFEN(fenString, *this);
}

// Update GUI state on change (reactive vs polling)
void ChessAnalysisProgram::setUCIEngineStateInGUI(const bool isEnabled) {
    gui->setIsUCIEngineRunning(isEnabled);
}

// FEN loader support methods
void ChessAnalysisProgram::clearBoard() {
    board.clearBoard();
    fenStateHistory.clearHistory();
    // Don't set starting position here - let the caller handle it
}

void ChessAnalysisProgram::clearBoardOnly() {
    board.clearBoard();
}

void ChessAnalysisProgram::applyFenDirect(const std::string& fenString) {
    // Parse FEN components
    std::istringstream iss(fenString);
    std::string boardPos, activeColor, castling, enPassant, halfmove, fullmove;
    iss >> boardPos >> activeColor >> castling >> enPassant >> halfmove >> fullmove;
    
    // Clear only the board pieces (not history)
    board.clearBoard();
    
    // Split board position by '/' to get individual ranks
    std::vector<std::string> ranks;
    std::stringstream ss(boardPos);
    std::string rank;
    while (std::getline(ss, rank, '/')) {
        ranks.push_back(rank);
    }
    
    // Process each rank
    for (int fenRank = 0; fenRank < 8 && fenRank < ranks.size(); fenRank++) {
        int boardRank = 7 - fenRank; // FEN rank 8 = board rank 7, FEN rank 1 = board rank 0
        int file = 0;
        
        // Process each character in this rank
        for (char c : ranks[fenRank]) {
            if (file >= 8) break; // Safety check
            
            if (c >= '1' && c <= '8') {
                // Empty squares - skip files
                file += (c - '0');
            } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                // Valid piece - place it
                board.setPieceAt(boardRank, file, c);
                file++;
            }
        }
    }
    
    // Set game state
    if (!activeColor.empty()) {
        gameState.setCurrentPlayer(activeColor[0]);
    }
    
    // Set castling rights
    bool wk = castling.find('K') != std::string::npos;
    bool wq = castling.find('Q') != std::string::npos;
    bool bk = castling.find('k') != std::string::npos;
    bool bq = castling.find('q') != std::string::npos;
    gameState.setCastlingRights(wk, wq, bk, bq);
    
    // Set en passant
    if (enPassant == "-") {
        gameState.clearEnPassantState();
    } else if (enPassant.length() == 2) {
        int file = enPassant[0] - 'a';
        int rank = enPassant[1] - '1';
        if (file >= 0 && file <= 7 && rank >= 0 && rank <= 7) {
            gameState.setEnPassantTarget(rank, file);
        }
    }
    
    // Set move clocks
    try {
        if (!halfmove.empty()) gameState.setHalfmoveClock(std::stoi(halfmove));
        if (!fullmove.empty()) gameState.setFullmoveClock(std::stoi(fullmove));
    } catch (const std::exception&) {
        // Ignore parsing errors
    }
}

void ChessAnalysisProgram::applyPositionState(const PositionState& state) {
    // Apply the FEN string for board position and game state
    applyFenDirect(state.fenString);
    
    // Restore captured pieces
    board.setCapturedPieces(state.whiteCapturedPieces, state.blackCapturedPieces);
}

void ChessAnalysisProgram::enableUCIEngine() {
    uciEngine->enable();
    
    // Get the position data
    std::string startPos = fenStateHistory.getStartPosition();
    std::vector<std::string> moves = fenStateHistory.getMoveHistory();
    
    // Debug: Force a standard starting position if none exists
    if (startPos.empty()) {
        startPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    }
    
    uciEngine->setPosition(startPos, moves);
}


void ChessAnalysisProgram::disableUCIEngine() {
    uciEngine->disable();
}

void ChessAnalysisProgram::toggleUCIEngine() {
    bool isEnabled = true;
    if (isUCIEngineEnabled()) {
        disableUCIEngine();
        isEnabled = false;
    } else
        enableUCIEngine();
    
    setUCIEngineStateInGUI(isEnabled);
}

void ChessAnalysisProgram::setUCIEnginePosition() {
    if (uciEngine && isUCIEngineEnabled()) {
        // Primary approach: Use start position + moves (better for engine understanding)
        std::string startFen = fenStateHistory.getStartPosition();
        std::vector<std::string> moves = fenStateHistory.getMoveHistory();
        
        // Ensure we have a valid starting position
        if (startFen.empty()) {
            startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        }

        uciEngine->setPosition(startFen, moves);
    }
}

void ChessAnalysisProgram::resetToInitialPosition() {
    // Reset the board to starting position
    board.resetToStartingPosition();
    
    // Reset game state
    setCurrentPlayer('w');
    setCastlingRights(true, true, true, true);
    clearEnPassantTarget();
    setHalfmoveClock(0);
    setFullmoveClock(1);
    currentGameState = GameState::IN_PROGRESS;

    // Generate starting FEN position
    FENPositionTracker tempTracker;
    tempTracker.recordPosition(board, gameState);
    std::string startingFEN = tempTracker.getCurrentPosition();

    // Clear position history and set starting position
    fenStateHistory.clearHistory();
    fenStateHistory.setStartingPosition(startingFEN);

    // Attempt to load from file (this will override if file exists)
    FENLoader::loadFromFile("initial_position.fen", *this);
    
    // Clear and update engine with new position if enabled
    if (isUCIEngineEnabled()) {
        uciEngine->clearAnalysis();
        setUCIEnginePosition();
    }
}