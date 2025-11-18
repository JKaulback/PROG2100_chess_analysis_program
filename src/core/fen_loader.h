#pragma once

#include <string>
#include <fstream>
#include "../application/chess_analysis_program.h"

class FENLoader {
public:
    // Load FEN from file and apply to the chess program
    static bool loadFromFile(const std::string& filename, ChessAnalysisProgram& controller);
    
    // Parse and apply FEN string to the chess program  
    static bool applyFEN(const std::string& fenString, ChessAnalysisProgram& controller);

private:
    // Helper methods for parsing FEN components
    static bool parseBoardPosition(const std::string& piecePositions, ChessAnalysisProgram& controller);
    static bool parseGameState(const std::string& activeColor, const std::string& castlingRights,
                              const std::string& enPassant, const std::string& halfmove,
                              const std::string& fullmove, ChessAnalysisProgram& controller);

    // Utility methods
    static bool isValidFENChar(char c);
    static bool isDigit(char c);
    static int charToInt(char c);
    static std::vector<std::string> splitString(const std::string& str, char delimiter);
};