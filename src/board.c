#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "move.h"

BOARD_t* new_empty_board()
{
    BOARD_t* board = malloc(sizeof(BOARD_t));

    board->white_pawns = 0x0ULL;
    board->white_knights = 0x0ULL;
    board->white_bishops = 0x0ULL;
    board->white_rooks = 0x0ULL;
    board->white_queens = 0x0ULL;
    board->white_king = 0x0ULL;

    board->black_pawns = 0x0ULL;
    board->black_knights = 0x0ULL;
    board->black_bishops = 0x0ULL;
    board->black_rooks = 0x0ULL;
    board->black_queens = 0x0ULL;
    board->black_king = 0x0ULL;

    board->white_castle_k = 0;
    board->white_castle_q = 0;
    board->black_castle_k = 0;
    board->black_castle_q = 0;

    board->move_number = 0;
    board->white_to_move = 0;
    board->fifty_move_rule = 0;
    board->en_passant_loc = -1;
}

// Creates a new board from a passed _properly-formatted_ FEN string.
BOARD_t* new_board(char fen[])
{
    int up;
    int out;
    int i;
    BOARD_t* board;

    board = new_empty_board();

    i = 0;

    // the piece locations
    for (up = 7; up >= 0; up--)
    {
        for (out = 0; out < 8; out++)
        {
            if (fen[i] == '/')
                i++;

            if (isdigit(fen[i]))
                out += fen[i] - '0';
            else 
            {
                U64 sq = 0x1ULL << (up * 8 + out);
                switch (fen[i])
                {
                    case 'P':
                        board->white_pawns |= sq;
                        break;
                    case 'N':
                        board->white_knights |= sq;
                        break;
                    case 'B':
                        board->white_bishops |= sq;
                        break;
                    case 'R':
                        board->white_rooks |= sq;
                        break;
                    case 'Q':
                        board->white_queens |= sq;
                        break;
                    case 'K':
                        board->white_king |= sq;
                        break;
                    case 'p':
                        board->black_pawns |= sq;
                        break;
                    case 'n':
                        board->black_knights |= sq;
                        break;
                    case 'b':
                        board->black_bishops |= sq;
                        break;
                    case 'r':
                        board->black_rooks |= sq;
                        break;
                    case 'q':
                        board->black_queens |= sq;
                        break;
                    case 'k':
                        board->black_king |= sq;
                        break;
                }
            }
            i++;
        }
    }

    // color to move
    i++;
    board->white_to_move = (fen[i] == 'w' ? 1 : 0);

    // castling rights
    i += 2;
    if (fen[i] == 'K')
    {
        board->white_castle_k = 1;
        i++;
    }    if (fen[i] == 'Q')
    {
        board->white_castle_q = 1;
        i++;
    }
    if (fen[i] == 'k')
    {
        board->black_castle_k = 1;
        i++;
    }
    if (fen[i] == 'q')
    {
        board->black_castle_q = 1;
        i++;
    }

    // en passant
    i++;
    if (fen[i] == '-')
        board->en_passant_loc = -1;
    else {
        board->en_passant_loc = alg_loc_to_int(fen[i], fen[i + 1]);
        i++;
    }

    // halfmove clock
    i += 2;
    if (isdigit(fen[i + 1]))
    {
        board->fifty_move_rule = 10 * (fen[i] - '0') + fen[i + 1] - '0';
        i++;
    }
    else
        board->fifty_move_rule = fen[i] - '0';

    // move number
    i += 2;
    char num[strlen(fen) - i + 1];
    {
        int j;
        for (j = i; j < strlen(fen); j++)
            num[j - i] = fen[j];
    }

    num[strlen(fen) - i] = '\0';
    
    board->move_number = atoi(num);

    return board;
}

char alg_loc_to_int(char file, char rank)
{
    return 8 * (rank - '1') + (file - 'a');
}

void print_bitboard(U64 board)
{
    int up;
    int out;

    printf("     1   2   3   4   5   6   7   8  \n");
    printf("   +---+---+---+---+---+---+---+---+\n");
    for (up = 7; up >= 0; up--)
    {
        printf(" %c |", 'a' + up);
        for (out = 0; out < 8; out++)
        {
            U64 sq = 0x1ULL << (up * 8 + out);
            printf(" %s |", board & sq ? "X" : " ");
        }
        printf(" %c \n", 'a' + up);
        printf("   +---+---+---+---+---+---+---+---+\n");
    }
    printf("     1   2   3   4   5   6   7   8  \n");
}

void print_board(BOARD_t* board)
{
    int up;
    int out;

    printf("     1   2   3   4   5   6   7   8  \n");
    printf("   +---+---+---+---+---+---+---+---+\n");
    for (up = 7; up >= 0; up--)
    {
        printf(" %c |", 'a' + up);
        for (out = 0; out < 8; out++)
        {
            U64 sq = 0x1ULL << (up * 8 + out);
            printf(" ");
            if (board->white_pawns & sq)
                printf("P");
            else if (board->white_knights & sq)
                printf("N");
            else if (board->white_bishops & sq)
                printf("B");
            else if (board->white_rooks & sq)
                printf("R");
            else if (board->white_queens & sq)
                printf("Q");
            else if (board->white_king & sq)
                printf("K");
            else if (board->black_pawns & sq)
                printf("p");
            else if (board->black_knights & sq)
                printf("n");
            else if (board->black_bishops & sq)
                printf("b");
            else if (board->black_rooks & sq)
                printf("r");
            else if (board->black_queens & sq)
                printf("q");
            else if (board->black_king & sq)
                printf("k");
            else
                printf(" ");

            printf(" |");
        }
        printf(" %c \n", 'a' + up);
        printf("   +---+---+---+---+---+---+---+---+\n");
    }
    printf("     1   2   3   4   5   6   7   8  \n");

    printf("%s to move.\n", board->white_to_move ? "White" : "Black");
    printf("White can castle: %s %s\n", board->white_castle_k ? "O-O" : "", board->white_castle_q ? "O-O-O" : "");
    printf("Black can castle: %s %s\n", board->black_castle_k ? "O-O" : "", board->black_castle_q ? "O-O-O" : "");
    printf("En passant: %d\n", board->en_passant_loc);
    printf("Fifty move rule: %d\n", board->fifty_move_rule);
    printf("Move number: %d\n", board->move_number);
}