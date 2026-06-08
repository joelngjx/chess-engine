#ifndef SEARCH_H
#define SEARCH_H
#include "Board.h"
#include "Eval.h"
#include "Move.h"
#include <climits>

int minimaxRoot(Board& b, int depth, int alpha, int beta, bool maximisingSide);
int minimax(Board& b, int depth, int originalDepth, int alpha, int beta, bool maximisingSide, std::vector<int>& nodeCounts);

#endif