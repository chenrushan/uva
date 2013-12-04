/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char G[10][10];
int N, L;
int path[10], visit[10];
int len, flag;

void
DFS(int v)
{
        int i = 0;

        if (len == L) {
                printf("(");
                printf("%d", path[0]);
                for (i = 1; i <= L; i++)
                        printf(",%d", path[i]);
                printf(")\n");
                flag = 1;
                return;
        }

        for (i = 0; i < N; i++) {
                if (G[v][i] == 0 || visit[i] == 1)
                        continue;
                visit[i] = 1;
                path[++len] = i + 1;
                DFS(i);
                visit[i] = 0;
                len--;
        }
}

int
main(int argc, char **argv)
{
        int i = 0, j = 0;
        int blank = 0;

        while (1) {
                /* read input
                 */
                scanf("%d%d", &N, &L);
                for (i = 0; i < N; i++)
                        for (j = 0; j < N; j++)
                                scanf("%d", &G[i][j]);

                /* initialize
                 */
                len = 0, flag = 0;
                memset(visit, 0, sizeof(visit));
                path[0] = 1;
                visit[0] = 1;

                /* generate output
                 */
                if (blank)
                        putchar('\n');
                blank = 1;
                DFS(0);
                if (flag == 0)
                        printf("no walk of length %d\n", L);

                /* get rid of -9999
                 */
                if (scanf("%d", &i) == EOF)
                        break;
        }

        return 0;
}

