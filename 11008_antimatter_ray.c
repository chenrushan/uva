/*
 * uva 11008
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NTREES 16
#define INF MAX_NTREES + 1
#define min(x, y) ((x) > (y) ? (y) : (x))

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

struct t_tree {
    short x, y;
};

uint8_t nbits[1 << MAX_NTREES]; /* nbits[i] = number of bits in i */

struct t_tree trees[MAX_NTREES]; /* all trees */
short ntrees; /* number of trees */
short ncuts; /* number of trees to cut */

uint16_t rays[MAX_NTREES * (MAX_NTREES + 1)]; /* all possible rays */
uint16_t nrays; /* number of all possible rays */

uint8_t T[2][1 << MAX_NTREES];

void
fill()
{
    int r = 0, v = 0;
    int last = (nrays & 1), cur = 0;

    for (v = 0; v < (1 << ntrees); ++v) {
        T[last][v] = INF;
        if (nbits[v] == ncuts) {
            T[last][v] = 0;
        }
    }

    for (r = nrays - 1; r >= 0; --r) {
        last = ((r + 1) & 1);
        cur = (r & 1);

        for (v = 0; v < (1 << ntrees); ++v) {
            int nr1 = INF, nr2;
            if (nbits[v | rays[r]] <= ncuts) {
                nr1 = 1 + T[last][v | rays[r]];
            }
            nr2 = T[last][v];
            T[cur][v] = min(nr1, nr2);
        }
    }
}

int
cmp_tree(struct t_tree *t1, struct t_tree *t2)
{
    if (t1->x == t2->x)
        return t1->y - t2->y;
    return t1->x - t2->x;
}

void
create_rays()
{
    int i = 0, j = 0, k = 0;
    int irays = 0;

    /* insertion sort trees by (x, y) */
    for (i = 1; i < ntrees; ++i) {
        /* j points to the 1st one smaller than trees[i] */
        for (j = i - 1; j >= 0; --j) {
            if (cmp_tree(trees + i, trees + j) > 0) {
                break;
            }
        }
        struct t_tree tmp = trees[i];
        memmove(trees + j + 2, trees + j + 1, sizeof(trees[0]) * (i - j - 1));
        trees[j + 1] = tmp;
    }

    memset(rays, 0, sizeof(rays));
    /* for rays cutting only one tree */
    for (i = 0; i < ntrees; ++i) {
        rays[irays] |= (1 << i);
        irays += 1;
    }
    /* for rays cutting a sequence of trees */
    for (i = 0; i < ntrees; ++i) {
        uint16_t mask = 0;
        for (j = i + 1; j < ntrees; ++j) {
            if ((mask & (1 << j)) != 0) {
                continue;
            }
            rays[irays] |= ((1 << i) | (1 << j));
            for (k = j + 1; k < ntrees; ++k) {
                if ((trees[j].x - trees[i].x) * (trees[k].y - trees[j].y) ==
                    (trees[j].y - trees[i].y) * (trees[k].x - trees[j].x)) {
                    rays[irays] |= (1 << k);
                    mask |= (1 << k);
                }
            }
            irays += 1;
        }
    }
    nrays = irays;
}

int
init_nbits()
{
    int i = 0;
    nbits[0] = 0;
    for (i = 1; i < sizeof(nbits) / sizeof(*nbits); ++i) {
        nbits[i] = 1 + nbits[i & (i - 1)];
    }
}

int
main(int argc, char **argv)
{
    int ncases = 0, c = 0, i = 0;

    init_nbits();
    scanf("%d", &ncases);
    for (c = 0; c < ncases; ++c) {
        /* read input */
        scanf("%hd%hd", &ntrees, &ncuts);
        for (i = 0; i < ntrees; ++i) {
            scanf("%hd%hd", &(trees[i].x), &(trees[i].y));
        }

        create_rays();
        fill();
        printf("Case #%d:\n", c + 1);
        printf("%d\n", T[0][0]);
        if (c != ncases - 1) {
            printf("\n");
        }
    }

    return 0;
}

