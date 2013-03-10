#include "movegen.h"

void init_movegen(void)
{
    init_kinglocs();
    init_knightlocs();
    init_rankmasks();
    init_filemasks();
    init_a1h8masks();
    init_a8h1masks();
    init_slidingmoves();
}

void init_kinglocs(void) 
{
    int i;

    for (i = 0; i < 64; i++)
    {
        int rank;
        int file;
        rank = i / 8;
        file = i % 8;

        king_movelocs[i] = 0;

        if (file > 0)
        {
            if (rank > 0)
                king_movelocs[i] |= 1ULL << ((rank - 1) * 8 + file - 1);

            king_movelocs[i] |= 1ULL << (rank * 8 + file - 1);

            if (rank < 7)
                king_movelocs[i] |= 1ULL << ((rank + 1) * 8 + file - 1);
        }

        if (rank > 0)
            king_movelocs[i] |= 1ULL << ((rank - 1) * 8 + file);

        if (rank < 7)
            king_movelocs[i] |= 1ULL << ((rank + 1) * 8 + file);

        if (file < 7)
        {
            if (rank > 0)
                king_movelocs[i] |= 1ULL << ((rank - 1) * 8 + file + 1);

            king_movelocs[i] |= 1ULL << (rank * 8 + file + 1);

            if (rank < 7)
                king_movelocs[i] |= 1ULL << ((rank + 1) * 8 + file + 1);
        }
    }
}

void init_knightlocs(void)
{
    int i;

    for (i = 0; i < 64; i++)
    {
        int rank;
        int file;
        rank = i / 8;
        file = i % 8;

        knight_movelocs[i] = 0;

        if (file > 0)
        {
            if (rank > 1)
                knight_movelocs[i] |= 1ULL << ((rank - 2) * 8 + file - 1);
            if (rank < 6)
                knight_movelocs[i] |= 1ULL << ((rank + 2) * 8 + file - 1);
        }

        if (file > 1)
        {
            if (rank > 0)
                knight_movelocs[i] |= 1ULL << ((rank - 1) * 8 + file - 2);
            if (rank < 7)
                knight_movelocs[i] |= 1ULL << ((rank + 1) * 8 + file - 2);
        }

        if (file < 6)
        {
            if (rank > 0)
                knight_movelocs[i] |= 1ULL << ((rank - 1) * 8 + file + 2);
            if (rank < 7)
                knight_movelocs[i] |= 1ULL << ((rank + 1) * 8 + file + 2);
        }

        if (file < 7)
        {
            if (rank > 1)
                knight_movelocs[i] |= 1ULL << ((rank - 2) * 8 + file + 1);
            if (rank < 6)
                knight_movelocs[i] |= 1ULL << ((rank + 2) * 8 + file + 1);
        }
    }
}

void init_rankmasks(void)
{
    int i;

    for (i = 0; i < 64; i++)
    {
        int rank;
        int file;
        rank = i / 8;
        file = i % 8;

        // note that these only includes bits (1..6) of the rank; they do not mask the
        // "edge" bits (they omit the 0th and 7th bit of the rank)
        rankmask[i]  = to_bitboard(rank, 1) | to_bitboard(rank, 2) | to_bitboard(rank, 3);
        rankmask[i] |= to_bitboard(rank, 4) | to_bitboard(rank, 5) | to_bitboard(rank, 6);
    }
}

void init_filemasks(void)
{
    int i;

    for (i = 0; i < 64; i++)
    {
        int rank;
        int file;
        rank = i / 8;
        file = i % 8;

        // same story as rank masks
        filemask[i]  = to_bitboard(1, file) | to_bitboard(2, file) | to_bitboard(3, file);
        filemask[i] |= to_bitboard(4, file) | to_bitboard(5, file) | to_bitboard(6, file);
    }
}

void init_a1h8masks(void)
{
    int i;

    for (i = 0; i < 64; i++)
    {
        int rank;
        int file;
        int diag;

        rank = i / 8;
        file = i % 8;
        diag = file - rank; // ranges from -7 to 7

        a1h8mask[i] = 0;

        if (diag >= 0)
        {
            int j;
            for (j = 1; j < 7 - diag; j++)
                a1h8mask[i] |= to_bitboard(j, diag + j);
        }
        else
        {
            int j;
            for (j = 1; j < 7 + diag; j++)
                a1h8mask[i] |= to_bitboard(j - diag, j);
        }
    }
}

void init_a8h1masks(void)
{
    int i;

    for (i = 0; i < 64; i++)
    {
        int rank;
        int file;
        int diag;

        rank = i / 8;
        file = i % 8;
        diag = file + rank;

        a8h1mask[i] = 0;

        if (diag < 8)
        {
            int j;
            for (j = 1; j < diag; j++)
                a8h1mask[i] |= to_bitboard(diag - j, j);
        }
        else // TODO: Do this, minus the weird for-loop logic
        {
            int j;
            for (j = 2; j < 15 - diag; j++)
                a8h1mask[i] |= to_bitboard(diag + j - 8, 8 - j);
        }
    }
}

void init_slidingmoves(void)
{
    int i;
    unsigned char charmask[8];

    charmask[0] = 1;
    for (i = 1; i < 8; i++)
        charmask[i] = charmask[i - 1] << 1;

    for (i = 0; i < 8; i++)
    {
        unsigned char state6b; // 6 bits representing occupancy

        for (state6b = 0; state6b < 64; state6b++)
        {
            unsigned char state8b;
            unsigned char attacks;
            unsigned char slide;

            // the 6-bit one goes from (1..6); this one
            // goes from (0..7).
            state8b = state6b << 1;
            attacks = 0;

            sliding_moves[i][state6b] = 0;

            if (i < 7)
                attacks |= charmask[i + 1];

            // TODO: Implement this with do..while loops

            slide = i + 2;
            while (slide <= 7)
            {
                if ((~state8b) & charmask[slide - 1])
                    attacks |= charmask[slide];
                else 
                    break;
                
                slide++;
            
            }

            if (i > 0)
                attacks |= charmask[i - 1];
            
            slide = i - 2;
            while (slide >= 0)
            {
                if ((~state8b) & charmask[slide + 1])
                    attacks |= charmask[slide];
                else 
                    break;
                
                slide--;
            }

            sliding_moves[i][state6b] = attacks;
        }
    }
}