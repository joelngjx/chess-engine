#include "Eval.h"

int evaluate(const Board &b){
    int whiteValues = 0, blackValues = 0;
    std::vector<int>pieceValues = {pawnValue, knightValue, bishopValue, rookValue, queenValue, kingValue};

    // white sum
    for (int p = W_PAWN; p <= W_KING; p++){
        whiteValues += pieceValues[p] * __popcnt64(b.boards[p].board);
    }


    // black sum
    for (int p = B_PAWN; p <= B_KING; p++){
        blackValues += pieceValues[p-6] * __popcnt64(b.boards[p].board);
    }

    int positionValue = whiteValues - blackValues;
    return positionValue;
}