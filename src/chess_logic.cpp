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

    // Initialize chess pieces pieces
    initPieces();

}

void ChessLogic::initPieces() {


    // Determine ranks for this color
    int whitePawnRank = 1;
    int whiteBackRank = 0;
    int blackPawnRank = 6;
    int blackBackRank = 7;
    
    // Add pawns using move semantics for efficiency
    for (int file = 0; file < 8; ++file) {
        board[whitePawnRank][file] = ChessPiece(ChessPiece::PieceType::WHITE_PAWN);
        board[blackPawnRank][file] = ChessPiece(ChessPiece::PieceType::BLACK_PAWN);
    }

    // Add back rank white pieces using move semantics
    ChessPiece::PieceType whiteBackPieces[] = {
        ChessPiece::PieceType::WHITE_ROOK, ChessPiece::PieceType::WHITE_KNIGHT,
        ChessPiece::PieceType::WHITE_BISHOP, ChessPiece::PieceType::WHITE_QUEEN,
        ChessPiece::PieceType::WHITE_KING, ChessPiece::PieceType::WHITE_BISHOP,
        ChessPiece::PieceType::WHITE_KNIGHT, ChessPiece::PieceType::WHITE_ROOK
    };
    
    // Add back rank black pieces using move semantics
    ChessPiece::PieceType blackBackPieces[] = {
        ChessPiece::PieceType::BLACK_ROOK, ChessPiece::PieceType::BLACK_KNIGHT,
        ChessPiece::PieceType::BLACK_BISHOP, ChessPiece::PieceType::BLACK_QUEEN,
        ChessPiece::PieceType::BLACK_KING, ChessPiece::PieceType::BLACK_BISHOP,
        ChessPiece::PieceType::BLACK_KNIGHT, ChessPiece::PieceType::BLACK_ROOK
    };

    for (int file = 0; file < 8; ++file) {
        board[whiteBackRank][file] = ChessPiece(whiteBackPieces[file]);
        board[blackBackRank][file] = ChessPiece(blackBackPieces[file]);
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
    if (board[fromRank][fromFile].getType() == ChessPiece::PieceType::EMPTY) {
        return; // No piece to move
    }

    // Check if there's a piece to capture
    if (board[toRank][toFile].getType() != ChessPiece::PieceType::EMPTY) {
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
