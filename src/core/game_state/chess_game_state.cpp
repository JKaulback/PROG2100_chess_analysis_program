#include "chess_game_state.h"

ChessGameState::ChessGameState(const ChessBoard& board) : board(&board) {
    currentPlayer = 'w'; // White goes first
    fullmoveClock = 0;
    
    resetHalfmoveClock();
    resetCastlingRights();
    clearEnPassantState();
}

void ChessGameState::makeMove(const ChessMove& move) {
    fullmoveClock++;
    halfmoveClock++; // Assume no pawn movement or capture
    
    char srcPiece = board->getPieceAt(move.getSrcRank(), move.getSrcFile());
    char destPiece = board->getPieceAt(move.getDestRank(), move.getDestFile());

    // Check for halfmove clock reset
    if (board->isPawn(srcPiece) || destPiece != BoardCfg::EMPTY)
        resetHalfmoveClock();
    
    // Update special move states
    updateCastlingRights(move, srcPiece, destPiece);
    updateEnPassantState(move, srcPiece);

    // Switch turns
    currentPlayer =
        (currentPlayer == 'w') ?
        'b' :
        'w';
}

char ChessGameState::getCurrentPlayer() const {
    return currentPlayer;
}

bool ChessGameState::isPlayerTurn(const char player) const {
    return currentPlayer == player;
}

bool ChessGameState::canCastleKingside(const char player) const {
    if (player == 'w')
        return (!whiteKingMoved && !whiteKRookMoved);
    return (!blackKingMoved && !blackKRookMoved);
}

bool ChessGameState::canCastleQueenside(const char player) const {
    if (player == 'w')
        return (!whiteKingMoved && !whiteQRookMoved);
    return (!blackKingMoved && !blackQRookMoved);
}

bool ChessGameState::isEnPassantAvailable() const {
    if (enPassantTargetRank != -1 && enPassantTargetFile != -1)
        return true;
    return false;
}

bool ChessGameState::isEnPassantTarget(const int rank, const int file) const {
    if (rank == enPassantTargetRank && file == enPassantTargetFile)
        return true;
    return false;
}

std::pair<int, int> ChessGameState::getEnPassantTarget() const {
    return {enPassantTargetRank, enPassantTargetFile};
}

std::pair<int, int> ChessGameState::getEnPassantPawn() const {
    return {enPassantPawnRank, enPassantPawnFile};
}

int ChessGameState::getHalfmoveClock() const {
    return halfmoveClock;
}

int ChessGameState::getFullmoveClock() const {
    return fullmoveClock;
}

bool ChessGameState::hasThreefoldRepetition() const {
    return false; //TODO: Implement threefold repetition tracking in ChessHistoryTracker class
}

void ChessGameState::updateCastlingRights(const ChessMove& move, const char srcPiece, const char destPiece) {
    // Load movement info
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();

    // Update king movement
    whiteKingMoved = whiteKingMoved || (srcPiece == 'K');
    blackKingMoved = blackKingMoved || (srcPiece == 'k');

    // Check for rook movement 
    if (srcPiece == 'R' && srcRank == BoardCfg::WHITE_BACK_RANK) {
        whiteKRookMoved = whiteKRookMoved || srcFile == BoardCfg::KINGSIDE_ROOK_FILE;
        whiteQRookMoved = whiteQRookMoved || srcFile == BoardCfg::QUEENSIDE_ROOK_FILE;
    } else if (srcPiece == 'r' && srcRank == BoardCfg::BLACK_BACK_RANK) {
        blackKRookMoved = blackKRookMoved || srcFile == BoardCfg::KINGSIDE_ROOK_FILE;
        blackQRookMoved = blackQRookMoved || srcFile == BoardCfg::QUEENSIDE_ROOK_FILE;
    }
    
    // Check for rook capture
    if (destPiece == 'R' && destRank == BoardCfg::WHITE_BACK_RANK) {
        whiteKRookMoved = whiteKRookMoved || destFile == BoardCfg::KINGSIDE_ROOK_FILE;
        whiteQRookMoved = whiteQRookMoved || destFile == BoardCfg::QUEENSIDE_ROOK_FILE;
    } else if (destPiece == 'r' && destRank == BoardCfg::BLACK_BACK_RANK) {
        blackKRookMoved = blackKRookMoved || destFile == BoardCfg::KINGSIDE_ROOK_FILE;
        blackQRookMoved = blackQRookMoved || destFile == BoardCfg::QUEENSIDE_ROOK_FILE;
    }
}

void ChessGameState::updateEnPassantState(const ChessMove& move, const char srcPiece) {
    // Load movement info
    int srcRank = move.getSrcRank();
    int srcFile = move.getSrcFile();
    int destRank = move.getDestRank();
    int destFile = move.getDestFile();

    // Clear the previous en passant state
    clearEnPassantState();

    // Check if a pawn made a double move
    if (srcPiece == 'P' || srcPiece == 'p') {
        int rankDiff = abs(destRank - srcRank);

        enPassantPawnRank = destRank;
        enPassantPawnFile = destFile;

        // If pawn moved 2 squares, set up en passant
        if (rankDiff == 2) {
            enPassantTargetRank = (srcRank + destRank) / 2;
            enPassantTargetFile = destFile;
        }
    }
}

void ChessGameState::resetHalfmoveClock() {
    halfmoveClock = 0;
}

void ChessGameState::resetCastlingRights() {
    whiteKingMoved = false, blackKingMoved = false;
    whiteKRookMoved = false, whiteQRookMoved = false;
    blackKRookMoved = false, blackQRookMoved = false;
}

void ChessGameState::clearEnPassantState() {
    enPassantTargetRank = -1;
    enPassantTargetFile = -1;
    enPassantPawnRank = -1;
    enPassantPawnFile = -1;
}