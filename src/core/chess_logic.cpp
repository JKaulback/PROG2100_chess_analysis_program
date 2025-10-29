#include "chess_logic.h"

// Namespace alias for cleaner code while maintaining clarity
namespace BoardCfg = Config::Board;

ChessLogic::ChessLogic() 
{
    // Initialize pieces array and board representation
    initializeBoard();
}

ChessLogic::~ChessLogic() 
{}

void ChessLogic::initializeBoard() 
{
    // Set all squares to empty
    for (int rank = 0; rank < board.size(); rank++) 
    {
        for (int file = 0; file < board[rank].size(); file++) 
        {
            board[rank][file] = ChessLogic::Piece::EMPTY;
        }
    }

    // Initialize chess pieces pieces
    initPieces();
}

void ChessLogic::initPieces() 
{
    // Determine ranks for this color
    int whitePawnRank = 1;
    int whiteBackRank = 0;
    int blackPawnRank = 6;
    int blackBackRank = 7;
    
    // Add pawns using move semantics for efficiency
    for (int file = 0; file < BoardCfg::BOARD_DIMENSION; ++file) 
    {
        board[whitePawnRank][file] = ChessLogic::Piece::WHITE_PAWN;
        board[blackPawnRank][file] = ChessLogic::Piece::BLACK_PAWN;
    }

    // Add back rank white pieces using move semantics
    ChessLogic::Piece whiteBackPieces[] = 
    {
        ChessLogic::Piece::WHITE_ROOK, ChessLogic::Piece::WHITE_KNIGHT,
        ChessLogic::Piece::WHITE_BISHOP, ChessLogic::Piece::WHITE_QUEEN,
        ChessLogic::Piece::WHITE_KING, ChessLogic::Piece::WHITE_BISHOP,
        ChessLogic::Piece::WHITE_KNIGHT, ChessLogic::Piece::WHITE_ROOK
    };
    
    // Add back rank black pieces using move semantics
    ChessLogic::Piece blackBackPieces[] = 
    {
        ChessLogic::Piece::BLACK_ROOK, ChessLogic::Piece::BLACK_KNIGHT,
        ChessLogic::Piece::BLACK_BISHOP, ChessLogic::Piece::BLACK_QUEEN,
        ChessLogic::Piece::BLACK_KING, ChessLogic::Piece::BLACK_BISHOP,
        ChessLogic::Piece::BLACK_KNIGHT, ChessLogic::Piece::BLACK_ROOK
    };

    for (int file = 0; file < BoardCfg::BOARD_DIMENSION; ++file) 
    {
        board[whiteBackRank][file] = whiteBackPieces[file];
        board[blackBackRank][file] = blackBackPieces[file];
    }
}

ChessLogic::Piece ChessLogic::getPieceAt(const int rank, const int file) const 
{
    if (rank < BoardCfg::MIN_RANK || rank > BoardCfg::MAX_RANK || 
        file < BoardCfg::MIN_FILE || file > BoardCfg::MAX_FILE) 
    {
        return ChessLogic::Piece::EMPTY; // Out of bounds
    }
    return board[rank][file]; // Return the piece in the array
}

void ChessLogic::executeMove(
    const int srcRank, 
    const int srcFile,
    const int destRank, 
    const int destFile) 
{
    // Note: This method assumes the move has already been validated
    // All validation will be handled by ChessMoveValidator
    
    // Capture piece if present
    if (board[destRank][destFile] != ChessLogic::Piece::EMPTY) 
    {
        capturedPieces.push_back(std::move(board[destRank][destFile]));
    }

    // Execute the move using move semantics for efficiency
    board[destRank][destFile] = std::move(board[srcRank][srcFile]);
    board[srcRank][srcFile] = ChessLogic::Piece::EMPTY;
}

std::string ChessLogic::pieceToString(Piece piece) const 
{
    switch (piece) 
    {
        case Piece::EMPTY: return "empty";
        case Piece::WHITE_KING: return "wk";
        case Piece::WHITE_QUEEN: return "wq";
        case Piece::WHITE_ROOK: return "wr";
        case Piece::WHITE_BISHOP: return "wb";
        case Piece::WHITE_KNIGHT: return "wn";
        case Piece::WHITE_PAWN: return "wp";
        case Piece::BLACK_KING: return "bk";
        case Piece::BLACK_QUEEN: return "bq";
        case Piece::BLACK_ROOK: return "br";
        case Piece::BLACK_BISHOP: return "bb";
        case Piece::BLACK_KNIGHT: return "bn";
        case Piece::BLACK_PAWN: return "bp";
        default: return "unknown";
    }
}

// Utility methods for move validation support
bool ChessLogic::isSquareEmpty(const int rank, const int file) const 
{
    return getPieceAt(rank, file) == Piece::EMPTY;
}

bool ChessLogic::isWhitePiece(const int rank, const int file) const 
{
    Piece piece = getPieceAt(rank, file);
    return piece >= Piece::WHITE_KING && piece <= Piece::WHITE_PAWN;
}

bool ChessLogic::isBlackPiece(const int rank, const int file) const 
{
    Piece piece = getPieceAt(rank, file);
    return piece >= Piece::BLACK_KING && piece <= Piece::BLACK_PAWN;
}

bool ChessLogic::areSameColorPieces(const int srcRank, const int srcFile, const int destRank, const int destFile) const 
{
    return (isWhitePiece(srcRank, srcFile) && isWhitePiece(destRank, destFile)) ||
           (isBlackPiece(srcRank, srcFile) && isBlackPiece(destRank, destFile));
}

const std::vector<ChessLogic::Piece>& ChessLogic::getCapturedPieces() const 
{
    return capturedPieces;
}

bool ChessLogic::isValidSquare(const int rank, const int file) const 
{
    return rank >= BoardCfg::MIN_RANK && rank <= BoardCfg::MAX_RANK &&
           file >= BoardCfg::MIN_FILE && file <= BoardCfg::MAX_FILE;
}
