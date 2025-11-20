#include "chess_move.h"

std::string ChessMove::toAlgebraicNotation() const {
    
    char srcFileChar = 'a' + srcFile;
    char srcRankChar = '1' + srcRank;
    char destFileChar = 'a' + destFile;
    char destRankChar = '1' + destRank;

    return 
        std::string(1, srcFileChar) +
        std::string(1, srcRankChar) +
        std::string(1, destFileChar) +
        std::string(1, destRankChar);
}