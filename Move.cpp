#include "Move.h"
#include "Zobrist.h"
#include<cassert>


void addMoveIfLegal(const Board& b, MoveList& list, Move m, bool whiteTurn){
    Board postMove = makeMove(b, m);
    if (!isInCheck(postMove, whiteTurn)){
        addMove(list, m);
    }
}


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

        southRay = shiftSouth(southRay);
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
    bool capture = (m.flags & FLAG_CAPTURE);
    bool castleWK = false;
    bool castleWQ = false;
    bool castleBK = false;
    bool castleBQ = false;
    bool promo = false;
    int enpassantfile = -1;


    bool whiteMoved = b.whiteToMove;
    int side = whiteMoved ? 6 : 0;

    // side to move
    original.whiteToMove = !whiteMoved;


    // check for double push
    if (m.flags & FLAG_DOUBLE_PUSH){
        if (whiteMoved){
            std::uint64_t enemyPawnCapture = bPawnCaptures(original.boards[B_PAWN].board, (1ULL << (m.toSquare - 8)));
            if (enemyPawnCapture != 0){
                original.enPassantSquare = m.toSquare - 8;
            } else {
                original.enPassantSquare = -1;
            }
        } else {
            std::uint64_t enemyPawnCapture = wPawnCaptures(original.boards[W_PAWN].board, (1ULL << (m.toSquare + 8)));
            if (enemyPawnCapture != 0){
                original.enPassantSquare = m.toSquare + 8;
            } else {
                original.enPassantSquare = -1;
            }
        }
    } else {
        original.enPassantSquare = -1;
    }


    // change in castling flags
    if (m.flags & FLAG_CASTLE_KSIDE){
        if (whiteMoved){  // white castled
            original.castlingRights &= ~1;
            castleWK = true;
            original.zobristHash ^= zobristKeys[769];
        } else {                       // black castled
            original.castlingRights &= ~4;
            castleBK = true;
            original.zobristHash ^= zobristKeys[771];
        }
    } else if (m.flags & FLAG_CASTLE_QSIDE){
        if (whiteMoved){
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


    if (m.flags & FLAG_CAPTURE){
        switch(m.toSquare){
            case 0: original.castlingRights &= ~2; break;
            case 7: original.castlingRights &= ~1; break;
            case 56: original.castlingRights &= ~8; break;
            case 63: original.castlingRights &= ~4; break;
            default: break;
        }
    }


    // checking for en passant
    // file = index modulo 8
    if (m.flags & FLAG_EN_PASSANT){
        enpassantfile = m.toSquare % 8;
        capture = true;
        original.zobristHash ^= zobristKeys[773 + enpassantfile];
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

    original.boards[piece] ^= (1ULL << m.fromSquare);
    original.zobristHash ^= zobristKeys[piece * 64 + m.fromSquare];
    uint8_t promoType = m.flags & 0b11100000;
    if (promoType != 0){
        if (promoType == PROMO_QUEEN){
            if (!(whiteMoved)){
                original.boards[B_QUEEN] |= (1ULL << m.toSquare);
                original.zobristHash ^= zobristKeys[B_QUEEN * 64 + m.toSquare];
            } else {
                original.boards[W_QUEEN] |= (1ULL << m.toSquare);
                original.zobristHash ^= zobristKeys[W_QUEEN * 64 + m.toSquare];
            }
            promo = true;
        } else if (promoType == PROMO_BISHOP){
            if (!(whiteMoved)){
                original.boards[B_BISHOP] |= (1ULL << m.toSquare);
                original.zobristHash ^= zobristKeys[B_BISHOP * 64 + m.toSquare];
            } else {
                original.boards[W_BISHOP] |= (1ULL << m.toSquare);
                original.zobristHash ^= zobristKeys[W_BISHOP * 64 + m.toSquare];
            }
            promo = true;
        } else if (promoType == PROMO_KNIGHT){
            if (!(whiteMoved)){
                original.boards[B_KNIGHT] |= (1ULL << m.toSquare);
                original.zobristHash ^= zobristKeys[B_KNIGHT * 64 + m.toSquare];
            } else {
                original.boards[W_KNIGHT] |= (1ULL << m.toSquare);
                original.zobristHash ^= zobristKeys[W_KNIGHT * 64 + m.toSquare];
            }
            promo = true;
        } else if (promoType == PROMO_ROOK){
            if (!(whiteMoved)){
                original.boards[B_ROOK] |= (1ULL << m.toSquare);
                original.zobristHash ^= zobristKeys[B_ROOK * 64 + m.toSquare];
            } else {
                original.boards[W_ROOK] |= (1ULL << m.toSquare);
                original.zobristHash ^= zobristKeys[W_ROOK* 64 + m.toSquare];
            }
            promo = true;
        } 
    } else {
        original.boards[piece] |= (1ULL << m.toSquare);
        original.zobristHash ^= zobristKeys[piece * 64 + m.toSquare];
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
        if (!whiteMoved){  // black captures white's pawn
            capturedSquare = m.toSquare + 8;
            original.boards[W_PAWN] ^= (1ULL << capturedSquare);
            original.zobristHash ^= zobristKeys[W_PAWN * 64 + capturedSquare];
        } else {                    // white captures black's pawn
            capturedSquare = m.toSquare - 8;
            original.boards[B_PAWN] ^= (1ULL << capturedSquare);
            original.zobristHash ^= zobristKeys[B_PAWN * 64 + capturedSquare];
        }
    }


    // updating board state
    if (((piece == 0) || (piece == 6)) || capture || promo){
        original.halfMoveClock = 0;
    }
    if (!whiteMoved) original.fullMoveNumber++;


    // updating Zobrist hash
    original.zobristHash ^= zobristKeys[768];   // xor sidetomove regardless
    return original;
}


bool isInCheck(const Board& b, bool whiteKingInCheck){
    Board temp = b;
    Bitboard occupied = temp.occupiedSquares();
    std::uint64_t empty = ~(occupied.board);
    Bitboard whites = temp.whitePieces();
    Bitboard blacks = temp.blackPieces();

    if (whiteKingInCheck){
        std::uint64_t blackRooks = temp.boards[B_ROOK].board;
        std::uint64_t blackBishops = temp.boards[B_BISHOP].board;
        std::uint64_t blackQueens = temp.boards[B_QUEEN].board;
        std::uint64_t blackKing = temp.boards[B_KING].board;
        std::uint64_t bpCaptures = bPawnCaptures(temp.boards[B_PAWN].board, whites.board);
        std::uint64_t bnCaptures = knightAttacks(temp.boards[B_KNIGHT].board, whites.board, empty);

        std::uint64_t brCaptures = 0ULL;
        while(blackRooks != 0){
            int rookIndex = popLSB(&blackRooks);
            brCaptures |= rookAttacks(rookIndex, occupied.board, blacks.board);
        }

        std::uint64_t bbCaptures = 0ULL;
        while(blackBishops != 0){
            int bishopIndex = popLSB(&blackBishops);
            bbCaptures |= bishopAttacks(bishopIndex, occupied.board, blacks.board);
        }

        std::uint64_t bqCaptures = 0ULL;
        while (blackQueens != 0){
            int queenIndex = popLSB(&blackQueens);
            bqCaptures |= queenAttacks(queenIndex, occupied.board, blacks.board);
        }

        std::uint64_t bkCaptures = 0ULL;
        if (blackKing != 0){
            int kingIndex = popLSB(&blackKing);
            bkCaptures = kingAttackTables[kingIndex];
        }

        std::uint64_t blackCaptures = bpCaptures | brCaptures | bbCaptures | bqCaptures | bnCaptures | bkCaptures;
        return (blackCaptures & temp.boards[W_KING].board) != 0;
    } else {
        std::uint64_t wpCaptures = wPawnCaptures(temp.boards[W_PAWN].board, blacks.board);
        std::uint64_t wnCaptures = knightAttacks(temp.boards[W_KNIGHT].board, blacks.board, empty);
        std::uint64_t whiteRooks = temp.boards[W_ROOK].board;
        std::uint64_t whiteBishops = temp.boards[W_BISHOP].board;
        std::uint64_t whiteQueens = temp.boards[W_QUEEN].board;
        std::uint64_t whiteKing = temp.boards[W_KING].board;

        std::uint64_t wrCaptures = 0ULL;
        while(whiteRooks != 0){
            int rookIndex = popLSB(&whiteRooks);
            wrCaptures |= rookAttacks(rookIndex, occupied.board, whites.board);
        }
        
        std::uint64_t wbCaptures = 0ULL;
        while(whiteBishops != 0){
            int bishopIndex = popLSB(&whiteBishops);
            wbCaptures |= bishopAttacks(bishopIndex, occupied.board, whites.board);
        }

        std::uint64_t wqCaptures = 0ULL;
        while (whiteQueens != 0){
            int queenIndex = popLSB(&whiteQueens);
            wqCaptures |= queenAttacks(queenIndex, occupied.board, whites.board);
        }

        std::uint64_t wkCaptures = 0ULL;
        if (whiteKing != 0){
            int kingIndex = popLSB(&whiteKing);
            wkCaptures = kingAttackTables[kingIndex];
        }


        std::uint64_t whiteCaptures = wpCaptures | wrCaptures | wbCaptures | wqCaptures | wnCaptures | wkCaptures;
        return (whiteCaptures & temp.boards[B_KING].board) != 0;
    }
}


MoveList generateLegalMoves(const Board &b){
    MoveList list = {};

    // helper variables
    bool whiteTurn = b.whiteToMove;
    Board temp = b;
    Bitboard occupied = temp.occupiedSquares();
    std::uint64_t empty = ~(occupied.board);
    Bitboard whites = temp.whitePieces();
    Bitboard blacks = temp.blackPieces();


    int sideOffset = whiteTurn ? 0 : 6;
    std::uint64_t ownPieces = whiteTurn ? whites.board : blacks.board;
    std::uint64_t enemyPieces = whiteTurn ? blacks.board : whites.board;

    for (int p = 0; p <= 5; p++){
        int pieceType = p + sideOffset;
        std::uint64_t pieceBoard = temp.boards[pieceType].board;

        while (pieceBoard != 0ULL){
            int fromSquare = popLSB(&pieceBoard);
            std::uint64_t singlePawnMask = (1ULL << fromSquare);
            std::uint64_t attackBoard = 0ULL;


            if (p == 0){    // pawn
                std::uint64_t singlePushBoard = 0ULL;
                std::uint64_t doublePushBoard = 0ULL;
                std::uint64_t captureBoard = 0ULL;
                const std::uint64_t rank4 = 0x00000000FF000000ULL;
                const std::uint64_t rank5 = 0x000000FF00000000ULL;


                if (whiteTurn){
                    captureBoard = wPawnCaptures(singlePawnMask, enemyPieces);
                    singlePushBoard = empty & shiftNorth(singlePawnMask);
                    doublePushBoard = empty & shiftNorth(singlePushBoard) & rank4;

                    if (b.enPassantSquare != -1) {
                        std::uint64_t enpassantMask = (1ULL << b.enPassantSquare);
                        captureBoard |= wPawnCaptures(singlePawnMask, enpassantMask);
                    }
                } else {
                    captureBoard = bPawnCaptures(singlePawnMask, enemyPieces);
                    singlePushBoard = empty & shiftSouth(singlePawnMask);
                    doublePushBoard = empty & shiftSouth(singlePushBoard) & rank5;

                    // Check for En Passant using your capture function rules
                    if (b.enPassantSquare != -1) {
                        std::uint64_t enpassantMask = (1ULL << b.enPassantSquare);
                        captureBoard |= bPawnCaptures(singlePawnMask, enpassantMask);
                    }
                }


                // processing into moves
                while (singlePushBoard != 0ULL){
                    int toSquare = popLSB(&singlePushBoard);
                    int rank = toSquare / 8;
                    if ((whiteTurn && rank == 7) || (!whiteTurn && rank == 0)){
                        addMoveIfLegal(b, list, Move{fromSquare, toSquare, pieceType, PROMO_QUEEN}, whiteTurn);
                        addMoveIfLegal(b, list, Move{fromSquare, toSquare, pieceType, PROMO_ROOK}, whiteTurn);
                        addMoveIfLegal(b, list, Move{fromSquare, toSquare, pieceType, PROMO_BISHOP}, whiteTurn);
                        addMoveIfLegal(b, list, Move{fromSquare, toSquare, pieceType, PROMO_KNIGHT}, whiteTurn);
                    } else {
                        addMoveIfLegal(b, list, Move{fromSquare, toSquare, pieceType, 0}, whiteTurn);
                    }
                }


                while (doublePushBoard != 0ULL){
                    int toSquare = popLSB(&doublePushBoard);
                    addMoveIfLegal(b, list, Move{fromSquare, toSquare, pieceType, FLAG_DOUBLE_PUSH}, whiteTurn);
                }


                while (captureBoard != 0ULL){
                    int toSquare = popLSB(&captureBoard);
                    int rank = toSquare / 8;
                    std::uint8_t moveFlag = FLAG_CAPTURE;

                    if (toSquare == b.enPassantSquare){
                        moveFlag = FLAG_EN_PASSANT;
                    }

                    if ((whiteTurn && rank == 7) || (!whiteTurn && rank == 0)){
                        addMoveIfLegal(b, list, Move{fromSquare, toSquare, pieceType, (std::uint8_t)(moveFlag | PROMO_QUEEN)}, whiteTurn);
                        addMoveIfLegal(b, list, Move{fromSquare, toSquare, pieceType, (std::uint8_t)(moveFlag | PROMO_ROOK)}, whiteTurn);
                        addMoveIfLegal(b, list, Move{fromSquare, toSquare, pieceType, (std::uint8_t)(moveFlag | PROMO_BISHOP)}, whiteTurn);
                        addMoveIfLegal(b, list, Move{fromSquare, toSquare, pieceType, (std::uint8_t)(moveFlag | PROMO_KNIGHT)}, whiteTurn);
                    } else {
                        addMoveIfLegal(b, list, Move{fromSquare, toSquare, pieceType, moveFlag}, whiteTurn);
                    }
                }
                continue;
            }

            if (p == 3){    // knight
                attackBoard = knightAttackTables[fromSquare] & (empty | enemyPieces);
            }

            else if (p == 5){    // king 
                attackBoard = kingAttackTables[fromSquare] & (empty | enemyPieces);
            }
    
            else if (p == 2){    // bishop
                attackBoard = bishopAttacks(fromSquare, occupied.board, ownPieces);
            }

            else if (p == 1){    // rook
                attackBoard = rookAttacks(fromSquare, occupied.board, ownPieces);
            }

            else if (p == 4){    // queen
                attackBoard = queenAttacks(fromSquare, occupied.board, ownPieces);
            }

            while (attackBoard != 0ULL){
                int toSquare = popLSB(&attackBoard);
                std::uint8_t moveFlag = 0;
                int capturedType = -1;

                if ((1ULL << toSquare) & enemyPieces){
                    moveFlag = FLAG_CAPTURE;

                    for (int capt = (6 - sideOffset); capt < (12 - sideOffset); capt++){
                        if ((1ULL << toSquare) & temp.boards[capt].board){
                            capturedType = capt;
                            break;
                        }
                    }
                }

                Move m;
                if (capturedType != -1){
                    m = {fromSquare, toSquare, pieceType, moveFlag, capturedType};
                } else {
                    m = {fromSquare, toSquare, pieceType, moveFlag};
                }
                
                Board postMoveBoard = makeMove(b, m);
                if (!isInCheck(postMoveBoard, whiteTurn)){
                    addMove(list, m);
                }
            }
        }
    }


    // handling castling
    if (whiteTurn){
        if (b.castlingRights & 1){  // white kingside
            std::uint64_t pathMask = (1ULL << 5) | (1ULL << 6);
            if ((occupied.board & pathMask) == 0ULL){
                if (!isInCheck(b, true)){
                    Board movingBoard = b;
                    movingBoard.boards[W_KING].board = (1ULL << 5);
                    if (!isInCheck(movingBoard, true)){
                        Move m{4, 6, W_KING, FLAG_CASTLE_KSIDE};
                        addMoveIfLegal(b, list, m, true);
                    }
                }
            }
        }


        if (b.castlingRights & 2){  // white queenside
            std::uint64_t pathMask = (1ULL << 1) | (1ULL << 2) | (1ULL << 3);
            if ((occupied.board & pathMask) == 0ULL){
                if (!isInCheck(b, true)){
                    Board movingBoard = b;
                    movingBoard.boards[W_KING].board = (1ULL << 3);
                    if (!isInCheck(movingBoard, true)){
                        Move m{4, 2, W_KING, FLAG_CASTLE_QSIDE};
                        addMoveIfLegal(b, list, m, true);
                    }
                }
            }
        }
    } else {
        if (b.castlingRights & 4){  // black kingside
            std::uint64_t pathMask = (1ULL << 61) | (1ULL << 62);
            if ((occupied.board & pathMask) == 0ULL){
                if(!isInCheck(b, false)){
                    Board movingBoard = b;
                    movingBoard.boards[B_KING].board = (1ULL << 61);
                    if (!isInCheck(movingBoard, false)){
                        Move m{60, 62, B_KING, FLAG_CASTLE_KSIDE};
                        addMoveIfLegal(b, list, m, false);
                    }
                }
            }
        }


        if (b.castlingRights & 8){
            std::uint64_t pathMask = (1ULL << 57) | (1ULL << 58) | (1ULL << 59);
            if ((occupied.board & pathMask) == 0ULL){
                if(!isInCheck(b, false)){
                    Board movingBoard = b;
                    movingBoard.boards[B_KING].board = (1ULL << 59);
                    if (!isInCheck(movingBoard, false)){
                        Move m{60, 58, B_KING, FLAG_CASTLE_QSIDE};
                        addMoveIfLegal(b, list, m, false);
                    }
                }
            }
        }
    }


    return list;
}


std::uint64_t perft(int depth, const Board &b){
    if (depth == 0) return 1ULL;

    MoveList list = generateLegalMoves(b);
    std::uint64_t nodes = 0;
    for (const Move&m : list){
        Board nextBoard = makeMove(b, m);
        nodes += perft(depth - 1, nextBoard);
    }

    return nodes;
}


/*
TODO
1. logic for handling if squares are checked when king passes through when castling
2. test perft (should return 1, 20, 400, 8902, 197281, 4865609)
*/