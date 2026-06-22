#ifndef MOVE_H
#define MOVE_H
#include<vector>
#include<cstdint>
#include "Bitboard.h"
#include "Board.h"


// FLAG CONSTANTS
constexpr std::uint8_t FLAG_CAPTURE       = 0b00000001;
constexpr std::uint8_t FLAG_DOUBLE_PUSH   = 0b00000010;
constexpr std::uint8_t FLAG_EN_PASSANT    = 0b00000100;
constexpr std::uint8_t FLAG_CASTLE_KSIDE  = 0b00001000;
constexpr std::uint8_t FLAG_CASTLE_QSIDE  = 0b00010000;
constexpr std::uint8_t PROMO_QUEEN        = 0b00100000;
constexpr std::uint8_t PROMO_ROOK         = 0b01000000;
constexpr std::uint8_t PROMO_BISHOP       = 0b01100000;
constexpr std::uint8_t PROMO_KNIGHT       = 0b10000000;


// structure to think about this:
// the Move struct represents a single possible move with specific characteristics
// these move objects will be combined into a MoveList later on
struct Move{
    int fromSquare;
    int toSquare;
    int pieceType;  // enum
    int capturedPieceType = -1;
    std::uint8_t flags;

    static Move nullMove(){
        return Move{-1, -1, -1, 0b0000};
    }

    bool isValid() const {
        return fromSquare != -1;
    }
};

using MoveList = std::vector<Move>;
void addMoveIfLegal(const Board& b, MoveList& list, Move m, bool whiteTurn);
void addMove(MoveList& list, Move m);


// move generation
uint64_t rookAttacks(int square, std::uint64_t occupied, std::uint64_t ownPieces);
uint64_t bishopAttacks(int square, std::uint64_t occupied, std::uint64_t ownPieces);
uint64_t queenAttacks(int square, std::uint64_t occupied, std::uint64_t ownPieces);

Board makeMove(const Board& b, Move m);
bool isInCheck(const Board& b, bool whiteKingInCheck);
MoveList generateLegalMoves(const Board& b);
std::uint64_t perft(int depth, const Board& b);

#endif