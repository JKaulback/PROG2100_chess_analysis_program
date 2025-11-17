#pragma once

class ChessMove {
    public:
        ChessMove(
            const int srcRank, const int srcFile, 
            const int destRank,const int destFile) 
            : srcRank(srcRank), srcFile(srcFile),
            destRank(destRank), destFile(destFile) {};
        
        int getSrcRank() const { return srcRank; };
        int getSrcFile() const { return srcFile; };
        int getDestRank() const {return destRank; };
        int getDestFile() const { return destFile; };

    private:
        int srcRank;
        int srcFile;
        int destRank;
        int destFile;
};