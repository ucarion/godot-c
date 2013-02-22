#ifndef MOVE_H
#define MOVE_H

typedef int MOVE_t;

#define MOVE_PAWN 0
#define MOVE_KNIGHT 1
#define MOVE_BISHOP 2
#define MOVE_ROOK 3
#define MOVE_QUEEN 4
#define MOVE_KING 5

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