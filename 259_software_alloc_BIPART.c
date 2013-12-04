/* 
 * Name: chen rushan
 * mail: chenrslucky@gmail.com
 * uva: 259
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N_COMP 10
#define MAX_N_SOFT 10
#define MAX_N_NODES (MAX_N_COMP + MAX_N_SOFT)

/* computers are numbered from 0 to 9
 * software are numbered from 10 to 19 
 */

/* the bipartite graph */
int nadjs[MAX_N_NODES];
int G[MAX_N_NODES][MAX_N_NODES];

/* array of flag denoting if an edge is used in matching */
int edge_used[MAX_N_NODES][MAX_N_NODES];

/* array of flag denoting if a node is used */
int node_used[MAX_N_NODES];

/* array of flag denoting if a node is visited in one DFS run */
int dfs_visited[MAX_N_NODES];

/* software id (starting from 10) to name map */
char id2name[MAX_N_NODES];

/* number of softwares */
int nsofts;

int
add_to_graph(char *line)
{
    char name = line[0]; /* name of software */
    int n = line[1] - '0'; /* number of softwares */
    int i = 0;

    if (nsofts + n > MAX_N_SOFT) {
        return 1;
    }
    for (i = 0; i < n; ++i) {
        id2name[nsofts + MAX_N_COMP + i] = name;
    }
    for (i = 3; line[i] != ';'; ++i) {
        int comp = line[i] - '0';
        int id = nsofts + MAX_N_COMP;
        for (; id < nsofts + MAX_N_COMP + n; ++id) {
            G[id][nadjs[id]++] = comp;
            G[comp][nadjs[comp]++] = id;
        }
    }
    nsofts += n;

    return 0;
}

void
print_graph(void)
{
    int i = 0;
    for (i = 0; i < nsofts; ++i) {
        printf("%c ", id2name[MAX_N_COMP + i]);
    }
    printf("\n");

    for (i = 0; i < MAX_N_NODES; ++i) {
        int j = 0;
        printf("%d: ", i);
        for (j = 0; j < nadjs[i]; ++j) {
            printf("%d ", G[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
 * find argmenting path 
 * return 1 if found, 0 otherwise
 */
int
DFS(int u)
{
    int i = 0, v = 0;

    dfs_visited[u] = 1;

    if (u < MAX_N_COMP) {
        if (node_used[u] == 0) {
            node_used[u] = 1;
            return 1;
        }

        for (i = 0; i < nadjs[u]; ++i) {
            v = G[u][i];
            if (! edge_used[u][v] || dfs_visited[v]) {
                continue;
            }
            if (DFS(v) == 1) {
                edge_used[u][v] = 0;
                edge_used[v][u] = 0;
                return 1;
            }
        }
    } else {
        for (i = 0; i < nadjs[u]; ++i) {
            v = G[u][i];

            if (edge_used[u][v] || dfs_visited[v]) {
                continue;
            }
            if (DFS(v) == 1) {
                edge_used[u][v] = 1;
                edge_used[v][u] = 1;
                return 1;
            }
        }
    }

    return 0;
}

/* 
 * return 1 if a matching can be found
 * 0 otherwise
 */
int
bipartite_matching(void)
{
    int i = 0;
    for (; i < nsofts; ++i) {
        int s = MAX_N_COMP + i;
        memset(dfs_visited, 0, sizeof(dfs_visited));
        if (DFS(s) != 1) {
            return 0;
        }
    }
    return 1;
}

void
print_matching(void)
{
    int c = 0;
    for (; c < MAX_N_COMP; ++c) {
        int i = 0;
        for (; i < nadjs[c]; ++i) {
            int s = G[c][i];
            if (edge_used[c][s]) {
                printf("%c", id2name[s]);
                break;
            }
        }
        if (i == nadjs[c]) {
            printf("_");
        }
    }
    printf("\n");
}

int
main(int argc, char **argv)
{
    char buf[200];
    int isEOF = 0, is_too_much_app = 0;

    while (1) {
        memset(nadjs, 0, sizeof(nadjs));
        memset(edge_used, 0, sizeof(edge_used));
        memset(node_used, 0, sizeof(node_used));
        nsofts = 0;

        while (1) {
            if (fgets(buf, sizeof(buf) / sizeof(*buf), stdin) == NULL) {
                isEOF = 1;
                break;
            }
            if (buf[0] == '\n') {
                break;
            }
            if (add_to_graph(buf) == 1) {
                /* too much app for a day */
                is_too_much_app = 1;
            }
        }
        if (is_too_much_app == 1) {
            printf("!\n");
            is_too_much_app = 0;
            continue;
        }

        int ret = 0;
        ret = bipartite_matching();
        if (ret == 0) {
            printf("!\n");
        } else {
            print_matching();
        }

        if (isEOF == 1) {
            break;
        }
    }
    return 0;
}

