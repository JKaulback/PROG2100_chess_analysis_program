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

    analysisLine.text = parseCpOrMate(line);
    
    analysisLine.text += parsePv(line);

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

std::string UCIAnalysisParser::parseCpOrMate(const std::string& line) {
    
    // Parse the text for cp or mate (courtesy of Dan)
    size_t cpPos = line.find(" cp ");
    size_t matePos = line.find(" mate ");
    
    std::string startLine = "";
    if (matePos != std::string::npos) {
        // Mate in N moves
        size_t mateStart = matePos + 6;
        size_t mateEnd = line.find(" ", mateStart);
        std::string mateValue = line.substr(mateStart, mateEnd - mateStart);
        startLine = "Mate in " + mateValue;
    } else if (cpPos != std::string::npos) {
        // Centipawn evaluation
        size_t cpStart = cpPos + 4;
        size_t cpEnd = line.find(" ", cpStart);
        std::string cpValue = line.substr(cpStart, cpEnd - cpStart);
        // Convert centipawns to pawns (divide by 100)
        try {
            int cp = std::stoi(cpValue);
            double pawns = cp / 100.0;
            char evalStr[32];
            snprintf(evalStr, sizeof(evalStr), "%+.2f", pawns);
            startLine = std::string(evalStr);
        } catch (...) {
            startLine = cpValue + " cp";
        }
    }

    return startLine;
}

std::string UCIAnalysisParser::parsePv(const std::string& line) {
    
    std::string pvString = "";
    
    // Extract the principal variation (pv)
    size_t pvPos = line.find(" pv ");
    if (pvPos != std::string::npos) {
        size_t pvStart = pvPos + 4;
        std::string pv = line.substr(pvStart);
        
        // Limit PV display to first 5 moves
        std::string pvDisplay;
        int moveCount = 0;
        size_t pos = 0;
        while (moveCount < 5 && pos < pv.length()) {
            size_t nextSpace = pv.find(" ", pos);
            if (nextSpace == std::string::npos) {
                pvDisplay += pv.substr(pos);
                break;
            }
            pvDisplay += pv.substr(pos, nextSpace - pos) + " ";
            pos = nextSpace + 1;
            moveCount++;
        }
        
        if (moveCount == 5 && pos < pv.length()) {
            pvDisplay += "...";
        }
        
        pvString = " : " + pvDisplay;
    }

    return pvString;    
}