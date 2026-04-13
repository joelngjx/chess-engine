#include "Bitboard.h"
std::array<std::uint64_t, 64> knightAttackTables;  // definition
std::array<std::uint64_t, 64> kingAttackTables; 

// TODO: ISSUE OPERATOR OVERLOADING FOR << AND >> NOT DONE. WRITE CUSTOM FUNCTION TO SHIFT STD::UINT64_T TYPES


/*
STRUCT DESIGN
=> Methods (that which belongs to a baord)
    - Operate directly on the board's internal uint64_t
    - Only needs one board to carry out a task 

=> Free functions (that which WORKS ON BOARDS => independant of board state)
    - Can apply to all instances


// using const Bitboard& prevents copies from being created 

*/


Bitboard operator&(const Bitboard& boardA, std::uint64_t boardB){
    return (boardA.board & boardB);
}

Bitboard operator&(const Bitboard& boardA, const Bitboard& boardB){
    return (boardA.board & boardB.board);
}

Bitboard operator|(const Bitboard& boardA, std::uint64_t boardB){
    return (boardA.board | boardB);
}

Bitboard operator|(const Bitboard& boardA, const Bitboard& boardB){
    return (boardA.board | boardB.board);
}

Bitboard operator^(const Bitboard& boardA, std::uint64_t boardB){
    return (boardA.board ^ boardB);
}

Bitboard operator^(const Bitboard& boardA, const Bitboard& boardB){
    return (boardA.board ^ boardB.board);
}



// shifting for std::uint64_t
std::uint64_t shiftIntLeft(std::uint64_t board, int amount){
    return (board << amount);
}


std::uint64_t shiftIntRight(std::uint64_t board, int amount){
    return (board >> amount);
}


// free functions
void displayBoard(std::uint64_t board){
    std::string ranks[8];
    int N;
    for (int rank = 7; rank >= 0; rank--){
        for (int file = 0; file <= 7; file++){
            N = 8*rank + file;
            bool occupied = (board >> N) & 1ULL;
            if (occupied){
                ranks[abs(rank-7)] += " 1";
            } else {
                ranks[abs(rank-7)] += " .";
            }
        }
    }

    for (int printing = 8; printing >= 1; printing--){
        std::cout << printing << " |" << ranks[abs(printing-8)] << std::endl;
    }

    std::cout << "    a b c d e f g h" << std::endl;
    std::cout << "\n" << std::endl;
}


int popLSB(std::uint64_t *board){
    if (*board != 0){
        for (int i = 0; i < 64; i++){
            if ((*board & (1ULL << i)) != 0){
                *board ^= (1ULL << i);
                return i;
            }
        }
    }

    return -1;   // error code to signal no bit found
}


std::uint64_t rankMask(int rank){
    std::uint64_t rmask = 0Ull;
    std::uint64_t toggle = 1ULL;
    for (int f = 0; f < 8; f++){
        rmask |= (toggle << (8*rank+f));
    }

    return rmask;
}


std::uint64_t fileMask(int file){
    std::uint64_t fmask = 0ULL;
    std::uint64_t toggle = 1ULL;
    for (int r = 0; r < 8; r++){
        fmask |= (toggle << (8*r+file));
    }

    return fmask;
}


std::uint64_t diagonalMask(int rank, int file){
    int diagvalue = rank - file + 7;
    std::uint64_t dmask = 0ULL;

    for (int r = 0; r < 8; r++){
        int f = r - diagvalue + 7;

        if (f >= 0 && f < 8){
            int index = 8*r + f;
            dmask |= (1ULL << index); // bitwise OR operation
        }
    }

    return dmask;
}


std::uint64_t antidiagMask(int rank, int file){
    std::uint64_t admask = 0ULL;
    int antidiagvalue = rank + file;

    for (int r = 0; r < 8; r++){
        int f = antidiagvalue - r;

        if (f >= 0 && f < 8){
            int index = 8*r + f;
            admask |= (1ULL << index);
        }
    }
    
    return admask;
}


// DIRECTIONAL BOARD SHIFTS
// note: uint64_t has a HARD BOUNDARY at 64 bits => anything that overflows is DISCARDED
std::uint64_t shiftNorth(std::uint64_t board){
    return shiftIntLeft(board, 8);
}

std::uint64_t shiftSouth(std::uint64_t board){
    return shiftIntRight(board, 8);
}

std::uint64_t shiftEast(std::uint64_t board){
    board &= NOT_H_FILE;
    return shiftIntLeft(board, 1);
}

std::uint64_t shiftWest(std::uint64_t board){
    board &= NOT_A_FILE;
    return shiftIntRight(board, 1);
}

std::uint64_t shiftNW(std::uint64_t board){
    board &= NOT_A_FILE;
    return shiftIntLeft(board, 7);
}

std::uint64_t shiftNE(std::uint64_t board){
    board &= NOT_H_FILE;
    return shiftIntLeft(board, 9);
}

std::uint64_t shiftSW(std::uint64_t board){
    board &= NOT_A_FILE;
    return shiftIntRight(board, 9);
}

std::uint64_t shiftSE(std::uint64_t board){
    board &= NOT_H_FILE;
    return shiftIntRight(board, 7);
}



// PAWN PUSHES AND CAPTURES
std::uint64_t wPawnPushes(std::uint64_t wp, std::uint64_t empty){
    std::uint64_t rank2 = shiftIntLeft(0xFFULL, 8);
    std::uint64_t singlesquare = shiftIntLeft(wp, 8);
    singlesquare &= empty;

    std::uint64_t twosquares = rank2 | wp;
    twosquares = shiftIntLeft(wp, 16);
    twosquares &= empty;

    return (singlesquare | twosquares);
}


std::uint64_t wPawnCaptures(std::uint64_t wp, std::uint64_t enemy){
    std::uint64_t captureDirections = shiftNW(wp) | shiftNE(wp);
    std::uint64_t captures = captureDirections & enemy;
    return captures;
}


std::uint64_t bPawnPushes(std::uint64_t bp, std::uint64_t empty){
    std::uint64_t rank7 = (0xFFULL << 48);
    std::uint64_t singlesquare = shiftIntRight(bp, 8);
    singlesquare &= empty;

    std::uint64_t twosquares = rank7 | bp;
    twosquares = shiftIntRight(bp, 16);
    twosquares &= empty;

    return (singlesquare | twosquares);
}


std::uint64_t bPawnCaptures(std::uint64_t bp, std::uint64_t enemy){
    std::uint64_t captureDirections = shiftSW(bp) | shiftSE(bp);
    std::uint64_t captures = captureDirections & enemy;
    return captures;
}



// KNIGHT MOVES
void initKnightAttacks(){
    for (int sq = 0; sq < 64; sq++){
        std::uint64_t knightMoves = (1ULL << sq);
        std::uint64_t NNW = ((knightMoves & NOT_A_FILE) << 15);
        std::uint64_t NNE = ((knightMoves & NOT_H_FILE) << 17);
        std::uint64_t NWW = ((knightMoves & (NOT_A_FILE | NOT_B_FILE)) << 10);
        std::uint64_t NEE = ((knightMoves & (NOT_G_FILE | NOT_H_FILE)) << 6);
        std::uint64_t SSW = ((knightMoves & NOT_A_FILE) >> 17);
        std::uint64_t SSE = ((knightMoves & NOT_H_FILE) >> 15);
        std::uint64_t SWW = ((knightMoves & (NOT_A_FILE | NOT_B_FILE)) >> 10);
        std::uint64_t SEE = ((knightMoves & (NOT_G_FILE | NOT_H_FILE)) >> 6);

        knightMoves = (NNW | NNE | NWW | NEE | SSW | SSE | SWW | SEE);
        knightAttackTables[sq] = knightMoves;
    }
}


std::uint64_t knightAttacks(std::uint64_t knights, std::uint64_t enemy, std::uint64_t empty){
    std::uint64_t temp = knights;
    std::uint64_t knightMoves = 0ULL;

    while (temp != 0ULL){
        int current = popLSB(&temp);
        knightMoves |= knightAttackTables[current];
        knightMoves = (knightMoves & (empty | enemy));
    }

    return knightMoves;
}


// KING MOVES
void initKingAttacks(){
    for (int sq = 0; sq < 64; sq++){
        std::uint64_t kingMoves = (1ULL << sq);
        std::uint64_t N = (kingMoves << 8);
        std::uint64_t S = (kingMoves >> 8);
        std::uint64_t E = ((kingMoves & NOT_H_FILE) << 1);
        std::uint64_t W = ((kingMoves & NOT_A_FILE) >> 1);

        std::uint64_t SW = ((kingMoves & NOT_A_FILE) >> 9);
        std::uint64_t SE = ((kingMoves & NOT_H_FILE) >> 7);
        std::uint64_t NW = ((kingMoves & NOT_A_FILE) << 7);
        std::uint64_t NE = ((kingMoves & NOT_H_FILE) << 9);

        kingMoves = (N | S | E | W | NW | NE | SW | SE);
        kingAttackTables[sq] = kingMoves;
    }
}


std::uint64_t kingAttacks(std::uint64_t king, std::uint64_t enemy, std::uint64_t empty){
    std::uint64_t temp = king;
    std::uint64_t kingMoves = 0ULL;

    while (temp != 0ULL){
        int current = popLSB(&temp);
        kingMoves |= kingAttackTables[current];
        kingMoves = (kingMoves & (empty | enemy));
    }

    return kingMoves;
}


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

*/