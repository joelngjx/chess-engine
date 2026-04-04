#ifndef BOARD_H
#define BOARD_H

#include<iostream>
#include<cstdint>
#include<string>
#include<bitset>
#include<array>
#include "Bitboard.h"


// operator overloading
struct Board{
    // member data
    std::array<Bitboard, 12> boards;

    // constructor (empty cuz we already initialise with value 0ULL for default Bitboard)
    Board();

    // static factory method to construct starting position for all 12 piecetypes
    static Board startingPosition();
    Bitboard whitePieces();
    Bitboard blackPieces();
    Bitboard occupiedSquares();
    Bitboard emptySquares();
};


#endif