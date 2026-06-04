#ifndef BOARD_H
#define BOARD_H

#include<vector>
#include<sstream>
#include<algorithm>
#include<iomanip>
#include <intrin.h>
#include "Bitboard.h"


// operator overloading
struct Board{
    // member data
    std::array<Bitboard, 12> boards;
    bool whiteToMove = true;
    uint8_t castlingRights = 0b1111;
    int enPassantSquare = -1;
    int halfMoveClock = 0;
    int fullMoveNumber = 1;

    // constructor (empty cuz we already initialise with value 0ULL for default Bitboard)
    Board();

    // static factory method to construct starting position for all 12 piecetypes
    static Board startingPosition();
    static Board parseFEN(std::string fen);
    Bitboard whitePieces();
    Bitboard blackPieces();
    Bitboard occupiedSquares();
    Bitboard emptySquares();

    uint64_t zobristHash;
    bool isValid() const;
    void printBoardState() const;

};

void printHex(std::uint64_t value);


#endif