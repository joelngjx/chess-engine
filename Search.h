#ifndef SEARCH_H
#define SEARCH_H
#include "Board.h"
#include "Eval.h"
#include "Move.h"
#include <climits>
#include <chrono>


using MoveList = std::vector<Move>;


// material values (in centipawns)
constexpr std::array<int, 6> materialValues = {100, 500, 330, 320, 900, 20000};
constexpr int pawnValue = 100;
constexpr int rookValue = 500;
constexpr int bishopValue = 330;
constexpr int knightValue = 320;
constexpr int queenValue = 900;
constexpr int kingValue = 20000;


struct searchResult {
    int eval;
    Move bestMove;
};


struct moveListEval {
    MoveList moveList;
    std::vector<int> evalList;
};


searchResult minimax(Board& b, int depth, int originalDepth, int alpha, int beta, bool maximisingSide, std::vector<int>& nodeCounts);
moveListEval iterativeDeepening(Board& b, int maxDepth);
void moveListPrinter(MoveList moveList, std::vector<int> evalList);
void moveOrdering(MoveList& moveList);  // pass by reference


#endif

