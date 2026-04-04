#include "Board.h"

int main(){
    // test 1: bit methods
    Bitboard bb;
    bb.setBit(2,2);
    bb.clearBit(2,2);
    if (bb.checkBit(2,2)){
        std::cout << "c3 is not empty" << std::endl;
    } else {
        std::cout << "c3 is empty" << std::endl;
    }


    // test 2: initialising starting positions
    Bitboard wp = Bitboard::wPawnBitboard();
    displayBoard(wp.board);


    // test 3: using operators
    Bitboard boardA;
    boardA.setBit(0,1);
    boardA.setBit(0,6);

    Bitboard boardB;
    boardB.setBit(2,2);
    boardB.setBit(2,5);

    Bitboard boardC = boardA | boardB;
    displayBoard(boardC.board);



    /* BOARD STRUCT */ 
    Board allBoards = Board::startingPosition();
    
    for (int z = W_PAWN; z <= B_KING; z++){
        displayBoard(allBoards.boards[z].board);
    } 

    Bitboard whites = allBoards.whitePieces();
    Bitboard blacks = allBoards.blackPieces();
    Bitboard occupied = allBoards.occupiedSquares();
    Bitboard empty = allBoards.emptySquares();

    displayBoard(whites.board);
    displayBoard(blacks.board);
    displayBoard(occupied.board);
    displayBoard(empty.board);

    return 0;
}