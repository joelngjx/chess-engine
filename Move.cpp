#include "Move.h"

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