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

int G[MAX_NODES][MAX_NODES];
int multi[MAX_NODES][MAX_NODES]; /* multi[i][j] is the number of edges from i to j,
                                    a graph maybe a multi-graph*/
int nadjs[MAX_NODES]; /* number of adjacent vertices */
int N; /* number of vertices */

int reach[MAX_NODES][MAX_NODES]; /* reach[i] for all vertices reachable from i */
int nreachs[MAX_NODES]; /* number of vertices reachable from i */

int bedge_reach[MAX_NODES]; /* nodes that can be reached by a back edge dusing
                               DFS from some node */
int nbedge_reach; /* number of nodes */

int subt[MAX_NODES][MAX_NODES]; /* subt[i] is the nodes contained in subtree
                                   rooted at i */
int subt_sz[MAX_NODES]; /* sizeof subtree subt[i] */

int is_cf_edge[MAX_NODES][MAX_NODES]; /* is_cf_edge[b][e] = 1 means edge (b, e)
                                         is a cross edge or a forward edge */
int is_t_edge[MAX_NODES][MAX_NODES]; /* is_t_edge[b][e] = 1 means edge (b, e)
                                        is a tree edge */

int visited[MAX_NODES]; /* boolean array */
int arr[MAX_NODES]; /* arrival time */
int dep[MAX_NODES]; /* departure time */

int npaths[MAX_NODES][MAX_NODES];

void
_find_reachable(int root, int cur)
{
    int i = 0;
    for (i = 0; i < nadjs[cur]; ++i) {
        int n = G[cur][i];
        if (visited[n] == 0) {
            visited[n] = 1;
            reach[root][nreachs[root]++] = n;
            _find_reachable(root, n);
        }
    }
}

void
find_reachable(int i)
{
    memset(visited, 0, sizeof(visited));
    nreachs[i] = 0;
    visited[i] = 1;
    _find_reachable(i, i);
}

void
set_reach()
{
    int i = 0;
    for (i = 0; i < N; ++i) {
        find_reachable(i);
    }
}

void
update_subtree(int root, int sr, int is_from_cf)
{
    int i = 0, j = 0;

    if (is_from_cf) {
        /* update path number of nodes of subtree rooted at @sr */
        for (j = 0; j < subt_sz[sr]; ++j) {
            npaths[root][subt[sr][j]] += 1;
        }
    }

    /* iterate over all cross edge and forward edges */
    for (i = 0; i < nadjs[sr]; ++i) {
        int m = G[sr][i];
        if (is_cf_edge[sr][m]) {
            update_subtree(root, m, 1);
        } else if (is_t_edge[sr][m]) {
            update_subtree(root, m, 0);
        }
    }
}

void
_DFS(int root, int n, int *p_time)
{
    int i = 0, j = 0;

    visited[n] = 1;
    arr[n] = (*p_time)++;
    subt[n][subt_sz[n]++] = n;

    for (i = 0; i < nadjs[n]; ++i) {
        int m = G[n][i];
        if (visited[m] == 0) { /* tree edge */
            is_t_edge[n][m] = 1;
            _DFS(root, m, p_time);
            /* merge nodes in subtree m into subtree n */
            for (j = 0; j < subt_sz[m]; ++j) {
                subt[n][subt_sz[n]++] = subt[m][j];
            }
        } else {
            if (dep[m] == -1) { /* back edge */
                bedge_reach[nbedge_reach++] = m;
            } else { /* corss edge or forward edge */
                is_cf_edge[n][m] = 1;
                update_subtree(root, m, 1);
            }
        }
    }

    dep[n] = (*p_time)++;
}

void
DFS()
{
    int i = 0, j = 0;

    memset(npaths, 0, sizeof(npaths));
    for (i = 0; i < N; ++i) {
        int time = 0;

        nbedge_reach = 0;
        memset(subt_sz, 0, sizeof(subt_sz));
        memset(is_cf_edge, 0, sizeof(is_cf_edge));
        memset(is_t_edge, 0, sizeof(is_t_edge));
        memset(visited, 0, sizeof(visited));
        memset(arr, 0xFF, sizeof(arr));
        memset(dep, 0xFF, sizeof(dep));

        /* for tree edges */
        for (j = 0; j < nreachs[i]; ++j) {
            npaths[i][reach[i][j]] += 1;
        }
        _DFS(i, i, &time);

        /* for those nodes reached by back edges */
        for (j = 0; j < nbedge_reach; ++j) {
            int k = 0, n = bedge_reach[j];
            npaths[i][n] = -1;
            for (; k < nreachs[n]; ++k) {
                npaths[i][reach[n][k]] = -1;
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

        /* read in graph */
        memset(nadjs, 0, sizeof(nadjs));
        memset(multi, 0, sizeof(multi));
        for (i = 0; i < nedges; ++i) {
            scanf("%d%d", &b, &e);
            max = MAX(MAX(max, e), b);
            if (multi[b][e] == 0) {
                G[b][nadjs[b]++] = e;
                multi[b][e] = 1;
            }
        }
        N = max + 1;

        set_reach();
        DFS();

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

