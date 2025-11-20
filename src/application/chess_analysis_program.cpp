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

// Delegate methods to logic
char ChessAnalysisProgram::getPieceAt(int rank, int file) const {
    return board.getPieceAt(rank, file); // char at board rank and file
}

std::string ChessAnalysisProgram::pieceToTextureString(char piece) const {
    return board.getPieceTextureString(piece); // String in form "wk" for texture rendering
}

char ChessAnalysisProgram::getCurrentPlayer() const {
    return gameState.getCurrentPlayer(); // char for current player
}

std::vector<char> ChessAnalysisProgram::getCapturedPieces() const {
    return board.getCapturedPieces(); // Vector of chars for currently captured pieces
}

char ChessAnalysisProgram::getPieceOwner(int rank, int file) const {
    return board.getPieceOwner(rank, file); // char for the current piece owner
}

char ChessAnalysisProgram::getPieceOwner(char piece) const {
    return board.getPieceOwner(piece);
}

int ChessAnalysisProgram::getHalfmoveClock() const {
    return gameState.getHalfmoveClock(); // Integer representing the halfmove clock
}

std::string ChessAnalysisProgram::getCurrentFENString() const {
    return fenStateHistory.getCurrentPosition(); // TODO: FEN string manager
}

// Delegate methods to input handler
bool ChessAnalysisProgram::getIsDragging() const {
    return inputHandler.getIsDragging(); // Whether the user is dragging a piece
}

int ChessAnalysisProgram::getDraggedPieceRank() const {
    return inputHandler.getDraggedPieceRank(); // The original rank of a currently dragged piece
}

int ChessAnalysisProgram::getDraggedPieceFile() const {
    return inputHandler.getDraggedPieceFile(); // The original file of a currently dragged piece
}

Vector2 ChessAnalysisProgram::getDragOffset() const {
    return inputHandler.getDragOffset(); // Offset coords to prevent the piece from "jumping" to mouse position
}

char ChessAnalysisProgram::getDraggedPiece() const {
    return inputHandler.getDraggedPiece(); // char for the piece being dragged
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
        else if (validationResult == MoveResult::VALID_PROMOTION)
            board.executePromotion(move);
        else
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

GameState ChessAnalysisProgram::getGameState() const {
    return currentGameState;
}

bool ChessAnalysisProgram::isGameOver() const {
    return currentGameState != GameState::IN_PROGRESS;
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
void ChessAnalysisProgram::setPieceAt(const int rank, const int file, const char piece) {
    board.setPieceAt(rank, file, piece);
}

void ChessAnalysisProgram::clearBoard() {
    board.clearBoard();
    fenStateHistory.clearHistory();
    fenStateHistory.setStartingPosition(getCurrentFENString());
}

void ChessAnalysisProgram::setCurrentPlayer(const char player) {
    gameState.setCurrentPlayer(player);
}

void ChessAnalysisProgram::setCastlingRights(bool whiteKingside, bool whiteQueenside, bool blackKingside, bool blackQueenside) {
    gameState.setCastlingRights(whiteKingside, whiteQueenside, blackKingside, blackQueenside);
}

void ChessAnalysisProgram::setEnPassantTarget(const int rank, const int file) {
    gameState.setEnPassantTarget(rank, file);
}

void ChessAnalysisProgram::clearEnPassantTarget() {
    gameState.clearEnPassantState();
}

void ChessAnalysisProgram::setHalfmoveClock(const int halfmoves) {
    gameState.setHalfmoveClock(halfmoves);
}

void ChessAnalysisProgram::setFullmoveClock(const int fullmoves) {
    gameState.setFullmoveClock(fullmoves);
}

void ChessAnalysisProgram::enableUCIEngine() {
    uciEngine->enable();
    uciEngine->setPosition(
        fenStateHistory.getStartPosition(),
        fenStateHistory.getMoveHistory()
    );
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

bool ChessAnalysisProgram::isUCIEngineEnabled() const {
    return uciEngine->isEnabled();
}

void ChessAnalysisProgram::setUCIEnginePosition() {
    if (uciEngine && isUCIEngineEnabled()) {
        std::string startFen = fenStateHistory.getStartPosition();
        std::vector<std::string> moves = fenStateHistory.getMoveHistory();

        uciEngine->setPosition(startFen, moves);
    }
}

EngineAnalysis ChessAnalysisProgram::pollUCIEngineAnalysis() {
    return uciEngine->pollAnalysis();
}