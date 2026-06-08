#ifndef EVAL_H
#define EVAL_H
#include "Board.h"


// material values (in centipawns)
constexpr int pawnValue = 100;
constexpr int knightValue = 320;
constexpr int bishopValue = 330;
constexpr int rookValue = 500;
constexpr int queenValue = 900;
constexpr int kingValue = 20000;


int evaluate(const Board &b);


#endif