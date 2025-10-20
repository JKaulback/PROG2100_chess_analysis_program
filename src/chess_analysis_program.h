#ifndef CHESS_ANALYSIS_PROGRAM_H
#define CHESS_ANALYSIS_PROGRAM_H

#include <raylib.h>

class ChessAnalysisProgram {
public:
    ChessAnalysisProgram();
    ~ChessAnalysisProgram();

    void run();
private:
    Texture2D boardTexture;
    void UpdateGame();
    void RenderGame();
    
};

#endif // CHESS_ANALYSIS_PROGRAM_H