/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define VERT_CNT 510

struct vertex {
        short num;
        short dist;
};

struct vertex g_econ[VERT_CNT][VERT_CNT]; /* adjacent list for economy express */
struct vertex g_comm[VERT_CNT][VERT_CNT]; /* adjacent list for commercial express  */
short nadj_econ[VERT_CNT];
short nadj_comm[VERT_CNT];
char visit_econ[VERT_CNT];
char visit_comm[VERT_CNT];
short prev_econ[VERT_CNT];              /* previous vertex for path using
                                           only economy route */
short prev_comm[VERT_CNT];              /* previous vertex for path using
                                           one commercial route */
char prev_comm_is_comm[VERT_CNT];       /* if the route from previous vertex
                                           is through commercial route */
int dist_econ[VERT_CNT];                /* econ_dist[i]: the dist from S to i
                                           using only economy express */
int dist_comm[VERT_CNT];                /* comm_dist[i]: the dist from S to i
                                           using one commercial route */
int N, S, E;

int path[VERT_CNT];
int C, T;

/**
 * The shortest path to a given vertex @v can only be the following 3 cases:
 * (1). a econ path + comm(u, v)
 * (2). a comm path + econ(u, v)
 * (3). a econ path + econ(u, v)
 * comm path denotes a minimum path containing a commercial edge
 * econ path denotes a minimum path containing only economy edge
 * comm(u, v) denotes a commercial edge
 * econ(u, v) denotes a economy edge
 *
 * Remember that no matter a path is a comm path or econ path, it's sure to be
 * minimum when it's minimum in all paths including comm paths and econ paths.
 * That's why I only set @visit_comm[i] to 1 only when (emin != -1 && cmin != -1
 * && cmin < emin) (it costs me a lot of time to figure out the condition to set
 * @visit_comm[i])
 */

void
find_shortest_distance()
{
        int ecurv = S; /* economy current vertex */
        int ccurv = -1; /* commercial current vertex */
        int i = 0;

        dist_econ[S] = dist_comm[S] = 0;
        visit_econ[S] = visit_comm[S] = 1;

        while (1) {
                int d = 0, e = 0;
                int emin = -1, emini = 0;
                int cmin = -1, cmini = 0;

                /* update distance
                 */
                if (ecurv != -1) {
                        for (i = 0; i < nadj_econ[ecurv]; i++) {
                                e = g_econ[ecurv][i].num;
                                d = dist_econ[ecurv] + g_econ[ecurv][i].dist;
                                if (dist_econ[e] == -1 || dist_econ[e] > d) {
                                        dist_econ[e] = d;
                                        prev_econ[e] = ecurv;
                                }
                        }
                        for (i = 0; i < nadj_comm[ecurv]; i++) {
                                e = g_comm[ecurv][i].num;
                                d = dist_econ[ecurv] + g_comm[ecurv][i].dist;
                                if (dist_comm[e] == -1 || dist_comm[e] > d) {
                                        dist_comm[e] = d;
                                        prev_comm[e] = ecurv;
                                        prev_comm_is_comm[e] = 1;
                                }
                        }
                }
                if (ccurv != -1) {
                        for (i = 0; i < nadj_econ[ccurv]; i++) {
                                e = g_econ[ccurv][i].num;
                                d = dist_comm[ccurv] + g_econ[ccurv][i].dist;
                                if (dist_comm[e] == -1 || dist_comm[e] > d) {
                                        dist_comm[e] = d;
                                        prev_comm[e] = ccurv;
                                        prev_comm_is_comm[e] = 0; /* don't forget */
                                }
                        }
                }

                /**
                 * Find minimum distance
                 */
                if (ecurv != -1) {
                        for (i = 0; i < VERT_CNT; i++) {
                                if (visit_econ[i] == 1 || dist_econ[i] == -1)
                                        continue;
                                if (emin == -1 || emin > dist_econ[i]) {
                                        emin = dist_econ[i];
                                        emini = i;
                                }
                        }
                }
                for (i = 0; i < VERT_CNT; i++) {
                        if (visit_comm[i] == 1 || dist_comm[i] == -1)
                                continue;
                        if (cmin == -1 || cmin > dist_comm[i]) {
                                cmin = dist_comm[i];
                                cmini = i;
                        }
                }

                if (emin == -1 && cmin == -1)
                        break;

                ecurv = ccurv = -1;

                if (emin != -1) {
                        ecurv = emini;
                        visit_econ[emini] = 1;
                        if (cmin != -1 && cmin < emin) {
                                if (cmini == E)
                                        break;
                                ccurv = cmini;
                                visit_comm[cmini] = 1;
                        }
                } else {
                        if (cmin != -1) {
                                if (cmini == E)
                                        break;
                                ccurv = cmini;
                                visit_comm[cmini] = 1;
                        }
                }
        }

        /* print the result
         */
        int v = 0, j = 0;
        i = 0;
        C = -1;
        if ((dist_econ[E] != -1 && dist_comm[E] != -1
             && dist_econ[E] > dist_comm[E]) || dist_econ[E] == -1) { /* use comm */
                T = dist_comm[E];
                v = E;
                while (1) {
                        path[i++] = v;
                        if (prev_comm_is_comm[v] == 1) {
                                v = prev_comm[v];
                                break;
                        }
                        v = prev_comm[v];
                }
                C = v;
                while (1) {
                        path[i++] = v;
                        if (prev_econ[v] == -1)
                                break;
                        v = prev_econ[v];
                }
        } else { /* use econ only */
                T = dist_econ[E];
                v = E;
                while (1) {
                        path[i++] = v;
                        if (prev_econ[v] == -1)
                                break;
                        v = prev_econ[v];
                }
        }
        printf("%d", path[i - 1]);
        for (j = i - 2; j >= 0; j--)
                printf(" %d", path[j]);
        putchar('\n');
        if (C == -1)
                printf("Ticket Not Used\n");
        else
                printf("%d\n", C);
        printf("%d\n", T);
}

int
main(int argc, char **argv)
{
        int ne_edge = 0, nc_edge = 0;
        int i = 0;
        int blank = 0;

        while (1) {
                int b = 0, e = 0, d = 0;

                /* initialize
                 */
                memset(g_econ, 0, sizeof(g_econ));
                memset(g_comm, 0, sizeof(g_comm));
                memset(nadj_econ, 0, sizeof(nadj_econ));
                memset(nadj_comm, 0, sizeof(nadj_comm));
                memset(visit_econ, 0, sizeof(visit_econ));
                memset(visit_comm, 0, sizeof(visit_comm));
                memset(prev_econ, 0xFF, sizeof(prev_econ));
                memset(prev_comm, 0xFF, sizeof(prev_comm));
                memset(prev_comm_is_comm, 0, sizeof(prev_comm_is_comm));
                memset(dist_econ, 0xFF, sizeof(dist_econ));
                memset(dist_comm, 0xFF, sizeof(dist_comm));

                /* read input
                 */
                if (scanf("%d%d%d", &N, &S, &E) == EOF)
                        break;
                scanf("%d", &ne_edge);
                for (i = 0; i < ne_edge; i++) {
                        scanf("%d%d%d", &b, &e, &d);
                        g_econ[b][nadj_econ[b]].num = e;
                        g_econ[b][nadj_econ[b]].dist = d;
                        nadj_econ[b]++;
                        g_econ[e][nadj_econ[e]].num = b;
                        g_econ[e][nadj_econ[e]].dist = d;
                        nadj_econ[e]++;
                }
                scanf("%d", &nc_edge);
                for (i = 0; i < nc_edge; i++) {
                        scanf("%d%d%d", &b, &e, &d);
                        g_comm[b][nadj_comm[b]].num = e;
                        g_comm[b][nadj_comm[b]].dist = d;
                        nadj_comm[b]++;
                        g_comm[e][nadj_comm[e]].num = b;
                        g_comm[e][nadj_comm[e]].dist = d;
                        nadj_comm[e]++;
                }
                if (blank == 1)
                        putchar('\n');
                blank = 1;
                if (S == E) {
                        printf("%d %d\nTicket Not Used\n0\n", S, E);
                        continue;
                }
                find_shortest_distance();
        }

        return 0;
}

