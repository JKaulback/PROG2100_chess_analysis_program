#include "uci_analysis_parser.h"
#include <sstream>
#include <algorithm>

// Static member initialization
const std::regex UCIAnalysisParser::multipvRegex_(R"(multipv\s+(\d+))");

AnalysisLine UCIAnalysisParser::parseAnalysisLine(const std::string& line) {
    AnalysisLine analysisLine;
    std::smatch match;
    
    // Extract multipv value (default to 1 if not found)
    if (std::regex_search(line, match, multipvRegex_))
        analysisLine.multipv = std::stoi(match[1]);
    else
        analysisLine.multipv = 1;
    
    // Store the trimmed line
    analysisLine.text = trim(line);
    
    return analysisLine;
}

bool UCIAnalysisParser::isInfoLine(const std::string& line) {
    return line.rfind("info", 0) == 0; // Check if line starts with "info"
}

bool UCIAnalysisParser::shouldIgnoreLine(const std::string& line) {
    return line.find("currmove") != std::string::npos;
}

bool UCIAnalysisParser::isValidFEN(const std::string& fen) {
    if (fen.empty())
        return false;
    
    std::istringstream stream(fen);
    std::string part;
    int partCount = 0;
    
    while (std::getline(stream, part, ' '))
        partCount++;
    
    // A valid FEN should have exactly 6 parts
    return partCount == 6;
}

std::string UCIAnalysisParser::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos)
        return "";
    
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}