/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 * uva: 10000
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 110

int adjs[MAX_NODES][MAX_NODES];
int nadjs[MAX_NODES];
int N; /* number of nodes in graph */
int start; /* start point */

int longest[MAX_NODES];
int end[MAX_NODES];
int ncase;

void
DP(void)
{
    int i = 0, j = 0, k = 0;

    /* init */
    memset(longest, 0, sizeof(longest));
    for (i = 1; i <= N; ++i) {
        end[i] = i;
    }

    /* i: level */
    for (i = 1; i <= N; ++i) {
        /* j: node */
        for (j = 1; j <= N; ++j) {
            int max = 0;
            int e = 0; /* end of longest path */

            /* k: adjacent */
            for (k = 0; k < nadjs[j]; ++k) {
                int n = adjs[j][k];
                int l = 1 + longest[n];

                if (l > max) {
                    max = l;
                    e = end[n];
                } else if (l == max) {
                    if (end[n] < e) {
                        e = end[n];
                    }
                }
            }
            longest[j] = max;
            if (e != 0) {
                end[j] = e;
            }
        }
    }
    printf("Case %d: The longest path from %d has length %d, finishing at %d.\n\n",
           ncase, start, longest[start], end[start]);
}

int
main(int argc, char **argv)
{
    while (1) {
        scanf("%d", &N);
        if (N == 0) {
            break;
        }
        scanf("%d", &start);

        memset(nadjs, 0, sizeof(nadjs));

        while (1) {
            int b, e;
            scanf("%d%d", &b, &e);
            if (b == 0 && e == 0) {
                break;
            }
            adjs[b][nadjs[b]++] = e;
        }

        ncase += 1;
        DP();
    }

    return 0;
}

