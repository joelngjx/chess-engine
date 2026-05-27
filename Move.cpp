#include "Move.h"
#include "Zobrist.h"


void addMove(MoveList& list, Move m){
    list.push_back(m);
}

uint64_t rookAttacks(int square, std::uint64_t occupied, std::uint64_t ownPieces){
    std::uint64_t rayAttacks = 0ULL;

    std::uint64_t westRay = (1ULL << square);
    bool West = true;
    while (West){
        if (westRay == 0){
            break;
        }

        westRay = shiftWest(westRay);
        if (occupied & westRay){
            if(!(ownPieces & westRay)){
                rayAttacks |= westRay;
                break;
            } else {
                break;
            }
        } else {
            rayAttacks |= westRay;
        }
    }

    std::uint64_t eastRay = (1ULL << square);
    bool East = true;
    while (East){
        if (eastRay == 0){
            break;
        }

        eastRay = shiftEast(eastRay);
        if (occupied & eastRay){
            if(!(ownPieces & eastRay)){
                rayAttacks |= eastRay;
                break;
            } else {
                break;
            }
        } else {
            rayAttacks |= eastRay;
        }
    }

    std::uint64_t northRay = (1ULL << square);
    bool North = true;
    while (North){
        if (northRay == 0){
            break;
        }

        northRay = shiftNorth(northRay);
        if (occupied & northRay){
            if(!(ownPieces & northRay)){
                rayAttacks |= northRay;
                break;
            } else {
                break;
            }
        } else {
            rayAttacks |= northRay;
        }
    }

    std::uint64_t southRay = (1ULL << square);
    bool South = true;
    while (South){
        if (southRay == 0){
            break;
        }

        southRay = shiftEast(southRay);
        if (occupied & southRay){
            if(!(ownPieces & southRay)){
                rayAttacks |= southRay;
                break;
            } else {
                break;
            }
        } else {
            rayAttacks |= southRay;
        }
    }

    return rayAttacks;
};

/*
std::uint64_t shiftEast(std::uint64_t board){
    board &= NOT_H_FILE;
    return shiftIntLeft(board, 1);
}
*/


uint64_t bishopAttacks(int square, std::uint64_t occupied, std::uint64_t ownPieces){
    std::uint64_t rayAttacks = 0ULL;

    std::uint64_t NWRay = (1ULL << square);
    bool NW = true;
    while(NW){
        if (NWRay == 0){
            break;
        }

        NWRay = shiftNW(NWRay);
        if(occupied & NWRay){
            if(!(ownPieces & NWRay)){
                rayAttacks |= NWRay;
                break;
            } else {
                break;
            }
        } else {
            rayAttacks |= NWRay;
        }
    }


    std::uint64_t NERay = (1ULL << square);
    bool NE = true;
    while(NE){
        if (NERay == 0){
            break;
        }

        NERay = shiftNE(NERay);
        if(occupied & NERay){
            if(!(ownPieces & NERay)){
                rayAttacks |= NERay;
                break;
            } else {
                break;
            }
        } else {
            rayAttacks |= NERay;
        }
    }


    std::uint64_t SWRay = (1ULL << square);
    bool SW = true;
    while(SW){
        if (SWRay == 0){
            break;
        }

        SWRay = shiftSW(SWRay);
        if(occupied & SWRay){
            if(!(ownPieces & SWRay)){
                rayAttacks |= SWRay;
                break;
            } else {
                break;
            }
        } else {
            rayAttacks |= SWRay;
        }
    }


    std::uint64_t SERay = (1ULL << square);
    bool SE = true;
    while(SE){
        if (SERay == 0){
            break;
        }

        SERay = shiftSE(SERay);
        if(occupied & SERay){
            if(!(ownPieces & SERay)){
                rayAttacks |= SERay;
                break;
            } else {
                break;
            }
        } else {
            rayAttacks |= SERay;
        }
    }

    return rayAttacks;
};


uint64_t queenAttacks(int square, std::uint64_t occupied, std::uint64_t ownPieces){
    std::uint64_t rankFileAttacks = rookAttacks(square, occupied, ownPieces);
    std::uint64_t diagonalAttacks = bishopAttacks(square, occupied, ownPieces);

    return (rankFileAttacks | diagonalAttacks);
};


Board makeMove(const Board& b, Move m){
    // updating bitboards
    Board original = b;
    int piece = m.pieceType;
    bool capture = false;
    bool castleWK = false;
    bool castleWQ = false;
    bool castleBK = false;
    bool castleBQ = false;
    int enpassantfile = -1;
    int side = 0;



    // side to move
    if ((piece >= 0) && (piece <= 5)){
        original.whiteToMove = false;  // for next move
    } else {
        original.whiteToMove = true;
        side = 6;
    }


    // check for captures
    if (m.flags & FLAG_CAPTURE){
        capture = true;
    }


    // check for double push
    if (m.flags & FLAG_DOUBLE_PUSH){
        if (original.whiteToMove){
            original.enPassantSquare = m.toSquare + 8;
        } else {
            original.enPassantSquare = m.toSquare - 8;
        }
    } else {
        original.enPassantSquare = -1;
    }


    // change in castling flags
    if (m.flags & FLAG_CASTLE_KSIDE){
        if (!(original.whiteToMove)){  // white castled
            original.castlingRights &= ~1;
            castleWK = true;
            original.zobristHash ^= zobristKeys[769];
        } else {                       // black castled
            original.castlingRights &= ~4;
            castleBK = true;
            original.zobristHash ^= zobristKeys[771];
        }
    } else if (m.flags & FLAG_CASTLE_QSIDE){
        if (!(original.whiteToMove)){
            original.castlingRights &=~ 2;
            castleWQ = true;
            original.zobristHash ^= zobristKeys[770];
        } else {
            original.castlingRights &=~ 8;
            castleBQ = true;
            original.zobristHash ^= zobristKeys[772];
        }
    }

    if (m.pieceType == W_KING){
        original.castlingRights &= ~(1 | 2);
    } else if (m.pieceType == B_KING){
        original.castlingRights &= ~(4 | 8);
    } else if (m.pieceType == W_ROOK){
        if (m.fromSquare == 0){
            original.castlingRights &= ~(2);
        } else if (m.fromSquare == 7){
            original.castlingRights &= ~(1);
        }
    } else if (m.pieceType == B_ROOK){
        if (m.fromSquare == 63){
            original.castlingRights &= ~(4);
        } else if (m.fromSquare == 56){
            original.castlingRights &= ~(8);
        }
    }


    // checking for en passant
    // file = index modulo 8
    if (m.flags & FLAG_EN_PASSANT){
        enpassantfile = m.toSquare % 8;
        capture = true;
        original.zobristHash ^= zobristKeys[773 + enpassantfile];
    }


    if (m.flags & PROMO_QUEEN){
        original.boards[piece] ^= (1ULL << m.fromSquare);
        original.zobristHash ^= zobristKeys[piece * 64 + m.fromSquare];
        if (original.whiteToMove){
            original.boards[B_QUEEN] |= (1ULL << m.toSquare);
            original.zobristHash ^= zobristKeys[B_QUEEN * 64 + m.fromSquare];
        } else {
            original.boards[W_QUEEN] |= (1ULL << m.toSquare);
            original.zobristHash ^= zobristKeys[W_QUEEN * 64 + m.fromSquare];
        }
    } else if (m.flags & PROMO_BISHOP){
        original.boards[piece] ^= (1ULL << m.fromSquare);
        original.zobristHash ^= zobristKeys[piece * 64 + m.fromSquare];
        if (original.whiteToMove){
            original.boards[B_BISHOP] |= (1ULL << m.toSquare);
            original.zobristHash ^= zobristKeys[B_BISHOP * 64 + m.fromSquare];
        } else {
            original.boards[W_BISHOP] |= (1ULL << m.toSquare);
            original.zobristHash ^= zobristKeys[W_BISHOP * 64 + m.fromSquare];
        }
    } else if (m.flags & PROMO_KNIGHT){
        original.boards[piece] ^= (1ULL << m.fromSquare);
        original.zobristHash ^= zobristKeys[piece * 64 + m.fromSquare];
        if (original.whiteToMove){
            original.boards[B_KNIGHT] |= (1ULL << m.toSquare);
            original.zobristHash ^= zobristKeys[B_KNIGHT * 64 + m.fromSquare];
        } else {
            original.boards[W_KNIGHT] |= (1ULL << m.toSquare);
            original.zobristHash ^= zobristKeys[W_KNIGHT * 64 + m.fromSquare];
        }
    } else if (m.flags & PROMO_ROOK){
        original.boards[piece] ^= (1ULL << m.fromSquare);
        original.zobristHash ^= zobristKeys[piece * 64 + m.fromSquare];
        if (original.whiteToMove){
            original.boards[B_ROOK] |= (1ULL << m.toSquare);
            original.zobristHash ^= zobristKeys[B_ROOK * 64 + m.fromSquare];
        } else {
            original.boards[W_ROOK] |= (1ULL << m.toSquare);
            original.zobristHash ^= zobristKeys[W_ROOK* 64 + m.fromSquare];
        }
    } else {
        original.boards[piece] |= (1ULL << m.toSquare);
        original.zobristHash ^= zobristKeys[piece * 64 + m.toSquare];
        original.boards[piece] ^= (1ULL << m.fromSquare);
        original.zobristHash ^= zobristKeys[piece * 64 + m.fromSquare];
    }


    if (capture && (enpassantfile == -1)){
        for (int enemy = 0 + side; enemy <= 5 + side; enemy++){
            if (((1ULL << m.toSquare) & (original.boards[enemy].board))  ){
                original.boards[enemy] ^= (1ULL << m.toSquare);
                original.zobristHash ^= zobristKeys[enemy * 64 + m.toSquare];
                break;
            }
        }
    }


    // hard coding specific rook moves for castling
    // W_ROOK = 1, B_ROOK = 7
    if (castleWK){
        original.boards[W_ROOK] |= (1ULL << 5);
        original.zobristHash ^= zobristKeys[W_ROOK * 64 + 5];
        original.boards[W_ROOK] ^= (1ULL << 7);
        original.zobristHash ^= zobristKeys[W_ROOK * 64 + 7];
    } else if (castleWQ){
        original.boards[W_ROOK] |= (1ULL << 3);
        original.zobristHash ^= zobristKeys[W_ROOK * 64 + 3];
        original.boards[W_ROOK] ^= (1ULL);
        original.zobristHash ^= zobristKeys[W_ROOK * 64];
    } else if (castleBK){
        original.boards[B_ROOK] |= (1ULL << 61);
        original.zobristHash ^= zobristKeys[B_ROOK * 64 + 61];
        original.boards[B_ROOK] ^= (1ULL << 63);
        original.zobristHash ^= zobristKeys[B_ROOK * 64 + 63];
    } else if (castleBQ){
        original.boards[B_ROOK] |= (1ULL << 59);
        original.zobristHash ^= zobristKeys[B_ROOK * 64 + 59];
        original.boards[B_ROOK] ^= (1ULL << 56);
        original.zobristHash ^= zobristKeys[B_ROOK * 64 + 56];
    }
    

    // piece removal for en passant
    if (enpassantfile != -1){
        int capturedSquare;
        if (original.whiteToMove){  // white pawn captured en passant
            capturedSquare = 8*3 + enpassantfile;
            original.boards[W_PAWN] ^= (1ULL << capturedSquare);
            original.zobristHash ^= zobristKeys[W_PAWN * 64 + capturedSquare];
        } else {                    // black pawn captured en passant
            capturedSquare = 8*4 + enpassantfile;
            original.boards[B_PAWN] ^= (1ULL << capturedSquare);
            original.zobristHash ^= zobristKeys[B_PAWN * 64 + capturedSquare];
        }
    }


    // updating board state
    if (((piece == 0) || (piece == 6)) || capture){
        original.halfMoveClock = 0;
    }
    if (original.whiteToMove) original.fullMoveNumber++;


    // updating Zobrist hash
    original.zobristHash ^= zobristKeys[768];   // xor sidetomove regardless
    return original;
}


/*
TODO
1. hardcode rook moves depending on colour and castling type [DONE]
2. change in board state for en passant [DONE]
3. handle zobrist xor-ing
4. handle king in check function
*/