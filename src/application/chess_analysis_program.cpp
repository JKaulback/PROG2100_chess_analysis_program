#include "chess_analysis_program.h"
#include "../core/fen_loader.h"

namespace GOCfg = Config::GameOver;

ChessAnalysisProgram::ChessAnalysisProgram() : 
    board{}, gameState{board}, fenStateHistory{}, moveValidator{},
    inputHandler{*this}, gameStateAnalyzer{}, 
    uciEngine{std::make_unique<UCIEngine>("src/analysis_engine/stockfish.exe")},
    gui{std::make_unique<ChessGUI>(*this)}, currentGameState{GameState::IN_PROGRESS}
    {
    // Try to load initial position from FEN file
    FENLoader::loadFromFile("initial_position.fen", *this);
    fenStateHistory.setStartingPosition(getCurrentFENString());
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
        
        // Update position tracker
        fenStateHistory.recordPosition(board, gameState);
        fenStateHistory.recordMove(move.toAlgebraicNotation());

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

// Update GUI state on change (reactive vs polling)
void ChessAnalysisProgram::setUCIEngineStateInGUI(const bool isEnabled) {
    gui->setIsUCIEngineRunning(isEnabled);
}

// FEN loader support methods
void ChessAnalysisProgram::clearBoard() {
    board.clearBoard();
    fenStateHistory.clearHistory();
    fenStateHistory.setStartingPosition(getCurrentFENString());
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

    // Attempt to load from file
    FENLoader::loadFromFile("initial_position.fen", *this);

    // Clear position history and set starting position
    fenStateHistory.clearHistory();
    fenStateHistory.setStartingPosition(getCurrentFENString());

    
    // Clear and update engine with new position if enabled
    if (isUCIEngineEnabled()) {
        uciEngine->clearAnalysis();
        setUCIEnginePosition();
    }
}