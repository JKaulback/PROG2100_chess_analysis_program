#pragma once

#include <string>
#include <vector>
#include <regex>

/**
 * Structure representing a parsed analysis line from the UCI engine
 */
struct AnalysisLine {
    int multipv = 1;            // Principal variation number (1-4)
    std::string text;           // Raw info line (trimmed) for this PV
};

/**
 * Handles parsing of UCI engine output into structured data
 */
class UCIAnalysisParser {
public:
    UCIAnalysisParser() = default;
    ~UCIAnalysisParser() = default;
    
    /**
     * Parse a single line of engine output
     * @param line The raw line from the engine
     * @return AnalysisLine structure with parsed data
     */
    static AnalysisLine parseAnalysisLine(const std::string& line);
    
    /**
     * Check if a line is an info line (starts with "info")
     * @param line The line to check
     * @return true if it's an info line, false otherwise
     */
    static bool isInfoLine(const std::string& line);
    
    /**
     * Check if a line should be ignored (contains "currmove")
     * @param line The line to check
     * @return true if the line should be ignored, false otherwise
     */
    static bool shouldIgnoreLine(const std::string& line);
    
    /**
     * Validate if a FEN string has the correct number of parts
     * @param fen The FEN string to validate
     * @return true if valid, false otherwise
     */
    static bool isValidFEN(const std::string& fen);
    
    /**
     * Trim whitespace from a string
     * @param str The string to trim
     * @return Trimmed string
     */
    static std::string trim(const std::string& str);
    
private:
    static const std::regex multipvRegex_;

    static std::string parseCpOrMate(const std::string& line);
    static std::string parsePv(const std::string& line);
};