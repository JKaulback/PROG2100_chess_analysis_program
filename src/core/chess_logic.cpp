#include "chess_logic.h"
#include <cmath> // For abs() function

// Namespace alias for cleaner code while maintaining clarity
namespace BoardCfg = Config::Board;

ChessLogic::ChessLogic() : currentPlayer(Player::WHITE_PLAYER)
{
    // Initialize en passant state
    clearEnPassantState();
    
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
    int whitePawnRank = BoardCfg::WHITE_PAWN_START_RANK;
    int whiteBackRank = BoardCfg::WHITE_BACK_RANK;
    int blackPawnRank = BoardCfg::BLACK_PAWN_START_RANK;
    int blackBackRank = BoardCfg::BLACK_BACK_RANK;
    
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
    
    // Update castling rights before moving
    updateCastlingRights(srcRank, srcFile);
    
    // Capture piece if present
    if (board[destRank][destFile] != ChessLogic::Piece::EMPTY) 
    {
        capturedPieces.push_back(std::move(board[destRank][destFile]));
    }

    // Execute the move using move semantics for efficiency
    board[destRank][destFile] = std::move(board[srcRank][srcFile]);
    board[srcRank][srcFile] = ChessLogic::Piece::EMPTY;
    
    // Update en passant state after the move
    updateEnPassantState(srcRank, srcFile, destRank, destFile);
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

std::pair<int, int> ChessLogic::getKingPosition(Player player) const 
{
    Piece kingPiece = (player == Player::WHITE_PLAYER) ? Piece::WHITE_KING : Piece::BLACK_KING;

    for (int rank = 0; rank < BoardCfg::BOARD_DIMENSION; ++rank) 
    {
        for (int file = 0; file < BoardCfg::BOARD_DIMENSION; ++file) 
        {
            if (board[rank][file] == kingPiece) 
            {
                return {rank, file};
            }
        }
    }

    return {-1, -1}; // King not found (should not happen in a valid game)
}

// Utility methods for move validation support
bool ChessLogic::isSquareEmpty(const int rank, const int file) const 
{
    return getPieceAt(rank, file) == Piece::EMPTY;
}

bool ChessLogic::isWhitePiece(const int rank, const int file) const {
    Piece piece = getPieceAt(rank, file);
    return piece >= Piece::WHITE_KING && piece <= Piece::WHITE_PAWN;
}

bool ChessLogic::isBlackPiece(const int rank, const int file) const {
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

// Turn management methods
ChessLogic::Player ChessLogic::getCurrentPlayer() const 
{
    return currentPlayer;
}

bool ChessLogic::isPlayerTurn(Player player) const 
{
    return currentPlayer == player;
}

void ChessLogic::switchTurn() 
{
    currentPlayer = (currentPlayer == Player::WHITE_PLAYER) ? Player::BLACK_PLAYER : Player::WHITE_PLAYER;
    // Note: En passant state is cleared in updateEnPassantState() after each move
    // This ensures the state is available for the current turn but cleared for the next
}

ChessLogic::Player ChessLogic::getPieceOwner(Piece piece) const {
    if (piece >= Piece::WHITE_KING && piece <= Piece::WHITE_PAWN) {
        return Player::WHITE_PLAYER;
    } else if (piece >= Piece::BLACK_KING && piece <= Piece::BLACK_PAWN) {
        return Player::BLACK_PLAYER;
    }
    return Player::WHITE_PLAYER; // Default
}

bool ChessLogic::isPawn(Piece piece) const {
    return piece == Piece::WHITE_PAWN || piece == Piece::BLACK_PAWN;
}

bool ChessLogic::isKing(Piece piece) const {
    return piece == Piece::WHITE_KING || piece == Piece::BLACK_KING;
}

void ChessLogic::makeTemporaryMove(int fromRank, int fromFile, int toRank, int toFile) {
    board[toRank][toFile] = board[fromRank][fromFile];
    board[fromRank][fromFile] = Piece::EMPTY;
}

void ChessLogic::undoTemporaryMove(int fromRank, int fromFile, int toRank, int toFile, Piece capturedPiece) {
    board[fromRank][fromFile] = board[toRank][toFile];
    board[toRank][toFile] = capturedPiece;
}

bool ChessLogic::canCastleKingside(Player player) const {
    if (player == Player::WHITE_PLAYER) {
        return !whiteKingMoved && !whiteKingsideRookMoved;
    } else {
        return !blackKingMoved && !blackKingsideRookMoved;
    }
}

bool ChessLogic::canCastleQueenside(Player player) const {
    if (player == Player::WHITE_PLAYER) {
        return !whiteKingMoved && !whiteQueensideRookMoved;
    } else {
        return !blackKingMoved && !blackQueensideRookMoved;
    }
}

void ChessLogic::updateCastlingRights(int fromRank, int fromFile) {
    Piece piece = board[fromRank][fromFile];
    
    // Check if king moved
    if (piece == Piece::WHITE_KING) {
        whiteKingMoved = true;
    } else if (piece == Piece::BLACK_KING) {
        blackKingMoved = true;
    }
    // Check if rook moved
    else if (piece == Piece::WHITE_ROOK) {
        if (fromRank == BoardCfg::WHITE_BACK_RANK && fromFile == BoardCfg::QUEENSIDE_ROOK_FILE) {
            whiteQueensideRookMoved = true;
        } else if (fromRank == BoardCfg::WHITE_BACK_RANK && fromFile == BoardCfg::KINGSIDE_ROOK_FILE) {
            whiteKingsideRookMoved = true;
        }
    } else if (piece == Piece::BLACK_ROOK) {
        if (fromRank == BoardCfg::BLACK_BACK_RANK && fromFile == BoardCfg::QUEENSIDE_ROOK_FILE) {
            blackQueensideRookMoved = true;
        } else if (fromRank == BoardCfg::BLACK_BACK_RANK && fromFile == BoardCfg::KINGSIDE_ROOK_FILE) {
            blackKingsideRookMoved = true;
        }
    }
}

void ChessLogic::executeCastling(int fromRank, int fromFile, int toRank, int toFile) {
    // Move the king
    board[toRank][toFile] = board[fromRank][fromFile];
    board[fromRank][fromFile] = Piece::EMPTY;
    
    // Move the rook
    if (toFile == BoardCfg::KINGSIDE_CASTLE_KING_FILE) { // Kingside castling
        // Move rook from kingside to its new position
        board[toRank][BoardCfg::KINGSIDE_CASTLE_ROOK_FILE] = board[toRank][BoardCfg::KINGSIDE_ROOK_FILE];
        board[toRank][BoardCfg::KINGSIDE_ROOK_FILE] = Piece::EMPTY;
    } else if (toFile == BoardCfg::QUEENSIDE_CASTLE_KING_FILE) { // Queenside castling
        // Move rook from queenside to its new position
        board[toRank][BoardCfg::QUEENSIDE_CASTLE_ROOK_FILE] = board[toRank][BoardCfg::QUEENSIDE_ROOK_FILE];
        board[toRank][BoardCfg::QUEENSIDE_ROOK_FILE] = Piece::EMPTY;
    }
    
    // Update castling rights
    updateCastlingRights(fromRank, fromFile);
}

// En passant query methods
bool ChessLogic::isEnPassantAvailable() const {
    return enPassantTargetRank != -1 && enPassantTargetFile != -1;
}

bool ChessLogic::isEnPassantTarget(int rank, int file) const {
    return enPassantTargetRank == rank && enPassantTargetFile == file;
}

std::pair<int, int> ChessLogic::getEnPassantTarget() const {
    return {enPassantTargetRank, enPassantTargetFile};
}

std::pair<int, int> ChessLogic::getEnPassantPawn() const {
    return {enPassantPawnRank, enPassantPawnFile};
}

// Execute en passant move
void ChessLogic::executeEnPassant(int fromRank, int fromFile, int toRank, int toFile) {
    // Move the capturing pawn
    board[toRank][toFile] = board[fromRank][fromFile];
    board[fromRank][fromFile] = Piece::EMPTY;
    
    // Capture the en passant pawn
    Piece capturedPawn = board[enPassantPawnRank][enPassantPawnFile];
    capturedPieces.push_back(capturedPawn);
    board[enPassantPawnRank][enPassantPawnFile] = Piece::EMPTY;
    
    // Clear en passant state
    clearEnPassantState();
}

// Helper methods for en passant
void ChessLogic::updateEnPassantState(int fromRank, int fromFile, int toRank, int toFile) {
    Piece piece = board[toRank][toFile]; // The piece that just moved
    
    // Clear previous en passant state
    clearEnPassantState();
    
    // Check if a pawn made a double move
    if (isPawn(piece)) {
        int rankDiff = abs(toRank - fromRank);
        
        // If pawn moved 2 squares, set up en passant
        if (rankDiff == 2) {
            enPassantPawnRank = toRank;
            enPassantPawnFile = toFile;
            
            // The target square is between the starting and ending positions
            enPassantTargetRank = (fromRank + toRank) / 2;
            enPassantTargetFile = toFile;
        }
    }
}

void ChessLogic::clearEnPassantState() {
    enPassantTargetRank = -1;
    enPassantTargetFile = -1;
    enPassantPawnRank = -1;
    enPassantPawnFile = -1;
}

void ChessLogic::executePromotion(int fromRank, int fromFile, int toRank, int toFile, Piece promoteTo) {
    // Validate promoteTo piece
    if (promoteTo == Piece::EMPTY) {
        promoteTo = (currentPlayer == Player::WHITE_PLAYER) ? 
            Piece::WHITE_QUEEN : 
            Piece::BLACK_QUEEN;
    }
    
    // Capture piece if present at destination
    if (board[toRank][toFile] != Piece::EMPTY) 
    {
        capturedPieces.push_back(std::move(board[toRank][toFile]));
    }

    // Move the pawn to the promotion square
    board[toRank][toFile] = board[fromRank][fromFile];
    board[fromRank][fromFile] = Piece::EMPTY;

    // Promote the pawn to the chosen piece
    board[toRank][toFile] = promoteTo;

    // Clear en passant state after promotion
    clearEnPassantState();
}
