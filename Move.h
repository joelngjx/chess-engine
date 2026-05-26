#ifndef MOVE_H
#define MOVE_H
#include<vector>
#include<cstdint>
#include "Bitboard.h"


// FLAG CONSTANTS
constexpr std::uint8_t FLAG_CAPTURE     = 0b00000001;
constexpr std::uint8_t FLAG_DOUBLE_PUSH = 0b00000010;
constexpr std::uint8_t FLAG_EN_PASSANT  = 0b00000100;
constexpr std::uint8_t FLAG_CASTLING    = 0b00001000;
constexpr std::uint8_t PROMO_QUEEN      = 0b00010000;
constexpr std::uint8_t PROMO_ROOK       = 0b00100000;
constexpr std::uint8_t PROMO_BISHOP     = 0b00110000;
constexpr std::uint8_t PROMO_KNIGHT     = 0b01000000;


// structure to think about this:
// the Move struct represents a single possible move with specific characteristics
// these move objects will be combined into a MoveList later on
struct Move{
    int fromSquare;
    int toSquare;
    int pieceType;  // enum
    std::uint8_t flags;
};

using MoveList = std::vector<Move>;
void addMove(MoveList& list, Move m);


// move generation
uint64_t rookAttacks(int square, std::uint64_t occupied, std::uint64_t ownPieces);
uint64_t bishopAttacks(int square, std::uint64_t occupied, std::uint64_t ownPieces);
uint64_t queenAttacks(int square, std::uint64_t occupied, std::uint64_t ownPieces);


#endif