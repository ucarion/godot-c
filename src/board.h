#ifndef BOARD_H
#define BOARD_H

#define WHITE 0
#define BLACK 1

#include "move.h"

typedef unsigned long long U64;

typedef struct
{
    U64 white_pawns;
    U64 white_knights;
    U64 white_bishops;
    U64 white_rooks;
    U64 white_queens;
    U64 white_king;

    U64 black_pawns;
    U64 black_knights;
    U64 black_bishops;
    U64 black_rooks;
    U64 black_queens;
    U64 black_king;

    U64 white_pieces;
    U64 black_pieces;
    U64 all_pieces;

    int move_number;

    char white_castle_k;
    char white_castle_q;
    char black_castle_k;
    char black_castle_q;

    char white_to_move;

    char fifty_move_rule;

    char en_passant_loc;
} BOARD_t;

BOARD_t* new_board(char[]);
void print_bitboard(U64);
void print_board(BOARD_t*);
char alg_loc_to_int(char,char);
void update_special_bitboards(BOARD_t*);
void make_move(BOARD_t*,MOVE_t*);

#endif