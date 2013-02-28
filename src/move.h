#ifndef MOVE_H
#define MOVE_H

typedef int MOVE_t;

#define MOVE_WHITE_PAWN 0
#define MOVE_BLACK_PAWN 1
#define MOVE_WHITE_KNIGHT 2
#define MOVE_BLACK_KNIGHT 3
#define MOVE_WHITE_BISHOP 4
#define MOVE_BLACK_BISHOP 5
#define MOVE_WHITE_ROOK 6
#define MOVE_BLACK_ROOK 7
#define MOVE_WHITE_QUEEN 8
#define MOVE_BLACK_QUEEN 9
#define MOVE_WHITE_KING 10
#define MOVE_BLACK_KING 11

#define MOVE_FLAG_NO_FLAG 0
#define MOVE_FLAG_OO 1
#define MOVE_FLAG_OOO 2
#define MOVE_FLAG_EP 3
#define MOVE_FLAG_PROMO_KNIGHT 4
#define MOVE_FLAG_PROMO_BISHOP 5
#define MOVE_FLAG_PROMO_ROOK 6
#define MOVE_FLAG_PROMO_QUEEN 7

MOVE_t new_move(char, char, char, bool, char);
char get_from(MOVE_t);
char get_to(MOVE_t);
char get_piece(MOVE_t);
bool is_capture(MOVE_t);
char get_flag(MOVE_t);

#endif