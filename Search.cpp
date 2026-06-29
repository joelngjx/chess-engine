#include "Search.h"


// alpha -- best score found by maximising side (lower bound)
// beta -- best score found by minimising side (upper bound)
searchResult minimax(Board& b, int depth, int originalDepth, int alpha, int beta, bool maximisingSide, std::vector<int>& nodeCounts){
    nodeCounts[originalDepth - depth]++;
    Move bestMove = Move::nullMove();
    if (depth == 0) return {quiescence(b, alpha, beta), bestMove};

    if (maximisingSide){
        int maxEval = INT_MIN;
        MoveList list = generateLegalMoves(b);
        if (list.empty()){
            if (isInCheck(b, true)){
                return {-100000 + (originalDepth - depth), Move::nullMove()};
            } else {
                return {0, Move::nullMove()};
            }
        }

        moveOrdering(list);
        for (Move m: list){
            Board next = makeMove(b, m);
            searchResult currentResult = minimax(next, depth - 1, originalDepth, alpha, beta, false, nodeCounts);
            if (currentResult.eval > maxEval){
                maxEval = currentResult.eval;
                bestMove = m;
            }
            
            alpha = std::max(alpha, currentResult.eval);
            if (beta <= alpha){
                break;
            }
        }
        return {maxEval, bestMove};


    } else {
        int minEval = INT_MAX;
        MoveList list = generateLegalMoves(b);
        if (list.empty()){
            if (isInCheck(b, false)){
                return {100000 - (originalDepth - depth), Move::nullMove()};
            } else {
                return {0, Move::nullMove()};
            }
        }

        moveOrdering(list);
        for (Move m: list){
            Board next = makeMove(b, m);
            searchResult currentResult = minimax(next, depth - 1, originalDepth, alpha, beta, true, nodeCounts);
            if (currentResult.eval < minEval){
                minEval = currentResult.eval;
                bestMove = m;
            }

            beta = std::min(beta, currentResult.eval);
            if (beta <= alpha){
                break;
            }
        }
        return {minEval, bestMove};
    }
}


moveListEval iterativeDeepening(Board& b, int maxDepth){
    std::vector<Move> bestMoves;
    std::vector<int> evaluations;
    Board current = b;
    const auto searchTimeLimit = std::chrono::seconds(30);
    auto startTime = std::chrono::steady_clock::now();
    

    for (int i = 1; i <= maxDepth; i++){
        std::vector<int> nodeCounts(i + 1, 0);
        searchResult depthBestMove = minimax(current, i, i, -1000000, 1000000, current.whiteToMove, nodeCounts);


        std::cout << "Depth " << i 
          << " from=" << depthBestMove.bestMove.fromSquare
          << " to=" << depthBestMove.bestMove.toSquare
          << " valid=" << depthBestMove.bestMove.isValid() << "\n";


        if (!(depthBestMove.bestMove.isValid())){
            break;
        }

        bestMoves.push_back(depthBestMove.bestMove);
        evaluations.push_back(depthBestMove.eval);


        if (i == maxDepth){
            for (int j = 0; j <= maxDepth; j++){
                std::cout << "Nodes at depth " << j << ": " << nodeCounts[j] << "\n"; 
            }
        }

        auto elapsedTime = std::chrono::steady_clock::now() - startTime;
        if (elapsedTime >= searchTimeLimit){
            std::cout << "Time limit passed. Stopping move search." << std::endl;
            break;
        }
    }

    return {bestMoves, evaluations};

}


void moveOrdering(MoveList& movelist){
    std::sort(movelist.begin(), movelist.end(), [](Move a, Move b){
        int scoreA = (a.capturedPieceType != -1) ? (materialValues[a.capturedPieceType % 6] - materialValues[a.pieceType % 6]) : 0;
        int scoreB = (b.capturedPieceType != -1) ? (materialValues[b.capturedPieceType % 6] - materialValues[b.pieceType % 6]) : 0;
        return (scoreA > scoreB);
    });
}


void moveListPrinter(std::vector<Move> moveList, std::vector<int> evalList){
    for (int i = 0; i < evalList.size(); i++){
        std::cout << "Depth " << i + 1 << ": " << moveList[i].fromSquare << " to " << moveList[i].toSquare << "\n";
        std::cout << "Evaluation: " << evalList[i] << "\n"; 
    }
}


int quiescence(Board& b, int alpha, int beta){
    int staticEval = evaluate(b);

    int bestValue = staticEval;
    if (bestValue >= beta){
        return bestValue;
    }
    if (bestValue > alpha){
        alpha = bestValue;
    }

    // checking capture moves
    MoveList list = generateLegalMoves(b);
    MoveList captureList;
    for (Move m: list){
        if (m.flags & FLAG_CAPTURE){
            captureList.push_back(m);
        }
    }

    moveOrdering(captureList);

    for (Move capture: captureList){
        Board captured = makeMove(b, capture);
        int score = quiescence(captured, alpha, beta);

        if (score >= beta){
            return score;
        }
        if (score > bestValue){
            bestValue = score;
        }
        if (score > alpha){
            alpha = score;
        }

    }

    return bestValue;
}



/*
bugs
1. iterativeDeepening and MoveListPrinter do not produce the intended output
    - possible fix: error in depthBestMove.bestMove.isValid()
*/