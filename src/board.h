#ifndef BOARD_H
#define BOARD_H

#define U64 unsigned long long;

struct board
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

    char white_to_move;

    char fifty_move_rule;

    char en_passant_loc;
};

#endif