#include "Board.h"

int main(){
    /*

    // test 1: bit methods
    Bitboard bb;
    bb.setBit(2,2);
    bb.clearBit(2,2);
    if (bb.checkBit(2,2)){
        std::cout << "c3 is not empty" << std::endl;
    } else {
        std::cout << "c3 is empty" << std::endl;
    }


    // test 2: initialising starting positions -- DEPRECATED FUNCTION
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

    */

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



    /* SHIFT TESTING */
    Bitboard shiftTest;
    shiftTest.setBit(3,3);
    shiftTest.setBit(0,0);
    shiftTest.setBit(0,7);
    shiftTest.setBit(7,0);
    shiftTest.setBit(7,7);

    displayBoard(shiftTest.board);
    std::uint64_t shiftedNorth = shiftNE(shiftTest.board);
    displayBoard(shiftedNorth);



    /* PIECE TYPE BITBOARDS */
    Bitboard wp = allBoards.boards[W_PAWN];
    Bitboard bp = allBoards.boards[B_PAWN];  

    std::uint64_t wpMoves = wPawnPushes(wp.board, empty.board);
    displayBoard(wpMoves);

    std::uint64_t bpMoves = bPawnPushes(bp.board, empty.board);
    displayBoard(bpMoves);

    /* custom test
    std::uint64_t wpCaptures = wPawnCaptures(0x00810000000000ULL, blacks.board);
    displayBoard(wpCaptures);
    */

    std::uint64_t wpCaptures = wPawnCaptures(wp.board, blacks.board);
    displayBoard(wpCaptures);

    std::uint64_t bpCaptures = bPawnCaptures(bp.board, whites.board);
    displayBoard(bpCaptures);

    return 0;
}