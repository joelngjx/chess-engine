#ifndef BITBOARD_H
#define BITBOARD_H

#include<iostream>
#include<cstdint>
#include<string>
#include<bitset>
#include<array>


// enum for piece type and colour
enum pieceTypes {
    W_PAWN = 0,
    W_ROOK = 1,
    W_BISHOP = 2,
    W_KNIGHT = 3,
    W_QUEEN = 4,
    W_KING = 5,
    B_PAWN = 6,
    B_ROOK = 7,
    B_BISHOP = 8,
    B_KNIGHT = 9,
    B_QUEEN = 10,
    B_KING = 11
};


struct Bitboard {
    // member data
    std::uint64_t board;


    // constructor
    // parameterised
    Bitboard(std::uint64_t b){
        board = b;
    }

    // default no-argument
    Bitboard(){
        board = 0ULL;
    }


    // static factory methods
    static Bitboard wPawnBitboard(){
        Bitboard b;
        int rank = 1;
        for (int file = 0; file < 8; file++){
            int index = 8*rank + file;
            std::uint64_t mask = 1ULL << index;
            b.board |= mask;
        }

        return b;
    }


    // conversion operator => non-zero board is true, else false
    operator bool() const {
        return board != 0ULL;
    }


    // methods
    void setBit(int rank, int file){
        int index = 8*rank + file;
        std::uint64_t shift = 1ULL << index;
        board |= shift;
    }

    void clearBit(int rank, int file){
        int index = 8*rank + file;
        std::uint64_t shift = 1ULL << index;
        shift = ~(shift);
        board &= shift;
    }

    bool checkBit(int rank, int file){
        int index = 8*rank + file;
        std::uint64_t shift = 1ULL << index;
        if (board & shift){
            return true;
        } else {
            return false;
        }
    }

    // 0xFFULL carries out AND operation between board and 00000000 ... 11111111, zeroing all bits except for the rightmost eight.
    std::bitset<8> piecesOnRank(int rank){
        std::uint64_t rmask = (board >> (8*rank)) & 0xFFULL;
        return std::bitset<8>(rmask);
    }

    std::bitset<8> piecesOnFile(int file){
        static const std::uint64_t fileMasks[8] = {
            0x0101010101010101ULL,  // a-file
            0x0202020202020202ULL,  // b-file
            0x0404040404040404ULL,  // c-file
            0x0808080808080808ULL,  // d-file
            0x1010101010101010ULL,  // e-file
            0x2020202020202020ULL,  // f-file
            0x4040404040404040ULL,  // g-file
            0x8080808080808080ULL   // h-file
        };

        std::uint64_t fmask = ((board & fileMasks[file]) >> file) * 0x0101010101010101ULL;
        fmask = (fmask >> 56) & 0xFFULL; // shows files where a piece is present
        return std::bitset<8>(fmask);
    }

    Bitboard shiftLeft(int amount){
        return Bitboard(board << amount);
    }

    Bitboard shiftRight(int amount){
        return Bitboard(board >> amount);
    }

    std::uint64_t wPawnMoves(){
        std::uint64_t empty = ~(board);
        std::uint64_t whitePawnMoves = 0ULL;
        whitePawnMoves |= board;
        whitePawnMoves = ((whitePawnMoves << 8) & empty);

        // extra step for first moves
        whitePawnMoves &= (0xFFULL << 16);
        std::uint64_t firstMove = ((whitePawnMoves << 8) & empty);

        whitePawnMoves |= firstMove;
        return whitePawnMoves;
    }


    // operator overloading
    friend Bitboard operator&(const Bitboard& boardA, std::uint64_t boardB);
    friend Bitboard operator&(const Bitboard& boardA, const Bitboard& boardB);
    friend Bitboard operator|(const Bitboard& boardA, std::uint64_t boardB);
    friend Bitboard operator|(const Bitboard& boardA, const Bitboard& boardB);
    friend Bitboard operator^(const Bitboard& boardA, std::uint64_t boardB);
    friend Bitboard operator^(const Bitboard& boardA, const Bitboard& boardB);
    
    
    // case 1: using a bitboard and a uint64_t
    // this is a pointer, by using * we get the actual value
    Bitboard& operator|=(std::uint64_t or_board){
        board |= or_board;
        return *this;   // pointer
    }

    Bitboard& operator&=(std::uint64_t and_board){
        board &= and_board;
        return *this;   // pointer
    }

    Bitboard& operator^=(std::uint64_t xor_board){
        board ^= xor_board;
        return *this;   // pointer
    }


    // case 2: using 2 bitboards
    Bitboard& operator|=(const Bitboard& or_b){
        board |= or_b.board;
        return *this;   // pointer
    }

    Bitboard& operator&=(const Bitboard& and_b){
        board &= and_b.board;
        return *this;   // pointer
    }

    Bitboard& operator^=(const Bitboard& xor_b){
        board ^= xor_b.board;
        return *this;   // pointer
    }

};


void displayBoard(std::uint64_t board);
std::uint64_t rankMask(int rank);
std::uint64_t fileMask(int file);
std::uint64_t diagonalMask(int rank, int file);
std::uint64_t antidiagMask(int rank, int file);




#endif