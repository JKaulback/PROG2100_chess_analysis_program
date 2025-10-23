#include "chess_logic.h"

ChessLogic::ChessLogic() {
    // Initialize pieces array and board representation
    initializeBoard();
}

ChessLogic::~ChessLogic() {
}

void ChessLogic::initializeBoard() {
    // Clear the board
    for ( int rank = 0; rank < board.size(); rank++ ) {
        for ( int file = 0; file < board[rank].size(); file++ ) {
            board[rank][file] = ChessPiece();
        }
    }

    // Initialize white pieces
    initPieces(ChessPiece::PieceColor::WHITE_PIECE);

    // Init black pieces
    initPieces(ChessPiece::PieceColor::BLACK_PIECE);

}

void ChessLogic::initPieces(ChessPiece::PieceColor color) {


    // Determine ranks for this color
    int pawnRank = (color == ChessPiece::PieceColor::WHITE_PIECE) ? 1 : 6;
    int backRank = (color == ChessPiece::PieceColor::WHITE_PIECE) ? 0 : 7;
    
    // Add pawns using move semantics for efficiency
    for (int file = 0; file < 8; ++file) {
        board[pawnRank][file] = ChessPiece(ChessPiece::PieceType::PAWN, color);
    }

    // Add back rank pieces using move semantics
    ChessPiece::PieceType backPieces[] = {
        ChessPiece::PieceType::ROOK, ChessPiece::PieceType::KNIGHT,
        ChessPiece::PieceType::BISHOP, ChessPiece::PieceType::QUEEN,
        ChessPiece::PieceType::KING, ChessPiece::PieceType::BISHOP,
        ChessPiece::PieceType::KNIGHT, ChessPiece::PieceType::ROOK
    };
    
    for (int file = 0; file < 8; ++file) {
        board[backRank][file] = ChessPiece(backPieces[file], color);
    }
}

ChessPiece ChessLogic::getPieceAt(const int rank, const int file) const {
    if (rank < 0 || rank >= 8 || file < 0 || file >= 8) {
        return ChessPiece(); // Out of bounds
    }
    return board[rank][file]; // Return the piece in the array
}

void ChessLogic::makeMove(const int fromRank, const int fromFile,
    const int toRank, const int toFile) {
    // Validate move in board boundaries
    if (!isValidMove(fromRank, fromFile, toRank, toFile)) {
        return; // Move not valid 
    }

    // Check if there's a piece to move
    if (board[fromRank][fromFile].getType() == ChessPiece::PieceType::NONE) {
        return; // No piece to move
    }

    // Check if there's a piece to capture
    if (board[toRank][toFile].getType() != ChessPiece::PieceType::NONE) {
        // Move the captured piece to captured pieces vector (efficient move)
        capturedPieces.push_back(std::move(board[toRank][toFile]));
    }

    // Move the piece efficiently using move semantics
    board[toRank][toFile] = std::move(board[fromRank][fromFile]);

    // Explicitly set from position to default ChessPiece
    board[fromRank][fromFile] = ChessPiece();
}

bool ChessLogic::isValidMove(const int fromRank, const int fromFile, const int toRank, const int toFile) const{
    // Check if ranks and files are within board bounds
    if (fromRank < 0 || fromRank > 7 || 
        fromFile < 0 || fromFile > 7 ||
        toRank < 0 || toRank > 7 || 
        toFile < 0 || toFile > 7) {
        
        return false;
    }

    // Check if attempting to move to the same square
    if (fromRank == toRank && fromFile == toFile) {
        return false;
    }

    return true;
}
