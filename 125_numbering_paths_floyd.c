/* 
 * Name: chen rushan
 * mail: chenrslucky@gmail.com
 * uva 125
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MAX_NODES 30

int npaths[MAX_NODES][MAX_NODES];
int N; /* number of vertices */

void
floyd()
{
    int i = 0, j = 0, k = 0;

    for (k = 0; k < N; ++k) {
        for (i = 0; i < N; ++i) {
            for (j = 0; j < N; ++j) {
                npaths[i][j] += npaths[i][k] * npaths[k][j];
            }
        }
    }

    /* check if there's cycle */
    for (k = 0; k < N; ++k) {
        if (npaths[k][k] != 0) {
            for (i = 0; i < N; ++i) {
                for (j = 0; j < N; ++j) {
                    if (npaths[i][k] != 0 && npaths[k][j] != 0) {
                        npaths[i][j] = -1;
                    }
                }
            }
        }
    }
}

int
main(int argc, char **argv)
{
    int nedges = 0, b = 0, e = 0, i = 0, id = 0;

    while (scanf("%d", &nedges) != EOF) {
        int max = 0;

        if (nedges == 0) {
            printf("matrix for city %d\n", id++);
            continue;
        }

        memset(npaths, 0, sizeof(npaths));
        for (i = 0; i < nedges; ++i) {
            scanf("%d%d", &b, &e);
            max = MAX(MAX(max, e), b);
            npaths[b][e] = 1;
        }
        N = max + 1;

        floyd();

        printf("matrix for city %d\n", id++);
        for (i = 0; i < N; ++i) {
            int j = 0;
            for (j = 0; j < N; ++j) {
                if (j != 0) {
                    printf(" ");
                }
                printf("%d", npaths[i][j]);
            }
            printf("\n");
        }
    }

    return 0;
}

