#include "movegen.h"

void init_movegen()
{
    int i;

    for (i = 0; i < 64; i++)
    {
        int rank;
        int file;
        rank = i / 8;
        file = i % 8;

        // kings
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

        // knights
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