/* 
 * Name: chen rushan
 * mail: chenrslucky@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 101

#define CAT 0
#define MOUSE 1

/* [0] for cat [1] for mouse */
int n_nodes;
int nadjs[2][MAX_NODES];
int G[2][MAX_NODES][MAX_NODES];
int S[2]; /* start point of cat/mouse */
int reach[2][MAX_NODES];

/* used by mouse bfs */
int edge_used[MAX_NODES][MAX_NODES];
int mou_queue[MAX_NODES * MAX_NODES];

void
BFS(int id)
{
    char visited[MAX_NODES] = {};
    int queue[MAX_NODES] = {};
    int beg = 0, end = 0;

    queue[end++] = S[id];
    visited[S[id]] = 1;
    reach[id][S[id]] = 1;

    while (beg != end) {
        int i = 0, n = queue[beg++];
        for (i = 0; i < nadjs[id][n]; ++i) {
            int m = G[id][n][i];
            if (visited[m] != 1) {
                queue[end++] = m;
                visited[m] = 1;
                reach[id][m] = 1;
            }
        }
    }
}

int
BFS_mouse(void)
{
    int beg = 0, end = 0;

    if (reach[CAT][S[MOUSE]] == 1) {
        return 0;
    }

    memset(edge_used, 0, sizeof(edge_used));

    mou_queue[end++] = S[MOUSE];
    while (beg != end) {
        int i = 0, n = mou_queue[beg++];
        for (i = 0; i < nadjs[MOUSE][n]; ++i) {
            int m = G[MOUSE][n][i];
            if (edge_used[n][m] != 1 && reach[0][m] == 0) {
                if (m == S[MOUSE]) {
                    return 1;
                }
                mou_queue[end++] = m;
                edge_used[n][m] = 1;
            }
        }
    }

    return 0;
}


void
print_G_helper(int id)
{
    int i = 0, j = 0;
    printf("%s\n", id == 0 ? "cat" : "mouse");
    for (i = 1; i <= n_nodes; ++i) {
        printf("%d: ", i);
        for (j = 0; j < nadjs[id][i]; ++j) {
            printf("%d ", G[id][i][j]);
        }
        printf("\n");
    }
}

void
print_G(void)
{
    int i = 0;

    printf("n_nodes: %d\ncat: %d\nmouse: %d\n", n_nodes, S[0], S[1]);
    print_G_helper(0);
    print_G_helper(1);
}

int
main(int argc, char **argv)
{
    int N = 0;
    char line[100];
    int sz = sizeof(line) / sizeof(*line);

    fgets(line, sz, stdin);
    sscanf(line, "%d", &N);
    fgets(line, sz, stdin);

    while (N--) {
        memset(nadjs, 0, sizeof(nadjs));
        memset(reach, 0, sizeof(reach));

        int id = CAT;
        fgets(line, sz, stdin);
        sscanf(line, "%d%d%d", &n_nodes, S, S + 1);
        while (1) {
            if (fgets(line, sz, stdin) == NULL) {
                break;
            }
            if (line[0] == '\n') {
                break;
            }

            int b, e;
            sscanf(line, "%d%d", &b, &e);
            if (b == -1) {
                id = MOUSE;
                continue;
            }

            G[id][b][nadjs[id][b]++] = e;
        }

        BFS(CAT);
        BFS(MOUSE);

        /* check if they can reach each other somewhere */
        int i = 0;
        int reachable = 0;
        for (i = 1; i <= n_nodes; ++i) {
            if (reach[0][i] && reach[1][i]) {
                reachable = 1;
                break;
            }
        }

        /* check if mouse can go back home without meeting cat */
        int flag = BFS_mouse();

        printf("%c %c\n", (reachable == 1 ? 'Y' : 'N'),
               (flag == 1 ? 'Y' : 'N'));
        if (N != 0) {
            printf("\n");
        }
    }

    return 0;
}

