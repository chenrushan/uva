/* 
 * Name: chen rushan
 * mail: chenrslucky@gmail.com
 * uva: 10304
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 251

int Q[MAX_NODES]; /* number of queries */
int N;

/* T[beg][end]: minimal cost of BST with nodes from
 * @beg to @end (@end is one after the last element)
 */
int T[MAX_NODES][MAX_NODES];

int Qsum[MAX_NODES][MAX_NODES];

void
DP(void)
{
    int i = 0, j = 0;

    /* fill Qsum */
    for (i = 0; i < N; ++i) {
        Qsum[i][i] = 0;
        for (j = i + 1; j < N + 1; ++j) {
            Qsum[i][j] = Qsum[i][j - 1] + Q[j - 1];
        }
    }

    /* initialize */
    for (i = 0; i < N; ++i) {
        T[i][i] = T[i][i + 1] = 0;
    }
    T[N][N] = 0;

    /* fill table */
    for (i = N - 2; i > -1; --i) {
        for (j = i + 2; j <= N; ++j) {
            int k = i;
            int min = T[i][k] + T[k + 1][j] + Qsum[i][j] - Q[k];
            for (k = i + 1; k < j; ++k) {
                int cost = T[i][k] + T[k + 1][j] + Qsum[i][j] - Q[k];
                if (min > cost) {
                    min = cost;
                }
            }
            T[i][j] = min;
        }
    }
}

int
main(int argc, char **argv)
{
    int i = 0;

    while (1) {
        if (scanf("%d", &N) == -1) {
            break;
        }
        for (i = 0; i < N; ++i) {
            scanf("%d", Q + i);
        }

        DP();
        printf("%d\n", T[0][N]);
    }
    return 0;
}

