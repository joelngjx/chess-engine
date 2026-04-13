#include "Board.h"

int main(){
    // INITIALISING GLOBAL CONSTANTS
    initKnightAttacks();
    initKingAttacks();

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
  
    /* display all baords
    std::cout << "Starting Positions: " << std::endl;
    for (int z = W_PAWN; z <= B_KING; z++){
        displayBoard(allBoards.boards[z].board);
    } 
    */


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
    std::uint64_t shiftedNorthEast = shiftNE(shiftTest.board);
    displayBoard(shiftedNorthEast);



    /* PIECE TYPE BITBOARDS */
    Bitboard wp = allBoards.boards[W_PAWN];
    Bitboard bp = allBoards.boards[B_PAWN];  
    Bitboard wn = allBoards.boards[W_KNIGHT];
    Bitboard bn = allBoards.boards[B_KNIGHT];
    Bitboard wk = allBoards.boards[W_KING];
    Bitboard bk = allBoards.boards[B_KING];


    std::cout << "Pawn Moves: " << std::endl;
    std::uint64_t wpMoves = wPawnPushes(wp.board, empty.board);
    displayBoard(wpMoves);

    std::uint64_t bpMoves = bPawnPushes(bp.board, empty.board);
    displayBoard(bpMoves);

    /* custom test
    std::uint64_t wpCaptures = wPawnCaptures(0x00810000000000ULL, blacks.board);
    displayBoard(wpCaptures);
    */

    std::cout << "Pawn Captures: " << std::endl;
    std::uint64_t wpCaptures = wPawnCaptures(wp.board, blacks.board);
    displayBoard(wpCaptures);

    std::uint64_t bpCaptures = bPawnCaptures(bp.board, whites.board);
    displayBoard(bpCaptures);

    std::cout << "Knight Moves: " << std::endl;
    std::uint64_t wnAttacks = knightAttacks(wn.board, blacks.board, empty.board);
    displayBoard(wnAttacks);

    std::uint64_t bnAttacks = knightAttacks(bn.board, whites.board, empty.board);
    displayBoard(bnAttacks);


    /* custom tests
    std::cout << "King Moves (Tests): " << std::endl;
    std::uint64_t wkMoves = kingAttacks(1ULL << 40, blacks.board, empty.board);
    displayBoard(wkMoves);

    std::uint64_t wkMovese4 = kingAttacks(1ULL << 28, blacks.board, empty.board);
    displayBoard(wkMovese4);
    */
    

    std::cout << "King Moves: " << std::endl;
    std::uint64_t wkAttacks = kingAttacks(wk.board, blacks.board, empty.board);
    displayBoard(wkAttacks);

    std::uint64_t bkAttacks = kingAttacks(bk.board, whites.board, empty.board);
    displayBoard(bkAttacks);


    std::cout << "All available moves for a given colour: " << std::endl;
    std::uint64_t whiteMoves = (wpMoves | wpCaptures | wnAttacks | wkAttacks);
    std::uint64_t blackMoves = (bpMoves | bpCaptures | bnAttacks | bkAttacks);
    displayBoard(whiteMoves);
    displayBoard(blackMoves);



    return 0;
}