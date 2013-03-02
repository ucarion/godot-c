#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "board.h"

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
                out += fen[i] - '1';
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

    update_special_bitboards(board);

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
    printf("\n");
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
    printf("\n");
}

void update_special_bitboards(BOARD_t* board)
{
    board->white_pieces = 
        board->white_pawns | board->white_knights | board->white_bishops | board->white_rooks | board->white_queens | board->white_king;

    board->black_pieces = 
        board->black_pawns | board->black_knights | board->black_bishops | board->black_rooks | board->black_queens | board->black_king;

    board->all_pieces = board->white_pieces | board->black_pieces;
}

void make_move(BOARD_t* board, MOVE_t move)
{
    char from;
    char to;
    U64 from_mask;
    U64 to_mask;
    char piece;
    bool is_capt;
    char flag;
    U64 move_mask;

    from = get_from(move);
    to = get_to(move);
    from_mask = 1ULL << from;
    to_mask = 1ULL << to;
    piece = get_piece(move);
    is_capt = is_capture(move);
    flag = get_flag(move);
    move_mask = from_mask | to_mask;
    
    if (!board->white_to_move)
        board->move_number++;

    board->fifty_move_rule++;

    if (is_capt)
    {
        char to_remove;
        U64 to_remove_mask;

        to_remove = to;

        if (flag == MOVE_FLAG_EP)
            to_remove = board->white_to_move ? to - 8 : to + 8;

        to_remove_mask = 1ULL << to_remove;

        board->white_pawns &= ~to_remove_mask;
        board->white_knights &= ~to_remove_mask;
        board->white_bishops &= ~to_remove_mask;
        board->white_rooks &= ~to_remove_mask;
        board->white_queens &= ~to_remove_mask;

        board->black_pawns &= ~to_remove_mask;
        board->black_knights &= ~to_remove_mask;
        board->black_bishops &= ~to_remove_mask;
        board->black_rooks &= ~to_remove_mask;
        board->black_queens &= ~to_remove_mask;

        board->fifty_move_rule = 0;
    }

    // though this value may be updated later, it is irrelevant now and should be default
    // be resetted.
    board->en_passant_loc = -1;

    switch (piece)
    {
        case MOVE_WHITE_PAWN:
            board->fifty_move_rule = 0;
            if (from + 16 == to)
                board->en_passant_loc = from + 8;

            switch (flag)
            {
                case MOVE_FLAG_PROMO_QUEEN:
                    board->white_queens |= to_mask;
                    board->white_pawns &= ~from_mask;
                    break;
                case MOVE_FLAG_PROMO_KNIGHT:
                    board->white_knights |= to_mask;
                    board->white_pawns &= ~from_mask;
                    break;
                case MOVE_FLAG_PROMO_ROOK:
                    board->white_rooks |= to_mask;
                    board->white_pawns &= ~from_mask;
                    break;
                case MOVE_FLAG_PROMO_BISHOP:
                    board->white_bishops |= to_mask;
                    board->white_pawns &= ~from_mask;
                    break;
                default: // so... not a promotion
                    board->white_pawns ^= move_mask;
                    break;
            }
            break;
        case MOVE_BLACK_PAWN:
            board->fifty_move_rule = 0;
            if (from - 16 == to)
                board->en_passant_loc = from - 8;

            switch (flag)
            {
                case MOVE_FLAG_PROMO_QUEEN:
                    board->black_queens |= to_mask;
                    board->black_pawns &= ~from_mask;
                    break;
                case MOVE_FLAG_PROMO_KNIGHT:
                    board->black_knights |= to_mask;
                    board->black_pawns &= ~from_mask;
                    break;
                case MOVE_FLAG_PROMO_ROOK:
                    board->black_rooks |= to_mask;
                    board->black_pawns &= ~from_mask;
                    break;
                case MOVE_FLAG_PROMO_BISHOP:
                    board->black_bishops |= to_mask;
                    board->black_pawns &= ~from_mask;
                    break;
                default:
                    board->white_pawns ^= move_mask;
                    break;
            }
            break;
        case MOVE_WHITE_KNIGHT:
            board->white_knights ^= move_mask;
            break;
        case MOVE_BLACK_KNIGHT:
            board->black_knights ^= move_mask;
            break;
        case MOVE_WHITE_BISHOP:
            board->white_bishops ^= move_mask;
            break;
        case MOVE_BLACK_BISHOP:
            board->black_bishops ^= move_mask;
            break;
        case MOVE_WHITE_ROOK:
            board->white_rooks ^= move_mask;
            break;
        case MOVE_BLACK_ROOK:
            board->black_rooks ^= move_mask;
            break;
        case MOVE_WHITE_QUEEN:
            board->white_queens ^= move_mask;
            break;
        case MOVE_BLACK_QUEEN:
            board->black_queens ^= move_mask;
            break;
        case MOVE_WHITE_KING:
            if (flag == MOVE_FLAG_OO)
            {
                U64 rook_move_mask = (1ULL << 5) | (1ULL << 7);
                board->white_king = 1ULL << 6;
                board->white_rooks ^= rook_move_mask;
            }
            else if (flag == MOVE_FLAG_OOO)
            {
                U64 rook_move_mask = 1ULL | (1ULL << 3);
                board->white_king = 1ULL << 2;
                board->white_rooks ^= rook_move_mask;
            }
            else
                board->white_king ^= move_mask;
            break;
        case MOVE_BLACK_KING:
            if (flag == MOVE_FLAG_OO)
            {
                U64 rook_move_mask = (1ULL << (5 + 56)) | (1ULL << (7 + 56));
                board->black_king = 1ULL << (6 + 56);
                board->black_rooks ^= rook_move_mask;
            }
            else if (flag == MOVE_FLAG_OOO)
            {
                U64 rook_move_mask = (1ULL << 56) | (1ULL << (3 + 56));
                board->black_king = 1ULL << (2 + 56);
                board->black_rooks ^= rook_move_mask;
            }
            else
                board->black_king ^= move_mask;
            break;
    }

    update_special_bitboards(board);

    if (board->white_to_move)
    {
        if (move_mask & 0x90ULL) // 0x90 = e1 | h1
            board->white_castle_k = 0;
        if (move_mask & 0x11ULL) // 0x11 = e1 | a1
            board->white_castle_q = 0;
    }
    else
    {
        if (move_mask & (0x90ULL << 56))
            board->black_castle_k = 0;
        if (move_mask & (0x11ULL << 56))
            board->black_castle_q = 0;
    }

    board->white_to_move = (board->white_to_move ? 0 : 1);
}