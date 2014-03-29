/* 
 * Author: Chen Rushan
 * E-Mail: juscodit@gmail.com
 * 10131
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX(i, j) ((i) > (j) ? (i) : (j))
#define MAX_ELES 1002
#define MAX_VALUE 10002

int WI[MAX_ELES][2];
int nWIs;
int maxI;

int offset[MAX_VALUE];
int old[MAX_ELES], order[MAX_ELES];

void
setup_offset(int idx, int ord)
{
    memset(offset, 0, sizeof(offset));
    /* first get count */
    int i = 0;
    for (i = 0; i < nWIs; ++i) {
        offset[WI[i][idx]] += 1;
    }
    /* turn count to offset */
    if (ord == 0) {
        int off = offset[1];
        offset[0] = 0;
        for (i = 1; i < MAX_VALUE; ++i) {
            int t = offset[i];
            offset[i] = off;
            off += t;
        }
    } else {
        int off = offset[MAX_VALUE - 1];
        offset[MAX_VALUE - 1] = 0;
        for (i = MAX_VALUE - 2; i >= 0; --i) {
            int t = offset[i];
            offset[i] = off;
            off += t;
        }
    }
}

void
sort()
{
    int i = 0;

    setup_offset(1, 1);
    for (i = 0; i < nWIs; ++i) {
        old[offset[WI[i][1]]++] = i;
    }

    setup_offset(0, 0);
    for (i = 0; i < nWIs; ++i) {
        order[offset[WI[old[i]][0]]++] = old[i];
    }
}

struct t_position {
    int beg;
    int min;
};

int max_len[MAX_ELES][MAX_VALUE];
struct t_position from[MAX_ELES][MAX_VALUE];

void
fill(int beg, int min)
{
    int next_w = 0;
    int first = -1;
    int w = 0, i = 0;

    /* find the first one smaller than min for the same w */
    w = WI[order[beg]][0];
    for (i = beg; i < nWIs; ++i) {
        if (WI[order[i]][0] == w) {
            if (WI[order[i]][1] < min) {
                first = i;
                break;
            }
        } else {
            break;
        }
    }

    /* get index for next weight */
    for (; i < nWIs; ++i) {
        if (WI[order[i]][0] != w) {
            break;
        }
    }
    next_w = i;

    int len = 0;
    if (first != -1) {
        len = 1 + max_len[next_w][WI[order[first]][1]];
    }
    if (len > max_len[next_w][min]) {
        max_len[beg][min] = len;
        from[beg][min].beg = next_w;
        from[beg][min].min = WI[order[first]][1];
    } else {
        max_len[beg][min] = max_len[next_w][min];
        from[beg][min].beg = next_w;
        from[beg][min].min = min;
    }
}

void
DP()
{
    int i = 0, j = 0;
    for (i = nWIs - 1; i >= 0; --i) {
        for (j = 0; j <= maxI; ++j) {
            fill(i, j);
        }
    }
}

int
main(int argc, char **argv)
{
    int i = 0, w = 0;

    /* read input */
    while (scanf("%d%d", &w, &i) != EOF) {
        WI[nWIs][0] = w;
        WI[nWIs][1] = i;
        nWIs += 1;
        maxI = MAX(i, maxI);
    }
    maxI += 1;

    sort();
    DP();
    printf("%d\n", max_len[0][maxI]);

    int cur_beg = 0, cur_min = maxI;

    while (cur_beg < nWIs) {
        if (from[cur_beg][cur_min].min < cur_min) {
            w = WI[order[cur_beg]][0];
            for (i = cur_beg; i < nWIs; ++i) {
                if (WI[order[i]][0] != w) {
                    break;
                }
                if (WI[order[i]][1] < cur_min) {
                    break;
                }
            }
            printf("%d\n", order[i] + 1);
        }

        int cb = from[cur_beg][cur_min].beg;
        int cm = from[cur_beg][cur_min].min;
        cur_beg = cb;
        cur_min = cm;
    }

    return 0;
}

