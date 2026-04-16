#include "Board.h"


// constructor (empty cuz we already initialise with value 0ULL for default Bitboard)
Board::Board() {}

// static factory method to construct starting position for all 12 piecetypes
Board Board::startingPosition(){
    Board b;
    b.boards[W_PAWN] = 0x000000000000FF00ULL;
    b.boards[W_ROOK] = 0x0000000000000081ULL;
    b.boards[W_BISHOP] = 0x0000000000000024ULL;
    b.boards[W_KNIGHT] = 0x0000000000000042ULL;
    b.boards[W_QUEEN] = 0x0000000000000008ULL;
    b.boards[W_KING] = 0x0000000000000010ULL;
    b.boards[B_PAWN] = 0x00FF000000000000ULL;
    b.boards[B_ROOK] = 0x8100000000000000ULL;
    b.boards[B_BISHOP] = 0x2400000000000000ULL;
    b.boards[B_KNIGHT] = 0x4200000000000000ULL;
    b.boards[B_QUEEN] = 0x0800000000000000ULL;
    b.boards[B_KING] = 0x1000000000000000ULL;
    return b;
}


Board Board::parseFEN(std::string fen){
    Board b;
    std::stringstream test(fen);
    std::vector<std::string> ranks;
    std::string rank;

    while (std::getline(test, rank, '/')){
        ranks.push_back(rank);
    }

    std::reverse(ranks.begin(), ranks.end());


    // parsing piecetypes and digits
    for (int rank = 0; rank < 8; rank++){
        int file = 0;
        std::string rankString = ranks[rank];
        

        for (char c: rankString){
            if (std::isdigit(c)){
                file += (c - '0');
                continue;
            } else {
                int pieceType = -1;
                switch(c){
                // White Pieces
                    case 'P': pieceType = W_PAWN; break; 
                    case 'R': pieceType = W_ROOK; break; 
                    case 'B': pieceType = W_BISHOP; break;
                    case 'N': pieceType = W_KNIGHT; break;
                    case 'Q': pieceType = W_QUEEN; break;
                    case 'K': pieceType = W_KING; break;

                    // Black Pieces
                    case 'p': pieceType = B_PAWN; break;
                    case 'r': pieceType = B_ROOK; break;
                    case 'b': pieceType = B_BISHOP; break;
                    case 'n': pieceType = B_KNIGHT; break;
                    case 'q': pieceType = B_QUEEN; break;
                    case 'k': pieceType = B_KING; break;
                    
                    default: break;
                }

                if (pieceType != -1){
                    b.boards[pieceType] |= (1ULL << (8*rank+file));   
                    file++;
                }
            }
        }
    }

    return b;
}


Bitboard Board::whitePieces(){
    Bitboard temp;
    for (int white = W_PAWN; white <= W_KING; white++){
        temp.board |= boards[white].board;
    }

    return temp;
};


Bitboard Board::blackPieces(){
    Bitboard temp;
    for (int black = B_PAWN; black <= B_KING; black++){
        temp.board |= boards[black].board;
    }

    return temp;
};

Bitboard Board::occupiedSquares(){
    Bitboard occupied;
    occupied.board = (whitePieces() | blackPieces()).board;
    return occupied;
}

Bitboard Board::emptySquares(){
    Bitboard empty;
    empty.board = (~occupiedSquares().board);
    return empty;
}



/*
    int main(){
        // BOARD STRUCT 
        Board allBoards = Board::startingPosition();
        
        for (int z = W_PAWN; z <= B_KING; z++){
            displayBoard(allBoards.boards[z].board);
        } 

        Bitboard whites = allBoards.whitePieces();
        Bitboard blacks = allBoards.blackPieces();
        Bitboard occupied = allBoards.occupiedSquares();
        Bitboard empty = allBoards.emptySquares();

        displayBoard(whites);
        displayBoard(blacks);
        displayBoard(occupied);
        displayBoard(empty);

        return 0;
    }
*/