#include "Board.h"
#include<random>

extern std::array<std::uint64_t, 781> zobristKeys;
void initZobrist();
std::uint64_t computeHash(const Board& b);