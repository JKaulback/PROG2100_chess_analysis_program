#include "chess_logic.h"

ChessLogic::ChessLogic() {
    // Initialize pieces array and board representation
    initializeBoard();
}

ChessLogic::~ChessLogic() {
}

void ChessLogic::initializeBoard() {
    // Set all squares to empty
    for ( int rank = 0; rank < board.size(); rank++ ) {
        for ( int file = 0; file < board[rank].size(); file++ ) {
            board[rank][file] = ChessLogic::Piece::EMPTY;
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
        board[whitePawnRank][file] = ChessLogic::Piece::WHITE_PAWN;
        board[blackPawnRank][file] = ChessLogic::Piece::BLACK_PAWN;
    }

    // Add back rank white pieces using move semantics
    ChessLogic::Piece whiteBackPieces[] = {
        ChessLogic::Piece::WHITE_ROOK, ChessLogic::Piece::WHITE_KNIGHT,
        ChessLogic::Piece::WHITE_BISHOP, ChessLogic::Piece::WHITE_QUEEN,
        ChessLogic::Piece::WHITE_KING, ChessLogic::Piece::WHITE_BISHOP,
        ChessLogic::Piece::WHITE_KNIGHT, ChessLogic::Piece::WHITE_ROOK
    };
    
    // Add back rank black pieces using move semantics
    ChessLogic::Piece blackBackPieces[] = {
        ChessLogic::Piece::BLACK_ROOK, ChessLogic::Piece::BLACK_KNIGHT,
        ChessLogic::Piece::BLACK_BISHOP, ChessLogic::Piece::BLACK_QUEEN,
        ChessLogic::Piece::BLACK_KING, ChessLogic::Piece::BLACK_BISHOP,
        ChessLogic::Piece::BLACK_KNIGHT, ChessLogic::Piece::BLACK_ROOK
    };

    for (int file = 0; file < 8; ++file) {
        board[whiteBackRank][file] = whiteBackPieces[file];
        board[blackBackRank][file] = blackBackPieces[file];
    }
}

ChessLogic::Piece ChessLogic::getPieceAt(const int rank, const int file) const {
    if (rank < 0 || rank >= 8 || file < 0 || file >= 8) {
        return ChessLogic::Piece::EMPTY; // Out of bounds
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
    if (board[fromRank][fromFile] == ChessLogic::Piece::EMPTY) {
        return; // No piece to move
    }

    // Check if there's a piece to capture
    if (board[toRank][toFile] != ChessLogic::Piece::EMPTY) {
        // Move the captured piece to captured pieces vector (efficient move)
        capturedPieces.push_back(std::move(board[toRank][toFile]));
    }

    // Move the piece efficiently using move semantics
    board[toRank][toFile] = std::move(board[fromRank][fromFile]);

    // Explicitly set from position to default ChessPiece
    board[fromRank][fromFile] = ChessLogic::Piece::EMPTY;
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
