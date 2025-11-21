#include <cctype>
#include "chess_board.h"

namespace BoardCfg = Config::Board;

ChessBoard::ChessBoard() {
    boardInit();
}

// Return char representation of piece on board (default to empty)
char ChessBoard::getPieceAt(const int rank, const int file) const {
    if (isValidBoardPosition(rank, file))
        return board[rank][file];
    return BoardCfg::EMPTY;
}

// Return char representation of the owner of a piece (empty if invalid)
char ChessBoard::getPieceOwner(const int rank, const int file) const {
    if (isValidBoardPosition(rank, file) && 
            getPieceAt(rank, file) != BoardCfg::EMPTY)
        return 
            (isupper(board[rank][file])) ?
            'w' :
            'b';
    return BoardCfg::EMPTY;
}

char ChessBoard::getPieceOwner(const char piece) const {
    return 
        (isupper(piece)) ?
        'w' :
        'b';
}

// Return board location of king position for specified player
std::pair<int, int> ChessBoard::getKingPosition(const char player) const {
    // Determine the king piece for the player
    char kingPiece =
        (player == 'w') ?
        'K' :
        'k';
    
    // Find the king for the player
    for (int rank = BoardCfg::MIN_RANK; rank <= BoardCfg::MAX_RANK; rank++) {
        for (int file = BoardCfg::MIN_FILE; file <= BoardCfg::MAX_FILE; file++) {
            if (board[rank][file] == kingPiece)
                return {rank, file};
        }
    }

    return {-1, -1}; // Shouldn't happen in a valid game
}

// Return a vector of all captured pieces
std::vector<char> ChessBoard::getCapturedPieces() const {
    return capturedPieces;
}

bool ChessBoard::isValidSquare(const int rank, const int file) const {
    return
        (rank >= BoardCfg::MIN_RANK && rank <= BoardCfg::MAX_RANK &&
        file >= BoardCfg::MIN_FILE && file <= BoardCfg::MAX_FILE);
}

// Sets a valid piece in a valid board location
void ChessBoard::setPieceAt(const int rank, const int file, const char piece) {
    if (isValidBoardPosition(rank, file) && isValidPiece(piece))
       board[rank][file] = piece; 
}

// Executes a basic move (with capture if applicable)
void ChessBoard::executeBasicMove(const ChessMove& move) {
    // Load movement values
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();

    // Check for capture
    if (board[destRank][destFile] != BoardCfg::EMPTY)
        capturedPieces.push_back(board[destRank][destFile]);

    // Execute move
    board[destRank][destFile] = board[srcRank][srcFile];
    board[srcRank][srcFile] = BoardCfg::EMPTY;
    
}

// Executes a castle
void ChessBoard::executeCastling(const ChessMove& move) {
    // Load movement values
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();

    // Move the king
    board[destRank][destFile] = board[srcRank][srcFile];
    board[srcRank][srcFile] = BoardCfg::EMPTY;

    // Move the rook
    if (destFile == BoardCfg::KINGSIDE_CASTLE_KING_FILE) { // Kingside castling
        // Move kingside rook to new position
        board[destRank][BoardCfg::KINGSIDE_CASTLE_ROOK_FILE] = 
            board[destRank][BoardCfg::KINGSIDE_ROOK_FILE];
        board[destRank][BoardCfg::KINGSIDE_ROOK_FILE] = BoardCfg::EMPTY;
    } else {
        // Move queenside rook to new position
        board[destRank][BoardCfg::QUEENSIDE_CASTLE_ROOK_FILE] =
            board[destRank][BoardCfg::QUEENSIDE_ROOK_FILE];
        board[destRank][BoardCfg::QUEENSIDE_ROOK_FILE] = BoardCfg::EMPTY;
    }
}

// Executes en passant
void ChessBoard::executeEnPassant(const ChessMove& move) {
    // Load movement values
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();

    // Move the capturing pawn
    board[destRank][destFile] = board[srcRank][srcFile];
    board[srcRank][srcFile] = BoardCfg::EMPTY;

    // Determine the rank of the captured pawn
    int enPassantCaptureRank =
        (getPieceOwner(destRank, destFile) == 'w') ?
        BoardCfg::WHITE_EN_PASSANT_CAPTURE_RANK :
        BoardCfg::BLACK_EN_PASSANT_CAPTURE_RANK;

    // Capture the en passant pawn
    capturedPieces.push_back(board[enPassantCaptureRank][destFile]);
    board[enPassantCaptureRank][destFile] = BoardCfg::EMPTY;
}

// Executes promotion (default to queen)
void ChessBoard::executePromotion(const ChessMove& move, const char promoteTo) {
    // Load movement values
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();

    // Check for capture
    if (board[destRank][destFile] != BoardCfg::EMPTY)
        capturedPieces.push_back(board[destRank][destFile]);

    // Move promoted piece to promotion square
    board[destRank][destFile] = promoteTo;
    board[srcRank][srcFile] = BoardCfg::EMPTY;
}

void ChessBoard::makeTemporaryMove(const ChessMove& move) {
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();

    board[destRank][destFile] = board[srcRank][srcFile];
    board[srcRank][srcFile] = BoardCfg::EMPTY;
}

void ChessBoard::undoTemporaryMove(const ChessMove& move, const char capturedPiece) {
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();

    board[srcRank][srcFile] = board[destRank][destFile];
    board[destRank][destFile] = capturedPiece;
}

bool ChessBoard::isSquareEmpty(const int rank, const int file) const {
    if (isValidBoardPosition(rank, file))
        return (board[rank][file] == BoardCfg::EMPTY);
    return false;
}

bool ChessBoard::isWhitePiece(const int rank, const int file) const {
    if (isValidBoardPosition(rank, file))
        return isupper(board[rank][file]);
    return false;
}

bool ChessBoard::areSameColorPieces(const ChessMove& move) const {
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();

    if (isValidBoardPosition(srcRank, srcFile) && isValidBoardPosition(destRank, destFile))
        return (getPieceOwner(srcRank, srcFile) == getPieceOwner(destRank, destFile));
    return false;
}

bool ChessBoard::isPawn(const char piece) const {
    return (piece == 'P' || piece == 'p');
}

bool ChessBoard::isKing(const char piece) const {
    return (piece == 'K' || piece == 'k');
}

std::string ChessBoard::getPieceTextureString(const char piece) const {
    std::string textureString = "";
    
    // Determine piece owner
    if (isupper(piece))
        textureString += "w";
    else
        textureString += "b";
    
    // Standardize piece type
    textureString += tolower(piece);
    
    return textureString;
}

void ChessBoard::boardInit() {
    clearBoard();

    // Load color ranks
    int whitePawnRank = BoardCfg::WHITE_PAWN_START_RANK;
    int whiteBackRank = BoardCfg::WHITE_BACK_RANK;
    int blackPawnRank = BoardCfg::BLACK_PAWN_START_RANK;
    int blackBackRank = BoardCfg::BLACK_BACK_RANK;

    // Add back rank white pieces
    char whiteBackPieces[] = {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};

    // Add back rank black pieces
    char blackBackPieces[] = {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'};

    // Populate the board
    for (int file = BoardCfg::MIN_FILE; file <= BoardCfg::MAX_FILE; file++) {
        board[whitePawnRank][file] = 'P';
        board[blackPawnRank][file] = 'p';
        board[whiteBackRank][file] = whiteBackPieces[file];
        board[blackBackRank][file] = blackBackPieces[file];
    }
}

void ChessBoard::clearBoard() {
    for (int rank = BoardCfg::MIN_RANK; rank <= BoardCfg::MAX_RANK; rank++) {
        for (int file = BoardCfg::MIN_FILE; file <= BoardCfg::MAX_FILE; file++) {
            board[rank][file] = BoardCfg::EMPTY;
        }
    }
    capturedPieces.clear();
}

void ChessBoard::resetToStartingPosition() {
    boardInit();
}

bool ChessBoard::isValidBoardPosition(const int rank, const int file) const {
    return (rank >= BoardCfg::MIN_RANK && rank <= BoardCfg::MAX_RANK &&
            file >= BoardCfg::MIN_FILE && file <= BoardCfg::MAX_FILE);
}

bool ChessBoard::isValidPiece(const char piece) const {
    if (piece == BoardCfg::EMPTY)
        return true;
    for (int i = 0; i < BoardCfg::VALID_PIECES.size(); i++) {
        if (BoardCfg::VALID_PIECES.at(i) == piece)
            return true;
    }
    return false;
}
