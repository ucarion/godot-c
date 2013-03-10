#include "utils.h"

U64 to_bitboard(int rank, int file)
{
    return 1ULL << (rank * 8 + file);
}