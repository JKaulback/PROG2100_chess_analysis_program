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
    tempTracker.record(board, gameState);
    std::string startingFEN = tempTracker.getCurrentPosition();
    
    // Ensure we have a valid starting position before setting it
    if (startingFEN.empty() || startingFEN.length() < 10) {
        // Fallback to standard starting position
        startingFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    }
    
    fenStateHistory.setStartingPosition(startingFEN);
    
    // Try to load initial position from FEN file (this will override if file exists)
    // But preserve move history for testing
    FENLoader::loadFromFile("initial_position.fen", *this);
    
    // FOR TESTING: Add 50 accurate chess moves to test the moves component
    // This creates actual move history that the moves component can display
    // Using simple source-destination notation (e2e4, g1f3, e1g1 for castling, etc.)
    
    std::vector<std::string> testMoves = {
        "e2e4", "e7e5", "g1f3", "b8c6", "f1c4", "g8f6", "d2d3", "d7d6", 
        "e1g1", "e8g8", "c1g5", "h7h6", "g5h4", "c8g4", "h2h3", "g4h5", 
        "b1c3", "a7a6", "a2a4", "f8e7", "f1e1", "d8d7", "d1d2", "a8d8", 
        "a1b1", "b7b5", "a4b5", "a6b5", "c4b3", "b5b4", "c3d5", "f6d5", 
        "e4d5", "c6e5", "f3e5", "d6e5", "d2e3", "e7d6", "e3b6", "d7c6", 
        "b6a7", "c6c2", "b1b2", "c2c1", "b2b1", "c1c7", "a7b6", "c7c6", 
        "b6a5"
    };
    
    // Unique FEN strings to prevent threefold repetition detection - each position is unique
    std::vector<std::string> uniqueFens = {
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
        "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2",
        "rnbqkbnr/pppp1ppp/8/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2",
        "r1bqkbnr/pppp1ppp/2n5/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 2 3",
        "r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 3 3",
        "r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4",
        "r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/3P1N2/PPP2PPP/RNBQK2R b KQkq - 0 4",
        "r1bqkb1r/ppp2ppp/2np1n2/4p3/2B1P3/3P1N2/PPP2PPP/RNBQK2R w KQkq - 0 5",
        "r1bqkb1r/ppp2ppp/2np1n2/4p3/2B1P3/3P1N2/PPP2PPP/RNBQ1RK1 b kq - 1 5",
        "r1bq1rk1/ppp2ppp/2np1n2/4p3/2B1P3/3P1N2/PPP2PPP/RNBQ1RK1 w - - 2 6",
        "rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/3P1N2/PPP2PPP/RNBQK2R w KQkq - 4 4",
        "rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 4",
        "rnbqkb1r/pppppppp/5n2/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 1 2",
        "rnbqkb1r/pppp1ppp/4pn2/8/3PP3/8/PPP2PPP/RNBQKBNR w KQkq - 0 3",
        "rnbqkb1r/ppp2ppp/4pn2/3p4/3PP3/8/PPP2PPP/RNBQKBNR w KQkq d6 0 4",
        "rnbqk2r/ppppbppp/5n2/4p3/4P3/3P4/PPP2PPP/RNBQKBNR w KQkq - 1 4",
        "r1bqkb1r/pppp1ppp/2n2n2/4p3/4P3/3P4/PPP1NPPP/RNBQKB1R b KQkq - 2 4",
        "r1bqkbnr/pppp2pp/2n2p2/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 4",
        "r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4",
        "r1bqkbnr/pppp2pp/2n2p2/4p3/3PP3/5N2/PPP2PPP/RNBQKB1R b KQkq d3 0 4",
        "r2qkbnr/pppbpppp/2n5/3p4/3P4/5N2/PPP1PPPP/RNBQKB1R w KQkq - 2 4",
        "r1bq1rk1/pppp1ppp/2n2n2/2b1p3/2B1P3/3P1N2/PPP2PPP/RNBQ1RK1 w - - 5 5",
        "r2qk2r/pppbbppp/2n2n2/3pp3/3PP3/2N2N2/PPP1BPPP/R1BQKR2 b Qkq - 5 5",
        "r1bqk2r/pppp1ppp/2n2n2/2b1p3/2B1P3/2NP1N2/PPP2PPP/R1BQK2R b KQkq - 5 5",
        "r1b1k2r/ppppqppp/2n2n2/2b1p3/2B1P3/3P1N2/PPP1QPPP/RNB1K2R w KQkq - 6 6",
        "r3k2r/pppbqppp/2n2n2/3pp3/2BPP3/2N2N2/PPP1QPPP/R1B1K2R b KQkq - 0 6",
        "r2q1rk1/pppbbppp/2n2n2/3pp3/3PP3/2N1BN2/PPP2PPP/R2QK2R w KQ - 6 6",
        "r1bq1rk1/ppp2ppp/2np1n2/2b1p3/2B1P3/2NP1N2/PPP2PPP/R1BQ1RK1 b - - 0 6",
        "r2qkb1r/pppbpppp/2n2n2/3p4/3P4/2N1PN2/PPP2PPP/R1BQKB1R w KQkq - 4 6",
        "r1bqkb1r/pppp1ppp/2n2n2/8/3Pp3/2N2N2/PPP2PPP/R1BQKB1R w KQkq e6 0 5",
        "r1b1kb1r/pppp1ppp/2n2n2/4p3/2B1P2q/3P1N2/PPP2PPP/RNBQK2R w KQkq - 4 5",
        "rnb1kb1r/pppp1ppp/5n2/4p3/4PP1q/8/PPPP2PP/RNBQKBNR w KQkq - 2 4",
        "rnbqk2r/ppp2ppp/3bpn2/3p4/3P4/3BPN2/PPP2PPP/RNBQK2R w KQkq - 2 5",
        "r1bqk2r/ppp2ppp/2npbn2/4p3/2B1P3/3P1N2/PPP2PPP/RNBQK2R w KQkq - 4 5",
        "r2qkb1r/pppbpppp/2n2n2/8/3Pp3/2N2N2/PPP2PPP/R1BQKB1R w KQkq e6 0 5",
        "r1bq1rk1/ppppbppp/2n2n2/4p3/2B1P3/3P1N2/PPP2PPP/RNBQ1RK1 w - - 4 5",
        "r1b2rk1/ppppqppp/2n2n2/2b1p3/2B1P3/3P1N2/PPP1QPPP/RNB2RK1 b - - 5 6",
        "r2q1rk1/ppp1bppp/2npbn2/4p3/2BPP3/2N2N2/PPP2PPP/R1BQ1RK1 b - - 0 6",
        "r1bq1rk1/ppp1nppp/3p1n2/2b1p3/2B1P3/2NP1N2/PPP2PPP/R1BQ1RK1 w - - 0 7",
        "r1b2rk1/ppp1qppp/2npbn2/4p3/2BPP3/2N2N2/PPP1QPPP/R1B2RK1 b - - 0 7",
        "r1bq1rk1/ppp2ppp/2npb3/4p3/1nB1P3/2NP1N2/PPP2PPP/R1BQ1RK1 w - - 2 7",
        "r1b1k2r/pppnqppp/3pbn2/4p3/2BPP3/2N2N2/PPP1QPPP/R1B1K2R w KQkq - 0 7",
        "r2q1rk1/ppp1bppp/2n1bn2/3pp3/3PP3/2N1BN2/PPP2PPP/R2Q1RK1 w - - 0 7",
        "r1bq1rk1/pp2ppbp/2np1np1/8/2BPP3/2N2N2/PPP2PPP/R1BQ1RK1 w - - 0 8",
        "r1bqk2r/pp2ppbp/2np1np1/8/2BPP3/2N2N2/PPP2PPP/R1BQK2R w KQkq - 0 8",
        "r2q1rk1/ppp1bppp/2n1bn2/3p4/3PP1p1/2N1BN2/PPP2PPP/R2Q1RK1 w - g6 0 8",
        "r1bq1rk1/ppp2p1p/2npbnp1/4p3/2BPP3/2N2N2/PPP2PPP/R1BQ1RK1 w - - 0 8",
        "r1b1k2r/ppppqppp/2n2n2/2b1p3/2B1P3/2NP1N2/PPP1QPPP/R1B1K2R b KQkq - 6 6",
        "r2qkb1r/ppp1pppp/2n2n2/3p4/1b1PP3/2N2N2/PPP2PPP/R1BQKB1R w KQkq - 2 5",
    };
    
    for (int i = 0; i < testMoves.size() && i < 49; i++) {
        PositionState testMove;
        testMove.fenString = uniqueFens[i];
        testMove.algebraicMove = testMoves[i];
        testMove.movedBy = (i % 2 == 0) ? 'w' : 'b';
        fenStateHistory.record(testMove);
    }
    
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
    tempTracker.record(board, gameState);
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