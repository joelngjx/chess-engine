#include "Zobrist.h"

std::array<std::uint64_t, 781> zobristKeys;

void initZobrist(){
    std::mt19937_64 mt(121212121212ULL);
    std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);

    for (int count = 0; count < 781; count++){
        zobristKeys[count] = dist(mt);
    }
}


std::uint64_t computeHash(const Board& b){
    std::uint64_t hash = 0ULL;

    // pieces
    for (int piece = W_PAWN; piece <= B_KING; piece++){
        uint64_t bitboard = b.boards[piece].board; // get raw iomt64_t
        for (int sq = 0; sq < 64; sq++){
            if (bitboard & (1ULL << sq)){
                int index = (piece * 64) + sq;
                hash ^= zobristKeys[index];
            }
        }
    }


    // side to move
    // note: standard practice is to XOR black
    if (!b.whiteToMove){
        hash ^= zobristKeys[768];
    }


    // castling rights
    if (b.castlingRights & 1) hash ^= zobristKeys[769];
    if (b.castlingRights & 2) hash ^= zobristKeys[770];
    if (b.castlingRights & 4) hash ^= zobristKeys[771];
    if (b.castlingRights & 8) hash ^= zobristKeys[772];

    
    // en passant
    if (b.enPassantSquare != -1) {
        int file = b.enPassantSquare % 8; // Get the file (0 to 7)
        hash ^= zobristKeys[773 + file];
    }

    return hash;

}