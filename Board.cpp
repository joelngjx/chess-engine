#include "Board.h"


// constructor (empty cuz we already initialise with value 0ULL for default Bitboard)
Board::Board() {}

// static factory method to construct starting position for all 12 piecetypes
Board Board::startingPosition(){
    Board b;
    b.boards[W_PAWN] = 0x000000000000FF00ULL;
    b.boards[W_ROOK] = 0x0000000000000081ULL;
    b.boards[W_BISHOP] = 0x0000000000000024ULL;
    b.boards[W_KNIGHT] = 0x0000000000000042ULL;
    b.boards[W_QUEEN] = 0x0000000000000008ULL;
    b.boards[W_KING] = 0x0000000000000010ULL;
    b.boards[B_PAWN] = 0x00FF000000000000ULL;
    b.boards[B_ROOK] = 0x8100000000000000ULL;
    b.boards[B_BISHOP] = 0x2400000000000000ULL;
    b.boards[B_KNIGHT] = 0x4200000000000000ULL;
    b.boards[B_QUEEN] = 0x0800000000000000ULL;
    b.boards[B_KING] = 0x1000000000000000ULL;
    return b;
}


Board Board::parseFEN(std::string fen){
    Board b;
    std::stringstream full(fen);
    std::vector<std::string> fields;
    std::string field;

    std::vector<std::string> ranks;
    std::string rank;


    // separation by fields
    while (std::getline(full, field, ' ')){
        fields.push_back(field);
    }


    std::stringstream r(fields[0]);
    

    // separation by ranks
    while (std::getline(r, rank, '/')){
        ranks.push_back(rank);
    }

    std::reverse(ranks.begin(), ranks.end());


    // parsing piecetypes and digits
    for (int rank = 0; rank < 8; rank++){
        int file = 0;
        std::string rankString = ranks[rank];
        

        for (char c: rankString){
            if (std::isdigit(c)){
                file += (c - '0');
                continue;
            } else {
                int pieceType = -1;
                switch(c){
                // White Pieces
                    case 'P': pieceType = W_PAWN; break; 
                    case 'R': pieceType = W_ROOK; break; 
                    case 'B': pieceType = W_BISHOP; break;
                    case 'N': pieceType = W_KNIGHT; break;
                    case 'Q': pieceType = W_QUEEN; break;
                    case 'K': pieceType = W_KING; break;

                    // Black Pieces
                    case 'p': pieceType = B_PAWN; break;
                    case 'r': pieceType = B_ROOK; break;
                    case 'b': pieceType = B_BISHOP; break;
                    case 'n': pieceType = B_KNIGHT; break;
                    case 'q': pieceType = B_QUEEN; break;
                    case 'k': pieceType = B_KING; break;
                    
                    default: break;
                }

                if (pieceType != -1){
                    b.boards[pieceType] |= (1ULL << (8*rank+file));   
                    file++;
                }
            }
        }
    }

    if (fields[1] == "w"){
        b.whiteToMove = true;
    } else {
        b.whiteToMove = false;
    }


    b.castlingRights = 0ULL;
    for (char c: fields[2]){
        if (c == 'K') {b.castlingRights |= 1;} 
        else if (c == 'Q') {b.castlingRights |= 2;}
        else if (c == 'k') {b.castlingRights |= 4;}
        else if (c == 'q') {b.castlingRights |= 8;}
    }


    std::string enPassant = fields[3];
    if (enPassant == "-"){
        b.enPassantSquare = -1;
    } else {
        int file = enPassant[0] - 'a';
        int rank = enPassant[1] - '1';
        int index = 8*rank + file;
        b.enPassantSquare = index;
    }


    b.halfMoveClock = std::stoi(fields[4]);
    b.fullMoveNumber = std::stoi(fields[5]);
    

    return b;
}


bool Board::isValid() const {
    // king count
    if (__popcnt64(boards[W_KING].board) != 1) return false;
    if (__popcnt64(boards[B_KING].board) != 1) return false;


    // no pawns on ranks 1,8
    const uint64_t ranks_1_8 = 0xFF000000000000FFULL;
    if((boards[W_PAWN].board & ranks_1_8) != 0) return false;
    if((boards[B_PAWN].board & ranks_1_8) != 0) return false;

    // popcount bounds, overlap check, material limit
    uint64_t seenMask = 0ULL;
    int wpCount = 0;
    int bpCount = 0;

    for (int i = W_PAWN; i <= B_KING; i++){
        uint64_t currentBoard = boards[i].board;
        int currentCount = static_cast<int>(__popcnt64(currentBoard));

        if ((seenMask & currentBoard) != 0){
            return false;
        }

        seenMask |= currentBoard;

        if (i <= W_KING){
            wpCount++;
        } else {
            bpCount++;
        }


        // individual piece checks
        if (i == W_PAWN || i == B_PAWN){
            if (currentCount > 8) return false;
        } else if (i == W_QUEEN || i == B_QUEEN){
            if (currentCount > 9) return false;
        } else if (i != W_KING || i != B_KING){
            if (currentCount > 10) return false;
        }

        if (wpCount > 16 || bpCount > 16) return false;

        return true;
    }

    return true;
}


void Board::printBoardState() const {
    // printing hexes of all bitboards
    // std::left => left justified
    std::string piecenames[12] = {
        "W_PAWN",
        "W_ROOK",
        "W_BISHOP",
        "W_KNIGHT",
        "W_QUEEN",
        "W_KING",
        "B_PAWN",
        "B_ROOK",
        "B_BISHOP",
        "B_KNIGHT",
        "B_QUEEN",
        "B_KING"
    };


    std::cout << "--------- BOARD STATE ---------" << "\n";
    for (int i = W_PAWN; i <= B_KING; i++){
        std::cout << piecenames[i] << "\n";
        displayBoard(boards[i].board);
    }

    
    // side to move
    std::cout << "\n" << "Side to move       : " << (whiteToMove ? "White" : "Black") << "\n";


    // castling rights
    std::cout << "Castling Rights     : ";
    if (castlingRights == 0) std::cout << "-"; 
    if (castlingRights & 1) std::cout << "K"; 
    if (castlingRights & 2) std::cout << "Q"; 
    if (castlingRights & 4) std::cout << "k"; 
    if (castlingRights & 8) std::cout << "q";
    std::cout << "\n";


    // en passant
    std::cout << "En Passant Square   : ";
    if (enPassantSquare == -1){
        std::cout << "None" << "\n";
    } else {
        char file = 'a' + (enPassantSquare % 8);
        char rank = '1' + (enPassantSquare / 8);
        std::cout << file << rank << "\n";
    }


    // half move clock, full move number, zobrist hash
    std::cout << "Half-Move Clock     : " << halfMoveClock << "\n";
    std::cout << "Full-Move Number    : " << fullMoveNumber << "\n";
    
    std::cout << "Zobrist Hash        : ";
    printHex(zobristHash);
}



Bitboard Board::whitePieces(){
    Bitboard temp;
    for (int white = W_PAWN; white <= W_KING; white++){
        temp.board |= boards[white].board;
    }

    return temp;
};


Bitboard Board::blackPieces(){
    Bitboard temp;
    for (int black = B_PAWN; black <= B_KING; black++){
        temp.board |= boards[black].board;
    }

    return temp;
};


Bitboard Board::occupiedSquares(){
    Bitboard occupied;
    occupied.board = (whitePieces() | blackPieces()).board;
    return occupied;
}


Bitboard Board::emptySquares(){
    Bitboard empty;
    empty.board = (~occupiedSquares().board);
    return empty;
}


void printHex(std::uint64_t value){
    std::cout << "0x" << std::setfill('0') << std::setw(16) << std::hex << value << std::dec << "\n";
}


/*
    int main(){
        // BOARD STRUCT 
        Board allBoards = Board::startingPosition();
        
        for (int z = W_PAWN; z <= B_KING; z++){
            displayBoard(allBoards.boards[z].board);
        } 

        Bitboard whites = allBoards.whitePieces();
        Bitboard blacks = allBoards.blackPieces();
        Bitboard occupied = allBoards.occupiedSquares();
        Bitboard empty = allBoards.emptySquares();

        displayBoard(whites);
        displayBoard(blacks);
        displayBoard(occupied);
        displayBoard(empty);

        return 0;
    }
*/