#include "move.h"

// A move is represented in bits, which are allocated as follows:
//  101 | 0101 | 0101 | 010101 | 010101 |
//  flg | capt | type |   to   |  from  |
// Where those parameters are below.
MOVE_t new_move(char from, char to, char piece, char capture, char flag) 
{
    return (from) | (to << 6) | (piece << 12) | (capture << 16) | (flag << 20);
}

char get_from(MOVE_t move)
{
    return move & 0x3f;
}

char get_to(MOVE_t move)
{
    return (move >> 6) & 0x3f;
}

char get_piece(MOVE_t move)
{
    return (move >> 12) & 0xf;
}

char get_capture(MOVE_t move)
{
    return (move >> 16) & 0xf;
}

char get_flag(MOVE_t move)
{
    return (move >> 20) & 0x7;
}

bool is_promotion(MOVE_t move)
{
    return get_flag(move) >= MOVE_FLAG_PROMO_KNIGHT;
}