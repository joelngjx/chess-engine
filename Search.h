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
int quiescence(Board& b, int alpha, int beta);


#endif

