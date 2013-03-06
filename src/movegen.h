#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board.h"

U64 king_movelocs[64];
U64 knight_movelocs[64];

void init_movegen();

#endif