#include "chess_logic.h"

ChessLogic::ChessLogic() {
    // Initialize pieces array and board representation
    initializeBoard();
}

ChessLogic::~ChessLogic() {
}

void ChessLogic::initializeBoard() {
    // Place pieces on the board
    int pieceIndex = 0;

    // Add white pieces
    addPieces(ChessPiece::PieceColor::WHITE_PIECE);

    // Add black pieces
    addPieces(ChessPiece::PieceColor::BLACK_PIECE);

}

void ChessLogic::addPieces(ChessPiece::PieceColor color) {
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

ChessPiece ChessLogic::getPieceAt(int rank, int file) const {
    if (rank < 0 || rank >= 8 || file < 0 || file >= 8) {
        return ChessPiece(); // Out of bounds
    }
    return board[rank][file]; // Return the piece in the array
}

void ChessLogic::makeMove(int fromRank, int fromFile, int toRank, int toFile) {
    // Validate move in board boundaries
    if (fromRank < 0 || fromRank >= 8 || fromFile < 0 || fromFile >= 8 ||
        toRank < 0 || toRank >= 8 || toFile < 0 || toFile >= 8) {
        return; // Invalid move
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
    
}