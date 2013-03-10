#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board.h"
#include "utils.h"

U64 king_movelocs[64];
U64 knight_movelocs[64];

U64 rankmask[64];
U64 filemask[64];
U64 a1h8mask[64];
U64 a8h1mask[64];

void init_movegen(void);
void init_kinglocs(void);
void init_knightlocs(void);
void init_rankmasks(void);
void init_filemasks(void);
void init_a1h8masks(void);
void init_a8h1masks(void);

#endif