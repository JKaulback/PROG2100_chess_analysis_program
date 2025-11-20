#include "fen_position_tracker.h"

FENPositionTracker::FENPositionTracker() {
    positionHistory = {};
    moves = {};
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

    positionHistory.push_back(newPosition);
    
}

std::vector<std::string> FENPositionTracker::getPositionHistory() const {
    return positionHistory;
}

std::string FENPositionTracker::getStartPosition() const {
    return 
        (positionHistory.empty()) ?
        "" :
        positionHistory.front();
}

std::string FENPositionTracker::getCurrentPosition() const {
    return 
        (positionHistory.empty()) ?
        "" :
        positionHistory.back();
}

bool FENPositionTracker::isThreefoldRepetition() const {
    if (positionHistory.empty())
        return false;

    // Only need to track the last move
    std::string currentPos = extractPositionKey(positionHistory.back());
    int count = 0;

    for (const std::string& fen : positionHistory) {
        if (extractPositionKey(fen) == currentPos){
            count++;
            if (count >= 3)
                return true;
        }
    }
    return false;
}

void FENPositionTracker::recordMove(const std::string& algebraicMove) {
    moves.push_back(algebraicMove);
}

void FENPositionTracker::setStartingPosition(const std::string& fen) {
    positionHistory.clear();
    positionHistory.push_back(fen);
}

void FENPositionTracker::clearHistory() {
    positionHistory.clear();
    moves.clear();
}

const std::vector<std::string>& FENPositionTracker::getMoveHistory() const {
    return moves;
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