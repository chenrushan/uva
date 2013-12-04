/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/* --------------------------------------------------------------------------- */
/*                        bitflag implementation                               */
/* --------------------------------------------------------------------------- */

static int char2idx[256];       /* map a char value to the position where
                                   0 first appears */
struct bitflag {
        char *bits;             /* a char array */
        int nbits;
        int sz;                 /* size of bits char array */
};

struct bitflag *
bitflag_new(int nbits)
{
        int sz = 0, i = 0, j = 0;
        struct bitflag *bf = NULL;

        sz = nbits >> 3;
        if ((nbits & 0x07) != 0)
                sz++;
        bf = calloc(1, sizeof(*bf) + sz);
        if (bf == NULL) {
                /* TODO: ... */
        }
        bf->sz = sz;
        bf->bits = (char *)(bf + 1);
        bf->nbits = nbits;
        if ((nbits & 0x07) != 0) {
                int n1 = 8 - (nbits & 0x07), high = 0;
                for (i = 0; i < n1; i++)
                        high |= (1 << i);
                high <<= (8 - n1);
                bf->bits[sz - 1] |= high;
        }

        /* initialize @char2idx
         */
        if (char2idx[255] != -1) {
                for (i = 7; i >= 0; i--) {
                        /* |    high     | |   low   |
                         * |-----(i)-----|0|-(8-i-1)-|
                         */
                        unsigned high = 0, low = 0;
                        for (j = 0; j <= i - 1; j++)
                                high |= (1 << j);
                        for (j = 0; j < 8 - i - 1; j++)
                                low |= (1 << j);

                        for (j = 0; j <= high; j++)
                                char2idx[(j << (8 - i)) | low] = 8 - i - 1;
                }
                char2idx[255] = -1; /* no 0 in 255 */
        }

        return bf;
}

/**
 * Return the position of the first 0 in @bf, if no 0 is found,
 * -1 is returned. Position starts from 0
 */
int
bitflag_first0(struct bitflag *bf)
{
        int i = 0, pos = 0;

        for (i = 0; i < bf->sz; i++) {
                pos = char2idx[(unsigned char)(bf->bits[i])];
                if (pos != -1)
                        return (i << 3) + pos;
        }

        return -1;
}

/**
 * Set position @pos of @bf to 1, @pos starts from 0
 */
void
bitflag_set(struct bitflag *bf, int pos)
{
        /* TODO:
        if (pos > bf->nbits)
                return;
        */
        int i = pos >> 3, j = pos & 0x07;
        bf->bits[i] |= (1 << j);
}

void
bitflag_clear(struct bitflag *bf, int pos)
{
        /* TODO:
        if (pos > bf->nbits)
                return;
        */
        int i = pos >> 3, j = pos & 0x07;
        bf->bits[i] &= (0xFF ^ (1 << j));
}

/**
 * This function sets upper portion of (@bf->nbits - @cnt) bits to 1,
 * so that only @cnt bits left to be used in future
 */
void
bitflag_set_portion(struct bitflag *bf, int cnt)
{
        /* TODO:
        if (cnt > bf->nbits)
                return;
        */
        int i = 0, j = 0, k = 0, high = 0;

        memset(bf->bits, 0xFF, bf->sz);
        j = cnt >> 3;
        memset(bf->bits, 0, j);
        k = cnt & 0x07;
        for (i = 0; i < 8 - k; i++)
                high |= (1 << i);
        high <<= k;
        bf->bits[j] = high;
}

void
bitflag_clear_all(struct bitflag *bf)
{
        memset(bf->bits, 0, bf->sz);
        if ((bf->nbits & 0x07) != 0) {
                int n1 = 8 - (bf->nbits & 0x07), high = 0;
                int i = 0;
                for (i = 0; i < n1; i++)
                        high |= (1 << i);
                high <<= (8 - n1);
                bf->bits[bf->sz - 1] |= high;
        }
}

/**
 * Copy @bf->bits to @buf
 */
void
bitflag_copy(struct bitflag *bf, char *buf)
{
        memcpy(bf->bits, buf, bf->sz);
}

