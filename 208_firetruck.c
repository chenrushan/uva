/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_VERTICES 21

int G[MAX_VERTICES][MAX_VERTICES];      /* adjacent list */
int nadjs[MAX_VERTICES];
int visit[MAX_VERTICES];
int reach[MAX_VERTICES];                /* can a vertex reach the destination */
int maxv;

/**
 * Return number of routes from @cur to @dest
 */
int
DFS(int cur, int dest, int *path, int path_idx)
{
        int i = 0, cnt = 0;

        if (cur == dest) {
                path[path_idx++] = cur;
                /* print route
                 */
                printf("%d", path[0] + 1);
                for (i = 1; i < path_idx; i++)
                        printf(" %d", path[i] + 1);
                printf("\n");
                return 1;
        }

        visit[cur] = 1;
        path[path_idx++] = cur;
        for (i = 0; i < nadjs[cur]; i++) {
                int k = G[cur][i];
                if (visit[k] == 0 && reach[k] == 1)
                        cnt += DFS(k, dest, path, path_idx);
        }
        /*
        for (i = 0; i <= maxv; i++) {
                if (G[cur][i] == 1 && visit[i] == 0 && reach[i] == 1)
                        cnt += DFS(i, dest, path, path_idx);
        }
        */
        visit[cur] = 0;

        return cnt;
}

void
set_reach(int cur)
{
        int i = 0;

        reach[cur] = 1;
        for (i = 0; i < nadjs[cur]; i++)
                if (! reach[G[cur][i]])
                        set_reach(G[cur][i]);
        /*
        for (i = 0; i <= maxv; i++)
                if (G[cur][i] && ! reach[i])
                        set_reach(i);
        */
}

int
main(int argc, char **argv)
{
        int path[MAX_VERTICES];
        int d = 0, cnt = 0, c = 1;

        while (1) {
                if (scanf("%d", &d) == EOF)
                        break;

                memset(G, 0, sizeof(G));
                memset(nadjs, 0, sizeof(nadjs));
                memset(visit, 0, sizeof(visit));
                memset(reach, 0, sizeof(reach));
                maxv = -1;

                printf("CASE %d:\n", c++);

                while (1) {
                        int b = 0, e = 0, i = 0;

                        scanf("%d%d", &b, &e);
                        if (b == 0 && e == 0)
                                break;

                        b--, e--;
                        /*
                        G[b][e] = G[e][b] = 1;
                        if (maxv < e)
                                maxv = e;
                        if (maxv < b)
                                maxv = b;
                        */
                        for (i = nadjs[b] - 1; i >= 0 && G[b][i] > e; i--)
                                G[b][i + 1] = G[b][i];
                        G[b][i + 1] = e;
                        nadjs[b]++;
                        for (i = nadjs[e] - 1; i >= 0 && G[e][i] > b; i--)
                                G[e][i + 1] = G[e][i];
                        G[e][i + 1] = b;
                        nadjs[e]++;
                }
                set_reach(d - 1);
                cnt = DFS(0, d - 1, path, 0);

                printf("There are %d routes from the firestation "
                       "to streetcorner %d.\n", cnt, d);
        }

        return 0;
}

