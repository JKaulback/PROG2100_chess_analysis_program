#include "chess_analysis_program.h"

namespace GOCfg = Config::GameOver;

ChessAnalysisProgram::ChessAnalysisProgram() : 
    logic{}, gui{*this}, inputHandler{}, moveValidator{}, gameStateAnalyzer{},
    currentGameState{GameState::IN_PROGRESS}
{}

ChessAnalysisProgram::~ChessAnalysisProgram() 
{}

void ChessAnalysisProgram::run() {
    // Main loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        inputHandler.handleInput(*this, gui); // Input handler processes input through controller
        gui.draw(); // GUI only renders
    }
}

// Delegate methods to logic
ChessLogic::Piece ChessAnalysisProgram::getPieceAt(int rank, int file) const {
    return logic.getPieceAt(rank, file); // ChessLogic::Piece at board rank and file
}

std::string ChessAnalysisProgram::pieceToTextureString(ChessLogic::Piece piece) const {
    return logic.pieceToTextureString(piece); // String in form "wk" for texture rendering
}

ChessLogic::Player ChessAnalysisProgram::getCurrentPlayer() const {
    return logic.getCurrentPlayer(); // ChessLogic::Player for current player
}

std::vector<ChessLogic::Piece> ChessAnalysisProgram::getCapturedPieces() const {
    return logic.getCapturedPieces(); // Vector of ChessLogic::Pieces for currently captured pieces
}

ChessLogic::Player ChessAnalysisProgram::getPieceOwner(const ChessLogic::Piece piece) const {
    return logic.getPieceOwner(piece); // ChessLogic::Player for the current piece owner
}

int ChessAnalysisProgram::getHalfmoveClock() const {
    return logic.getHalfmoveClock(); // Integer representing the halfmove clock
}

std::string ChessAnalysisProgram::getCurrentFENString() const {
    return logic.getCurrentPositionString(); // String representing current game state (excludes halfmove and fullmove clock)
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

ChessLogic::Piece ChessAnalysisProgram::getDraggedPiece() const {
    return inputHandler.getDraggedPiece(); // ChessLogic::Piece for the piece being dragged
}

// Move validation and execution methods (Controller coordination)
bool ChessAnalysisProgram::attemptMove(
    const int srcRank,
    const int srcFile,
    const int destRank,
    const int destFile) {
    // 1. Validate the move using the validator
    MoveResult validationResult = moveValidator.validateMove(logic, srcRank, srcFile, destRank, destFile);
    
    // 2. If valid move, execute the move and switch turns
    if (isValidMoveResult(validationResult)) {
        // Handle different types of valid moves
        if (validationResult == MoveResult::VALID_CASTLE_KINGSIDE || validationResult == MoveResult::VALID_CASTLE_QUEENSIDE) {
            logic.executeCastling(srcRank, srcFile, destRank, destFile);
        } 
        else if (validationResult == MoveResult::VALID_EN_PASSANT) {
            logic.executeEnPassant(srcRank, srcFile, destRank, destFile);
        } 
        else if (validationResult == MoveResult::VALID_PROMOTION) {
            logic.executePromotion(srcRank, srcFile, destRank, destFile);
        }
        else {
            logic.executeMove(srcRank, srcFile, destRank, destFile);
        }
        
        logic.switchTurn(); // Switch to the other player
        
        // After successful move, analyze the new game state
        currentGameState = gameStateAnalyzer.analyzeGameState(logic);
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
        case GameState::IN_PROGRESS:
            gameOverText = ""; // No game over message
            break;
        default:
            gameOverText = GOCfg::ERROR_STRING;
            break;
    }
    return gameOverText;
}
