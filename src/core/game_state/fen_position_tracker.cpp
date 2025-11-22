#include "fen_position_tracker.h"
#include "../../config/config.h"

namespace BoardCfg = Config::Board;

FENPositionTracker::FENPositionTracker() {
    positionHistory = {};
    positionRedo = {};
}

const std::vector<PositionState>& FENPositionTracker::getPositionHistory() const {
    return positionHistory;
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

    // Create PositionState with FEN, captured pieces, algebraic move, and move owner
    PositionState newState(
        newPosition, 
        board.getWhiteCapturedPieces(), 
        board.getBlackCapturedPieces(), 
        algebraicMove,
        gameState.getCurrentPlayer());
    
    positionHistory.push_back(newState);

    if (!positionRedo.empty()) {
        if (newPosition == positionRedo.back().fenString)
            positionRedo.pop_back(); // Pop position redone from stack
        else
            positionRedo.clear(); // Clear stack; different future
    }
}

void FENPositionTracker::record(const PositionState& state) {
    positionHistory.push_back(state);
    if (!positionRedo.empty()) {
        if (state.fenString == positionRedo.back().fenString)
            positionRedo.pop_back(); // Pop position redone from stack
        else
            positionRedo.clear(); // Clear stack; different future
    }
}

void FENPositionTracker::record(const ChessBoard& board, const ChessGameState& gameState) {
    record(board, gameState, "");
}

void FENPositionTracker::undoMove() {
    if (positionHistory.size() > 1) { // Need at least 2 positions to undo
        PositionState lastPosition = positionHistory.back();
        positionHistory.pop_back();
        positionRedo.push_back(lastPosition);
    }
}

void FENPositionTracker::redoMove() {
    if (!positionRedo.empty()) {
        PositionState nextPosition = positionRedo.back();
        positionRedo.pop_back();
        positionHistory.push_back(nextPosition);
    }
}

const bool FENPositionTracker::isUndoAvailable() const {
    // We can undo if we have more than 1 position (initial position + at least 1 move)
    return positionHistory.size() > 1;
}

const bool FENPositionTracker::isRedoAvailable() const {
    return !positionRedo.empty();
}

const std::string FENPositionTracker::getRedoPosition() const {
    return
        (positionRedo.empty()) ?
        "" :
        positionRedo.back().fenString;
}

const std::string FENPositionTracker::getRedoMove() const {
    return
        (positionRedo.empty()) ?
        "" :
        positionRedo.back().algebraicMove;
}

const std::vector<PositionState>& FENPositionTracker::getRedoPositions() const {
    return positionRedo;
}

void FENPositionTracker::setStartingPosition(const std::string& fen) {
    clearHistory();
    // Starting position has no captured pieces and no move (empty string)
    PositionState startState(fen, {}, {}, "");
    positionHistory.push_back(startState);
}

void FENPositionTracker::clearHistory() {
    positionHistory.clear();
    positionRedo.clear();
}

const std::vector<std::string> FENPositionTracker::getMoveHistory() const {
    std::vector<std::string> moves;
    for (auto position : positionHistory) {
        moves.push_back(position.algebraicMove);
    }
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

std::string FENPositionTracker::getCurrentMove() const {
    return positionHistory.empty() ? "" : positionHistory.back().algebraicMove;
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