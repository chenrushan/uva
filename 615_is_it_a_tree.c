/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_NODES 2000
#define MAX_EDGES 100000

int G[MAX_NODES][MAX_NODES];    /* adjacent matrix */
int indegs[MAX_NODES];
int nodes[MAX_NODES];           /* the id of each node, this array is sorted
                                   and can be binary searched */
int nodes_sz;                   /* how many nodes */
int start[MAX_EDGES], end[MAX_EDGES];
int nedges;
int queue[MAX_NODES];
int visit[MAX_NODES];

/**
 * Return the index for @id, if no such @id, return -1
 */
int
bsrch_node(int id)
{
        int b = 0, e = nodes_sz - 1, m = 0;

        while (b <= e) {
                m = (b + e) / 2;
                if (nodes[m] == id)
                        return m;
                else if (nodes[m] < id)
                        b = m + 1;
                else
                        e = m - 1;
        }

        return -1;
}

/**
 * add @id to @nodes using insertion sort
 */
void
add_to_nodes(int id)
{
        int i = 0;
        for (i = nodes_sz - 1; i >= 0 && nodes[i] > id; i--)
                nodes[i + 1] = nodes[i];
        nodes[i + 1] = id;
        nodes_sz++;
}

/**
 * Return if all vertices are visited
 */
int
BFS(int r)
{
        int i = 0, t = 0, j = 0;

        queue[t++] = r;
        visit[r] = 1;

        while (i != t) {
                int cur = queue[i++];

                for (j = 0; j < nodes_sz; j++) {
                        if (G[cur][j] == 0 || visit[j] == 1)
                                continue;
                        queue[t++] = j;
                        visit[j] = 1;
                }
        }

        for (i = 0; i < nodes_sz; i++)
                if (visit[i] == 0)
                        return 0;
        return 1;
}

int
main(int argc, char **argv)
{
        int b = 0, e = 0;
        int i = 0, tc = 1;

        while (1) {
                scanf("%d%d", &b, &e);
                if (b == -1 && e == -1)
                        break;

                /* initialize
                 */
                memset(G, 0, sizeof(G));
                memset(indegs, 0, sizeof(indegs));
                memset(nodes, 0, sizeof(nodes));
                memset(start, 0, sizeof(start));
                memset(end, 0, sizeof(end));
                memset(queue, 0, sizeof(queue));
                memset(visit, 0, sizeof(visit));
                nodes_sz = nedges = 0;

                /* read input and construct graph
                 */
                while (1) {
                        if (b == 0 && e == 0)
                                break;
                        int bi = bsrch_node(b), ei = bsrch_node(e);
                        if (bi == -1)
                                add_to_nodes(b);
                        if (ei == -1)
                                add_to_nodes(e);
                        start[nedges] = b;
                        end[nedges++] = e;

                        scanf("%d%d", &b, &e);
                }
                if (nedges == 0) {
                        printf("Case %d is a tree.\n", tc++);
                        continue;
                }

                for (i = 0; i < nedges; i++) {
                        int bi = bsrch_node(start[i]);
                        int ei = bsrch_node(end[i]);
                        G[bi][ei] = 1;
                        indegs[ei]++;
                }

                int r = -1, is_root = 0;
                for (i = 0; i < nodes_sz; i++) {
                        if (indegs[i] > 1) { /* indegree > 1 */
                                printf("Case %d is not a tree.\n", tc++);
                                break;
                        } else if (indegs[i] == 0) {
                                if (is_root == 1) { /* multiple root */
                                        printf("Case %d is not a tree.\n", tc++);
                                        break;
                                }
                                is_root = 1;
                                r = i;
                        }
                }
                if (i != nodes_sz) /* not a tree */
                        continue;
                if (r == -1) { /* no root found */
                        printf("Case %d is not a tree.\n", tc++);
                        continue;
                }
                if (BFS(r))
                        printf("Case %d is a tree.\n", tc++);
                else
                        printf("Case %d is not a tree.\n", tc++);
        }

        return 0;
}

