/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_NODES 40
#define MAX_EDGES 5000

int G[MAX_NODES][MAX_NODES];    /* adjacent matrix */
int nodes[MAX_NODES];           /* the id of each node, this array is sorted
                                   and can be binary searched */
int nodes_sz;                   /* how many nodes */
int start[MAX_EDGES], end[MAX_EDGES];

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
 * Return the number of nodes not able to visit
 * @b is the index of some node id in @nodes
 */
int
BFS(int b, int ttl)
{
        int queue[MAX_NODES] = {}, dist[MAX_NODES] = {};
        int visit[MAX_NODES] = {};
        int i = 0, t = 0, cnt = 0, j = 0, cur = 0;

        queue[t] = b;
        dist[t++] = 0;
        visit[b] = 1;
        cnt++;

        while (i != t && dist[i] != ttl) {
                cur = queue[i];
                for (j = 0; j < nodes_sz; j++) {
                        if (G[cur][j] == 0 || visit[j] == 1)
                                continue;
                        queue[t] = j;
                        dist[t++] = dist[i] + 1;
                        visit[j] = 1;
                        cnt++;
                }
                i++;
        }

        return nodes_sz - cnt;
}

int
main(int argc, char **argv)
{
        int NC = 0, tc = 1, i = 0, j = 0;

        while (1) {
                scanf("%d\n", &NC);
                if (NC == 0)
                        break;

                memset(G, 0, sizeof(G));
                memset(nodes, 0, sizeof(nodes));
                nodes_sz = 0;

                /* read input
                 */
                j = 0; /* don't forget this */
                for (i = 0; i < NC; i++) {
                        int b = 0, e = 0;
                        scanf("%d%d", &b, &e);
                        start[j] = b;
                        end[j] = e;
                        j++;
                        if (bsrch_node(b) == -1)
                                add_to_nodes(b);
                        if (bsrch_node(e) == -1)
                                add_to_nodes(e);
                }

                /* construct graph
                 */
                for (j = 0; j < NC; j++) {
                        int bi = 0, ei = 0;
                        bi = bsrch_node(start[j]);
                        ei = bsrch_node(end[j]);
                        G[bi][ei] = G[ei][bi] = 1;
                }

                /* read query
                 */
                while (1) {
                        int b = 0, ttl = 0, bi = 0, left = nodes_sz;
                        scanf("%d%d", &b, &ttl);
                        if (b == 0 && ttl == 0)
                                break;
                        bi = bsrch_node(b);
                        if (bi != -1)
                                left = BFS(bi, ttl);
                        printf("Case %d: %d nodes not reachable "
                               "from node %d with TTL = %d.\n",
                               tc++, left, b, ttl);
                }
        }

        return 0;
}

