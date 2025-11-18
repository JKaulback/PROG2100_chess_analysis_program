#include "fen_loader.h"

bool FENLoader::loadFromFile(const std::string& filename, ChessAnalysisProgram& controller) {
    std::ifstream file(filename);
    
    // Check if file exists and can be opened
    if (!file.is_open())
        return false; // File doesn't exist or can't be opened
    
    std::string fenString;
    if (!std::getline(file, fenString)) {
        file.close();
        return false; // File is empty or can't be read
    }
    
    file.close();
    
    // Try to apply the FEN string
    return applyFEN(fenString, controller);
}

bool FENLoader::applyFEN(const std::string& fenString, ChessAnalysisProgram& controller) {
    // Split FEN string into its components
    std::vector<std::string> parts = splitString(fenString, ' ');
    
    // At minimum, we need the board position (first component)
    if (parts.empty())
        return false; // No data at all

    // Parse and apply board position - this is mandatory
    if (!parseBoardPosition(parts[0], controller))
        return false; // Invalid board position - fail completely
    
    // If we have additional parts, try to parse game state
    // But don't fail the entire load if game state parsing fails
    if (parts.size() >= 6) {
        // Try to parse full game state, but don't fail if it's invalid
        parseGameState(parts[1], parts[2], parts[3], parts[4], parts[5], controller);
    }
    // If we have fewer than 6 parts, just use default game state
    
    return true; // Successfully loaded at least the board position
}

bool FENLoader::parseBoardPosition(const std::string& piecePositions, ChessAnalysisProgram& controller) {
    // Clear the current board first
    controller.clearBoard();
    
    // Split board into ranks (rows)
    std::vector<std::string> ranks = splitString(piecePositions, '/');
    
    if (ranks.size() != 8)
        return false; // Must have exactly 8 ranks
    
    // Process each rank (FEN rank 8 = board rank 7, FEN rank 1 = board rank 0)
    for (int fenRank = 0; fenRank < 8; fenRank++) {
        int boardRank = 7 - fenRank; // Convert FEN rank to board rank
        int file = 0;
        
        for (char c : ranks[fenRank]) {
            if (isDigit(c)) {
                // Number represents empty squares
                int emptySquares = charToInt(c);
                if (emptySquares < 1 || emptySquares > 8 || file + emptySquares > 8) 
                    return false; // Invalid number of empty squares
                file += emptySquares; // Skip empty squares
            } else if (isValidFENChar(c)) {
                // Valid piece character
                if (file >= 8)
                    return false; // Too many pieces in rank
                
                controller.setPieceAt(boardRank, file, c);
                
                file++;
            } else 
                return false; // Invalid character
        }
        
        if (file != 8) 
            return false; // Rank doesn't have exactly 8 squares
    }
    
    return true;
}

bool FENLoader::parseGameState(const std::string& activeColor, const std::string& castlingRights,
                              const std::string& enPassant, const std::string& halfmove,
                              const std::string& fullmove, ChessAnalysisProgram& controller) {
    bool success = true;
    
    // Try to set current player
    if (activeColor == "w" || activeColor == "b")
        controller.setCurrentPlayer(activeColor[0]);
    else
        success = false; // Invalid, but continue trying other parts
    
    // Try to set castling rights
    bool validCastling = true;
    for (char c : castlingRights) {
        if (c != 'K' && c != 'Q' && c != 'k' && c != 'q' && c != '-') {
            validCastling = false;
            break;
        }
    }
    if (validCastling) {
        bool whiteKingside = castlingRights.find('K') != std::string::npos;
        bool whiteQueenside = castlingRights.find('Q') != std::string::npos;
        bool blackKingside = castlingRights.find('k') != std::string::npos;
        bool blackQueenside = castlingRights.find('q') != std::string::npos;
        controller.setCastlingRights(whiteKingside, whiteQueenside, blackKingside, blackQueenside);
    } else
        success = false;
    
    // Try to set en passant target
    if (enPassant == "-")
        controller.clearEnPassantTarget();
    else if (enPassant.length() == 2) {
        char file = enPassant[0];
        char rank = enPassant[1];
        if (file >= 'a' && file <= 'h' && rank >= '1' && rank <= '8') {
            int fileNum = file - 'a'; // Convert 'a'-'h' to 0-7
            int rankNum = rank - '1'; // Convert '1'-'8' to 0-7
            controller.setEnPassantTarget(rankNum, fileNum);
        } else
            success = false;
    } else
        success = false;
    
    // Try to set halfmove clock
    try {
        int halfmoveCount = std::stoi(halfmove);
        if (halfmoveCount >= 0) 
            controller.setHalfmoveClock(halfmoveCount);
        else 
            success = false;
    } catch (const std::exception&) {
        success = false;
    }
    
    // Try to set fullmove number
    try {
        int fullmoveCount = std::stoi(fullmove);
        if (fullmoveCount >= 1)
            controller.setFullmoveClock(fullmoveCount);
        else 
            success = false;
    } catch (const std::exception&) { 
        success = false;
    }
    
    return success; // Returns true only if ALL parts were valid
}

bool FENLoader::isValidFENChar(char c) {
    return c == 'P' || c == 'R' || c == 'N' || c == 'B' || c == 'Q' || c == 'K' ||
           c == 'p' || c == 'r' || c == 'n' || c == 'b' || c == 'q' || c == 'k';
}

bool FENLoader::isDigit(char c) {
    return c >= '1' && c <= '8';
}

int FENLoader::charToInt(char c) {
    return c - '0';
}

std::vector<std::string> FENLoader::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}