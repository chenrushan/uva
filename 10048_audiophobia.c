/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_VERT 101

int G[MAX_VERT][MAX_VERT];    /* adjcent matrix */
int vcnt;

void
floyd(void)
{
        int i = 0, j = 0, k = 0;
        int max = 0;

        for (k = 1; k <= vcnt; k++) {
                for (i = 1; i <= vcnt; i++) {
                        if (G[i][k] == -1)
                                continue;
                        for (j = 1; j <= vcnt; j++) {
                                if (G[k][j] == -1)
                                        continue;
                                max = G[i][k] > G[k][j] ? G[i][k] : G[k][j];
                                if (G[i][j] == -1 || G[i][j] > max)
                                        G[i][j] = max;
                        }
                }
        }
}

int
main(int argc, char **argv)
{
        int S = 0, Q = 0, i = 0, tc = 1;
        int blank = 0;

        while (1) {
                scanf("%d%d%d", &vcnt, &S, &Q);
                if (vcnt == 0 && S == 0 && Q == 0)
                        break;

                /* read in graph
                 */
                memset(G, 0xFF, sizeof(G));
                for (i = 0; i < S; i++) {
                        int b = 0, e = 0, w = 0;
                        scanf("%d%d%d", &b, &e, &w);
                        G[b][e] = G[e][b] = w;
                }

                floyd();

                /* output header
                 */
                if (blank)
                        putchar('\n');
                blank = 1;
                printf("Case #%d\n", tc++);

                /* make query
                 */
                for (i = 0; i < Q; i++) {
                        int b = 0, e = 0;
                        scanf("%d%d", &b, &e);
                        if (G[b][e] == -1)
                                printf("no path\n");
                        else
                                printf("%d\n", G[b][e]);
                }
        }

        return 0;
}

