#ifndef BOARD_H
#define BOARD_H

#include<vector>
#include<sstream>
#include<algorithm>
#include "Bitboard.h"


// operator overloading
struct Board{
    // member data
    std::array<Bitboard, 12> boards;
    bool whiteToMove;
    uint8_t castlingRights;
    int enPassantSquare;
    int halfMoveClock;
    int fullMoveNumber;

    // constructor (empty cuz we already initialise with value 0ULL for default Bitboard)
    Board();

    // static factory method to construct starting position for all 12 piecetypes
    static Board startingPosition();
    static Board parseFEN(std::string fen);
    Bitboard whitePieces();
    Bitboard blackPieces();
    Bitboard occupiedSquares();
    Bitboard emptySquares();
};


#endif