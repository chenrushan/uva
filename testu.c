/* 
 * Author: Chen Rushan
 * E-Mail: juscodit@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/*
int len(WI[0:N][2], min)
{
    int next_w = 0;
    int first = -1;

    // find the first one smaller than min for the same W
    W = WI[0][0];
    for (i = 0; i < N; ++i) {
        if (WI[i][0] == W) {
            if (WI[i][1] < min) {
                first = i;
                break;
            }
        } else {
            break;
        }
    }
    // get index for next weight
    for (; i < N; ++i) {
        if (WI[i][0] != W) {
            break;
        }
    }
    next_w = i;

    int len1 = 0;
    if (first != -1) {
        len1 = 1 + len(WI[next_w:N][2], WI[first][1]);
    }
    len2 = len(WI[next_w:N][2], min);

    return max(len1, len2);
}
*/

#define MAX(i, j) ((i) > (j) ? (i) : (j))
#define MAX_ELES 1002
#define MAX_VALUE 10002

int WI[MAX_ELES][2];
int nWIs;
int maxI;

int offset[MAX_VALUE];
int old[MAX_ELES], order[MAX_ELES];

void
setup_offset(int idx)
{
    memset(offset, 0, sizeof(offset));
    /* first get count */
    int i = 0;
    for (i = 0; i < nWIs; ++i) {
        offset[WI[i][idx]] += 1;
    }
    /* turn count to offset */
    int off = offset[1];
    offset[0] = 0;
    for (i = 1; i < MAX_VALUE; ++i) {
        int t = offset[i];
        offset[i] = off;
        off += t;
    }
}

void
sort()
{
    int i = 0;

    setup_offset(1);
    for (i = 0; i < nWIs; ++i) {
        old[offset[WI[i][1]]++] = i;
    }

    setup_offset(0);
    for (i = 0; i < nWIs; ++i) {
        order[offset[WI[old[i]][0]]++] = old[i];
    }

    /*
    for (i = 0; i < nWIs; ++i) {
        printf("%d %d\n", WI[order[i]][0], WI[order[i]][1]);
    }
    */
}

int max_len[MAX_ELES][MAX_VALUE];
int from[MAX_ELES][MAX_VALUE];

void
fill(int beg, int min)
{
    int next_w = 0;
    int first = -1;
    int w = 0, i = 0;

    /* find the first one smaller than min for the same W */
    w = WI[beg][0];
    for (i = beg; i < nWIs; ++i) {
        if (WI[i][0] == w) {
            if (WI[i][1] < min) {
                first = i;
                break;
            }
        } else {
            break;
        }
    }
    printf("first: %d\n", first);

    /* get index for next weight */
    for (; i < nWIs; ++i) {
        if (WI[i][0] != w) {
            break;
        }
    }
    next_w = i;

    int len = 0;
    if (first != -1) {
        len = 1 + max_len[next_w][WI[first][1]];
    }
    max_len[beg][min] = MAX(len, max_len[next_w][min]);
}

void
DP()
{
    int i = 0, j = 0;
    for (i = nWIs - 1; i >= 0; --i) {
        for (j = 0; j < maxI; ++j) {
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

    return 0;
}

