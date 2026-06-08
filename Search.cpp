#include "Search.h"


int minimaxRoot(Board& b, int depth, int alpha, int beta, bool maximisingSide){
    std::vector<int> nodeCounts(depth + 1, 0);
    int result = minimax(b, depth, depth, alpha, beta, maximisingSide, nodeCounts);
    for (int i = 0; i <= depth; i++){
        std::cout << "Depth " << i << ": " << nodeCounts[i] << " nodes" << std::endl;
    }
    return result;
}


int minimax(Board& b, int depth, int originalDepth, int alpha, int beta, bool maximisingSide, std::vector<int>& nodeCounts){
    nodeCounts[originalDepth - depth]++;
    if (depth == 0) return evaluate(b);

    if (maximisingSide){
        int maxEval = INT_MIN;
        MoveList list = generateLegalMoves(b);
        if (list.empty()){
            if (isInCheck(b, true)){
                return -100000 + (originalDepth - depth);
            } else {
                return 0;
            }
        }

        for (Move m: list){
            Board next = makeMove(b, m);
            int eval = minimax(next, depth - 1, originalDepth, alpha, beta, false, nodeCounts);
            maxEval = std::max(eval, maxEval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha){
                break;
            }
        }
        return maxEval;


    } else {
        int minEval = INT_MAX;
        MoveList list = generateLegalMoves(b);
        if (list.empty()){
            if (isInCheck(b, false)){
                return 100000 - (originalDepth - depth);
            } else {
                return 0;
            }
        }

        for (Move m: list){
            Board next = makeMove(b, m);
            int eval = minimax(next, depth - 1, originalDepth, alpha, beta, true, nodeCounts);
            minEval = std::min(eval, minEval);
            beta = std::min(beta, eval);
            if (beta <= alpha){
                break;
            }
        }
        return minEval;
    }
}