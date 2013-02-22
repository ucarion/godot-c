#include <stdio.h>
#include <stdbool.h>
#include "move.h"

// A move is represented in bits, which are allocated as follows:
//   101 | 0 | 101 | 010101 | 010101 |
//   flg | c | typ |   to   |  from  |
// Where those parameters are below.
MOVE_t new_move(char from, char to, char piece, bool capture, char flag) 
{
    return (from) | (to << 6) | (piece << 12) | ( (capture ? 1 : 0) << 15) | (flag << 16);
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
    return (move >> 12) & 0x7;
}

bool is_capture(MOVE_t move)
{
    return ( (move >> 15) & 0x1) == 1;
}

char get_flag(MOVE_t move)
{
    return (move >> 16) & 0x7;
}