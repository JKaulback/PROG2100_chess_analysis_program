#include "fen_position_tracker.h"
#include "../../config/config.h"

namespace BoardCfg = Config::Board;

FENPositionTracker::FENPositionTracker() {
    positionHistory = {};
    positionRedo = {};
    moves = {};
    movesRedo = {};
}

void FENPositionTracker::recordPosition(const ChessBoard& board, const ChessGameState& gameState) {
    std::string newPosition = "";

    newPosition += getBoardState(board);
    newPosition += " ";

    newPosition += std::string(1, gameState.getCurrentPlayer());
    newPosition += " ";

    newPosition += getCastlingRights(gameState);
    newPosition += " ";

    newPosition += getEnPassantTarget(gameState);
    newPosition += " ";

    newPosition += std::to_string(gameState.getHalfmoveClock());
    newPosition += " ";
    
    newPosition += std::to_string(gameState.getFullmoveClock());

    // Create PositionState with FEN and captured pieces
    PositionState newState(newPosition, board.getWhiteCapturedPieces(), board.getBlackCapturedPieces());
    positionHistory.push_back(newState);

    if (!positionRedo.empty()) {
        if (newPosition == positionRedo.back().fenString)
            positionRedo.pop_back(); // Pop position redone from stack
        else
            positionRedo.clear(); // Clear stack; different future
    }
}

const std::vector<std::string>& FENPositionTracker::getPositionHistory() const {
    // Need to create a static vector to return
    static std::vector<std::string> fenHistory;
    fenHistory.clear();
    for (const auto& state : positionHistory) {
        fenHistory.push_back(state.fenString);
    }
    return fenHistory;
}

std::string FENPositionTracker::getStartPosition() const {
    return 
        (positionHistory.empty()) ?
        "" :
        positionHistory.front().fenString;
}

std::string FENPositionTracker::getCurrentPosition() const {
    return 
        (positionHistory.empty()) ?
        "" :
        positionHistory.back().fenString;
}

void FENPositionTracker::record(
    const ChessBoard& board, 
    const ChessGameState& gameState, 
    const std::string& algebraicMove) {
    
    recordPosition(board, gameState);
    recordMove(algebraicMove);
}

void FENPositionTracker::undoMove() {
    if (!moves.empty()) {
        PositionState lastPosition = positionHistory.back();
        positionHistory.pop_back();
        positionRedo.push_back(lastPosition);

        std::string lastMove = moves.back();
        moves.pop_back();
        movesRedo.push_back(lastMove);
    }
}

void FENPositionTracker::redoMove() {
    if (!movesRedo.empty()) {
        PositionState nextPosition = positionRedo.back();
        positionRedo.pop_back();
        positionHistory.push_back(nextPosition);

        std::string nextMove = movesRedo.back();
        movesRedo.pop_back();
        moves.push_back(nextMove);
    }
}

const bool FENPositionTracker::isUndoAvailable() const {
    // We can undo if we have more than 1 position (initial position + at least 1 move)
    return positionHistory.size() > 1;
}

const bool FENPositionTracker::isRedoAvailable() const {
    return !movesRedo.empty();
}

const std::string FENPositionTracker::getRedoPosition() const {
    return
        (positionRedo.empty()) ?
        "" :
        positionRedo.back().fenString;
}

const std::string FENPositionTracker::getRedoMove() const {
    return
        (movesRedo.empty()) ?
        "" :
        moves.back();
}

const std::vector<std::string>& FENPositionTracker::getRedoPositions() const {
    // Need to create a static vector to return
    static std::vector<std::string> redoFenHistory;
    redoFenHistory.clear();
    for (const auto& state : positionRedo) {
        redoFenHistory.push_back(state.fenString);
    }
    return redoFenHistory;
}

const std::vector<std::string>& FENPositionTracker::getRedoMoves() const {
    return movesRedo;
}

void FENPositionTracker::recordMove(const std::string& algebraicMove) {
    moves.push_back(algebraicMove);

    if (!movesRedo.empty()){
        if (algebraicMove == movesRedo.back())
            movesRedo.pop_back(); // Pop move being redone from stack
        else
            movesRedo.clear(); // Clear stack; different future
    }
}

void FENPositionTracker::setStartingPosition(const std::string& fen) {
    positionHistory.clear();
    // Starting position has no captured pieces
    PositionState startState(fen, {}, {});
    positionHistory.push_back(startState);
}

void FENPositionTracker::clearHistory() {
    positionHistory.clear();
    moves.clear();
}

const std::vector<std::string>& FENPositionTracker::getMoveHistory() const {
    return moves;
}

bool FENPositionTracker::isThreefoldRepetition() const {
    if (positionHistory.empty())
        return false;

    // Only need to track the last move
    std::string currentPos = extractPositionKey(positionHistory.back().fenString);
    int count = 0;

    for (const auto& state : positionHistory) {
        if (extractPositionKey(state.fenString) == currentPos){
            count++;
            if (count >= 3)
                return true;
        }
    }
    return false;
}

PositionState FENPositionTracker::getCurrentPositionState() const {
    return positionHistory.empty() ? PositionState() : positionHistory.back();
}

PositionState FENPositionTracker::getRedoPositionState() const {
    return positionRedo.empty() ? PositionState() : positionRedo.back();
}

std::string FENPositionTracker::getBoardState(const ChessBoard& board) const {
    std::string positionString = "";
    int emptyCount = 0;

    // FEN notation goes from rank 8 to rank 1 (top to bottom)  
    // Our internal representation: rank 7 = FEN rank 8, rank 0 = FEN rank 1
    for (int rank = BoardCfg::MAX_RANK; rank >= BoardCfg::MIN_RANK; rank--) {
        
        for (int file = BoardCfg::MIN_FILE; file <= BoardCfg::MAX_FILE; file++) {
            
            char piece = board.getPieceAt(rank, file);
            
            if (piece == BoardCfg::EMPTY) {
                emptyCount++;
                continue;
            }
            
            if (emptyCount > 0) {
                positionString += std::to_string(emptyCount);
                emptyCount = 0;
            }

            positionString += std::string(1, piece);
        }
        
        // Add remaining empty squares at end of rank
        if (emptyCount > 0) {
            positionString += std::to_string(emptyCount);
        }
        // Reset empty count
        emptyCount = 0;
        // Add rank separator (except for the last rank)
        if (rank != BoardCfg::MIN_RANK) {
            positionString += "/";
        }
    }

    return positionString;
}

std::string FENPositionTracker::getCastlingRights(const ChessGameState& gameState) const {
    std::string castlingString = "";

    if (gameState.canCastleKingside('w'))
        castlingString += "K";
    if (gameState.canCastleQueenside('w'))
        castlingString += "Q";
    if (gameState.canCastleKingside('b'))
        castlingString += "k";
    if (gameState.canCastleQueenside('b'))
        castlingString += "q";
    if (castlingString.empty())
        castlingString = "-";
    
    return castlingString;
}

std::string FENPositionTracker::getEnPassantTarget(const ChessGameState& gameState) const {
    std::string enPassantString = "-";

    if (gameState.isEnPassantAvailable()){
        auto [rank, file] = gameState.getEnPassantTarget();
        
        // Convert to algebraic notition
        char fileChar = 'a' + file;
        char rankChar = '1' + rank;

        enPassantString = std::string(1, fileChar) + std::string(1, rankChar);
    }

    return enPassantString;
}

std::string FENPositionTracker::extractPositionKey(const std::string& fenString) const {
    
    std::istringstream iss(fenString);
    std::string board, activeColor, castling, enPassant;

    // Whitespace is the default delimiter when using extraction operator
    iss >> board >> activeColor >> castling >> enPassant;

    return board + " " + activeColor + " " + castling + " " + enPassant;
}