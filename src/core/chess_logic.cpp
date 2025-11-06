#include "chess_logic.h"
#include <cmath> // For abs() function

// Namespace alias for cleaner code while maintaining clarity
namespace BoardCfg = Config::Board;

ChessLogic::ChessLogic() : 
    currentPlayer(Player::WHITE_PLAYER), 
    halfMoveClock(0),
    board({{Piece::EMPTY}}) {
    // Initialize en passant state
    clearEnPassantState();
    
    // Initialize pieces array and board representation
    initializePieces();

    // Initialize the position occurrances
    positionOccurrances.insert({getCurrentPositionString(), 1});
}

ChessLogic::~ChessLogic() 
{}

void ChessLogic::initializePieces() 
{
    // Load color ranks
    int whitePawnRank = BoardCfg::WHITE_PAWN_START_RANK;
    int whiteBackRank = BoardCfg::WHITE_BACK_RANK;
    int blackPawnRank = BoardCfg::BLACK_PAWN_START_RANK;
    int blackBackRank = BoardCfg::BLACK_BACK_RANK;
    
    // Add pawns using move semantics for efficiency
    for (int file = 0; file < BoardCfg::BOARD_DIMENSION; ++file) {
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

    for (int file = 0; file < BoardCfg::BOARD_DIMENSION; ++file) {
        board[whiteBackRank][file] = whiteBackPieces[file];
        board[blackBackRank][file] = blackBackPieces[file];
    }
}

ChessLogic::Piece ChessLogic::getPieceAt(const int rank, const int file) const {
    // Check if rank and file are out of bounds. Default to EMPTY
    if (rank < BoardCfg::MIN_RANK || rank > BoardCfg::MAX_RANK || 
            file < BoardCfg::MIN_FILE || file > BoardCfg::MAX_FILE)
        return ChessLogic::Piece::EMPTY;
    
    return board[rank][file]; // Return the piece in the array
}

void ChessLogic::executeMove(
    const int srcRank, 
    const int srcFile,
    const int destRank, 
    const int destFile) {
    // Note: This method assumes the move has already been validated
    // All validation will be handled by ChessMoveValidator
    
    // Assume move without pawn or capture
    halfMoveClock++;

    // Update castling rights before moving (need to check if rook will be captured)
    updateCastlingRights(srcRank, srcFile, destRank, destFile);
    
    // Capture piece if present
    if (board[destRank][destFile] != ChessLogic::Piece::EMPTY) 
    {
        capturedPieces.push_back(std::move(board[destRank][destFile]));
        // Piece captured, reset halfmove clock
        halfMoveClock = 0;
    }

    // Execute the move using move semantics for efficiency
    board[destRank][destFile] = std::move(board[srcRank][srcFile]);
    board[srcRank][srcFile] = ChessLogic::Piece::EMPTY;
    
    // Update en passant state after the move
    updateEnPassantState(srcRank, srcFile, destRank, destFile);

    // Reset halfmoce clock if piece moved was a pawn
    if (isPawn(board[destRank][destFile])) 
        halfMoveClock = 0;

    // Update position occurances
    std::string currentPosition = getCurrentPositionString();
    // positionOccurrances is a std::map. If the currentPosition already exists,
    // the value is incremented. If the currentPosition does not exist,
    // positionOccurrances will automagically create a key of the currentPosition
    // with a value of 1
    positionOccurrances[currentPosition]++;
}

std::string ChessLogic::pieceToTextureString(const Piece piece) const {
    switch (piece) {
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

std::pair<int, int> ChessLogic::getKingPosition(const Player player) const {
    Piece kingPiece = 
        (player == Player::WHITE_PLAYER) ? 
        Piece::WHITE_KING : 
        Piece::BLACK_KING;

    for (int rank = 0; rank < BoardCfg::BOARD_DIMENSION; ++rank) {
        for (int file = 0; file < BoardCfg::BOARD_DIMENSION; ++file) {
            if (board[rank][file] == kingPiece) 
                return {rank, file};
        }
    }

    return {-1, -1}; // King not found (should not happen in a valid game)
}

// Utility methods for move validation support
bool ChessLogic::isSquareEmpty(const int rank, const int file) const {
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

bool ChessLogic::areSameColorPieces(
    const int srcRank, 
    const int srcFile, 
    const int destRank, 
    const int destFile) const {

    return (isWhitePiece(srcRank, srcFile) && isWhitePiece(destRank, destFile)) ||
           (isBlackPiece(srcRank, srcFile) && isBlackPiece(destRank, destFile));
}

const std::vector<ChessLogic::Piece>& ChessLogic::getCapturedPieces() const {
    return capturedPieces;
}

ChessLogic::Player ChessLogic::getPieceOwner(const ChessLogic::Piece piece) const {
    return (piece >= ChessLogic::Piece::BLACK_KING && piece <= ChessLogic::Piece::BLACK_PAWN) ?
        ChessLogic::Player::BLACK_PLAYER :
        ChessLogic::Player::WHITE_PLAYER;    
}

bool ChessLogic::isValidSquare(const int rank, const int file) const {
    return rank >= BoardCfg::MIN_RANK && rank <= BoardCfg::MAX_RANK &&
           file >= BoardCfg::MIN_FILE && file <= BoardCfg::MAX_FILE;
}

// Turn management methods
ChessLogic::Player ChessLogic::getCurrentPlayer() const {
    return currentPlayer;
}

bool ChessLogic::isPlayerTurn(const Player player) const {
    return currentPlayer == player;
}

void ChessLogic::switchTurn() {
    currentPlayer = 
        (currentPlayer == Player::WHITE_PLAYER) ? 
        Player::BLACK_PLAYER : 
        Player::WHITE_PLAYER;
}

bool ChessLogic::isPawn(const Piece piece) const {
    return piece == Piece::WHITE_PAWN || piece == Piece::BLACK_PAWN;
}

bool ChessLogic::isKing(const Piece piece) const {
    return piece == Piece::WHITE_KING || piece == Piece::BLACK_KING;
}

void ChessLogic::makeTemporaryMove(
    const int srcRank, 
    const int srcFile,
    const int destRank,
    const int destFile) {

    board[destRank][destFile] = board[srcRank][srcFile];
    board[srcRank][srcFile] = Piece::EMPTY;
}

void ChessLogic::undoTemporaryMove(
    const int srcRank,
    const int srcFile,
    const int destRank,
    const int destFile,
    const Piece capturedPiece) {

    board[srcRank][srcFile] = board[destRank][destFile];
    board[destRank][destFile] = capturedPiece;
}

bool ChessLogic::canCastleKingside(const Player player) const {
    return player == Player::WHITE_PLAYER ?
        !whiteKingMoved && !whiteKingsideRookMoved :
        !blackKingMoved && !blackKingsideRookMoved;
}

bool ChessLogic::canCastleQueenside(const Player player) const {
    return player == Player::WHITE_PLAYER ?
        !whiteKingMoved && !whiteQueensideRookMoved :
        !blackKingMoved && !blackQueensideRookMoved;
}

void ChessLogic::updateCastlingRights(
    const int srcRank,
    const int srcFile,
    const int destRank,
    const int destFile) {

    Piece pieceMoved = board[srcRank][srcFile];
    Piece pieceCaptured = board[destRank][destFile];

    // Update whether the king moved
    whiteKingMoved = whiteKingMoved || (pieceMoved == Piece::WHITE_KING);
    blackKingMoved = blackKingMoved || (pieceMoved == Piece::BLACK_KING);
    // Update whether a white rook moved    
    if (pieceMoved == Piece::WHITE_ROOK && srcRank == BoardCfg::WHITE_BACK_RANK) {
        whiteKingsideRookMoved = whiteKingsideRookMoved || srcFile == BoardCfg::KINGSIDE_ROOK_FILE;
        whiteQueensideRookMoved = whiteQueensideRookMoved || srcFile == BoardCfg::QUEENSIDE_ROOK_FILE;
    // Check if black rook moved
    } else if (pieceMoved == Piece::BLACK_ROOK && srcRank == BoardCfg::BLACK_BACK_RANK) {
        blackKingsideRookMoved = blackKingsideRookMoved || srcFile == BoardCfg::KINGSIDE_ROOK_FILE;
        blackQueensideRookMoved = blackQueensideRookMoved || srcFile == BoardCfg::QUEENSIDE_ROOK_FILE;
    }
    // Check if rook captured
    if (pieceCaptured == Piece::WHITE_ROOK && destRank == BoardCfg::WHITE_BACK_RANK) {
        whiteKingsideRookMoved = whiteKingsideRookMoved || destFile == BoardCfg::KINGSIDE_ROOK_FILE;
        whiteQueensideRookMoved = whiteQueensideRookMoved || destFile == BoardCfg::QUEENSIDE_ROOK_FILE;
    } else if (pieceCaptured == Piece::BLACK_ROOK && destRank == BoardCfg::BLACK_BACK_RANK) {
        blackKingsideRookMoved = blackKingsideRookMoved || destFile == BoardCfg::KINGSIDE_ROOK_FILE;
        blackQueensideRookMoved = blackQueensideRookMoved || destFile == BoardCfg::QUEENSIDE_ROOK_FILE;
    }
}

void ChessLogic::executeCastling(
    const int srcRank,
    const int srcFile,
    const int destRank,
    const int destFile) {
    // Note: This method assumes the castling has already been validated
    // All validation will be handled by ChessMoveValidator

    // No capture or pawn movement during castling. Iterate half move clock
    halfMoveClock++;
    
    // Move the king
    board[destRank][destFile] = board[srcRank][srcFile];
    board[srcRank][srcFile] = Piece::EMPTY;
    
    // Move the rook
    if (destFile == BoardCfg::KINGSIDE_CASTLE_KING_FILE) { // Kingside castling
        // Move rook from kingside to its new position
        board[destRank][BoardCfg::KINGSIDE_CASTLE_ROOK_FILE] = board[destRank][BoardCfg::KINGSIDE_ROOK_FILE];
        board[destRank][BoardCfg::KINGSIDE_ROOK_FILE] = Piece::EMPTY;
    } else if (destFile == BoardCfg::QUEENSIDE_CASTLE_KING_FILE) { // Queenside castling
        // Move rook from queenside to its new position
        board[destRank][BoardCfg::QUEENSIDE_CASTLE_ROOK_FILE] = board[destRank][BoardCfg::QUEENSIDE_ROOK_FILE];
        board[destRank][BoardCfg::QUEENSIDE_ROOK_FILE] = Piece::EMPTY;
    }
    
    // Update castling rights
    updateCastlingRights(srcRank, srcFile, destRank, destFile);

    // Update position occurance string
    std::string currentPosition = getCurrentPositionString();
    positionOccurrances[currentPosition]++;
}

// En passant query methods
bool ChessLogic::isEnPassantAvailable() const {
    return enPassantTargetRank != -1 && enPassantTargetFile != -1;
}

bool ChessLogic::isEnPassantTarget(const int rank, const int file) const {
    return enPassantTargetRank == rank && enPassantTargetFile == file;
}

std::pair<int, int> ChessLogic::getEnPassantTarget() const {
    return {enPassantTargetRank, enPassantTargetFile};
}

std::pair<int, int> ChessLogic::getEnPassantPawn() const {
    return {enPassantPawnRank, enPassantPawnFile};
}

// Execute en passant move
void ChessLogic::executeEnPassant(
    const int srcRank,
    const int srcFile, 
    const int destRank, 
    const int destFile) {
    // Pawn was moved, reset half move clock
    halfMoveClock = 0;

    // Move the capturing pawn
    board[destRank][destFile] = board[srcRank][srcFile];
    board[srcRank][srcFile] = Piece::EMPTY;
    
    // Capture the en passant pawn
    Piece capturedPawn = board[enPassantPawnRank][enPassantPawnFile];
    capturedPieces.push_back(capturedPawn);
    board[enPassantPawnRank][enPassantPawnFile] = Piece::EMPTY;
    
    // Clear en passant state
    clearEnPassantState();

    // Update position occurance string
    std::string currentPosition = getCurrentPositionString();
    positionOccurrances[currentPosition]++;
}

// Helper methods for en passant
void ChessLogic::updateEnPassantState(
    const int srcRank, 
    const int srcFile, 
    const int destRank, 
    const int destFile) {
    // The piece that just moved
    Piece piece = board[destRank][destFile]; 
    
    // Clear previous en passant state
    clearEnPassantState();
    
    // Check if a pawn made a double move
    if (isPawn(piece)) {
        int rankDiff = abs(destRank - srcRank);
        
        // If pawn moved 2 squares, set up en passant
        if (rankDiff == 2) {
            enPassantPawnRank = destRank;
            enPassantPawnFile = destFile;
            
            // The target square is between the starting and ending positions
            enPassantTargetRank = (srcRank + destRank) / 2;
            enPassantTargetFile = destFile;
        }
    }
}

void ChessLogic::clearEnPassantState() {
    enPassantTargetRank = -1;
    enPassantTargetFile = -1;
    enPassantPawnRank = -1;
    enPassantPawnFile = -1;
}

void ChessLogic::executePromotion(
    const int srcRank, 
    const int srcFile, 
    const int destRank, 
    const int destFile, 
    Piece promoteTo) {
    // Pawn was moved, reset half move clock
    halfMoveClock = 0;
    
    // Validate promoteTo piece
    if (promoteTo == Piece::EMPTY) {
        promoteTo = 
            (currentPlayer == Player::WHITE_PLAYER) ? 
            Piece::WHITE_QUEEN : 
            Piece::BLACK_QUEEN;
    }
    
    // Capture piece if present at destination
    if (board[destRank][destFile] != Piece::EMPTY) {
        updateCastlingRights(srcRank, srcFile, destRank, destFile);
        capturedPieces.push_back(std::move(board[destRank][destFile]));
    }

    // Move the pawn to the promotion square
    board[destRank][destFile] = board[srcRank][srcFile];
    board[srcRank][srcFile] = Piece::EMPTY;

    // Promote the pawn to the chosen piece
    board[destRank][destFile] = promoteTo;

    // Clear en passant state after promotion
    clearEnPassantState();

    // Update position occurance string
    std::string currentPosition = getCurrentPositionString();
    positionOccurrances[currentPosition]++;
}

int ChessLogic::getHalfmoveClock() const {
    return halfMoveClock;
}

bool ChessLogic::hasThreefoldRepetition() const {
    for (const std::pair<const std::string, int>& pair : positionOccurrances) {
        if (pair.second >= 3) 
            return true;
    }
    return false;
}

std::string ChessLogic::getCurrentPositionString() const {
    std::string positionString = "";
    
    // 1. Board positions to FEN string
    positionString += piecePositionToFENString();
    
    // 2. Add the current player turn
    positionString += 
        (currentPlayer == Player::WHITE_PLAYER) ?
        " w " :
        " b ";
        
    // 3. Add castling rights
    positionString += castlingRightsToFENString();
    positionString += " ";

    // 4. Add en passant target
    positionString += enPassantTargetToFENString();

    return positionString;
}

std::string ChessLogic::pieceToFENString(Piece piece) const{
    switch(piece) {
        case Piece::EMPTY: 
            return "-";
        case Piece::WHITE_KING: 
            return "K";
        case Piece::WHITE_QUEEN: 
            return "Q";
        case Piece::WHITE_ROOK: 
            return "R";
        case Piece::WHITE_BISHOP: 
            return "B";
        case Piece::WHITE_KNIGHT: 
            return "N";
        case Piece::WHITE_PAWN: 
            return "P";
        case Piece::BLACK_KING: 
            return "k";
        case Piece::BLACK_QUEEN: 
            return "q";
        case Piece::BLACK_ROOK: 
            return "r";
        case Piece::BLACK_BISHOP: 
            return "b";
        case Piece::BLACK_KNIGHT: 
            return "n";
        case Piece::BLACK_PAWN: 
            return "p";
        default: 
            return "ERROR";        
    }
}

std::string ChessLogic::piecePositionToFENString() const {
    std::string piecePositionString = "";   
    
    for (int rank = BoardCfg::MIN_RANK; rank <= BoardCfg::MAX_RANK; rank++) {
        int emptyCount = 0;
        for (int file = BoardCfg::MIN_FILE; file <= BoardCfg::MAX_FILE; file++) {
            Piece piece = getPieceAt(rank, file);
            if (piece == Piece::EMPTY) {
                emptyCount++;
                continue;
            }
            if (emptyCount > 0) {
                piecePositionString += std::to_string(emptyCount);
                emptyCount = 0;
            }
            piecePositionString += pieceToFENString(piece);
        }
        if (emptyCount > 0)
            piecePositionString += std::to_string(emptyCount);
        piecePositionString += "/";
    }

    // Drop the trailing /
    if (!piecePositionString.empty())
        piecePositionString.pop_back();

    return piecePositionString;
}

std::string ChessLogic::castlingRightsToFENString() const {
    std::string castlingRightsString = "";
    
    // Check castling rights in order
    if (canCastleKingside(Player::WHITE_PLAYER)) 
        castlingRightsString += "K";
    if (canCastleQueenside(Player::WHITE_PLAYER)) 
        castlingRightsString += "Q";
    if (canCastleKingside(Player::BLACK_PLAYER)) 
        castlingRightsString += "k";
    if (canCastleQueenside(Player::BLACK_PLAYER)) 
        castlingRightsString += "q";
    
    // If no castling rights, use "-"
    if (castlingRightsString.empty())
        castlingRightsString = "-";
    
    return castlingRightsString;
}

std::string ChessLogic::enPassantTargetToFENString() const {
    std::string enPassantString = "-"; // Assume no target
    if (enPassantTargetFile != -1 && enPassantTargetRank != -1) {
        char fileChar = 'a' + enPassantTargetFile;
        char rankChar = '1' + enPassantTargetRank;
        enPassantString = std::string(1, fileChar) + std::string(1, rankChar);
    }
    return enPassantString;
}
