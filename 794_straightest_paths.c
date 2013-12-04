/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_NROW 50
#define MAX_NCOL 90
#define QUEUE_SZ (MAX_NROW * MAX_NCOL)

struct position {
        int row;
        int col;
};

int rows;
char G[MAX_NROW][MAX_NCOL];
int level[MAX_NROW][MAX_NCOL];

int is_draw[MAX_NROW][MAX_NCOL][4]; /* dynamic programming to avoid drawing
                                       same path multiple times and record
                                       the number of different paths */

/* nturns[r][c][i]: minimum turns from
 * start to (r, c) from some direction.
 * [0]: from up, [1]: from right
 * [2]: from down, [3]: from left
 */
int nturns[MAX_NROW][MAX_NCOL][4];

struct position start, stop;
struct position queue[QUEUE_SZ];
int cur, tail;

/**
 * DP to print path and get the number of different paths.
 * I used to implement this function using simple DFS, and it's quite slow
 * for some kind of input. After changing to DP, it becomes so quick, and
 * this is the first time I experience the strong power of DP.
 *
 * @prev_dir: from which direction of previous grid to this (@r, @c) grid
 */
void
draw_paths(int r, int c, int t, int prev_dir)
{
        static int r_dir[] = { -1, 0, 1, 0 }; /* up, right, down, left */
        static int c_dir[] = { 0, 1, 0, -1 }; /* up, right, down, left */
        static int skip[] = { 2, 3, 0, 1 };

        int i = 0;

        if (r == start.row && c == start.col) {
                is_draw[r][c][prev_dir] = 1;
                return;
        }

        G[r][c] = '#';

        for (i = 0; i < 4; i++) {
                if (i == skip[prev_dir] || nturns[r][c][i] == -1)
                        continue;
                int nr = r + r_dir[i], nc = c + c_dir[i];

                if (i == prev_dir && t == nturns[r][c][i]) {
                        if (is_draw[nr][nc][i] == 0)
                                draw_paths(nr, nc, t, i);
                        is_draw[r][c][prev_dir] += is_draw[nr][nc][i];
                } else if (i != prev_dir && t == nturns[r][c][i] + 1) {
                        if (is_draw[nr][nc][i] == 0)
                                draw_paths(nr, nc, t - 1, i);
                        is_draw[r][c][prev_dir] += is_draw[nr][nc][i];
                }
        }
}

/**
 * Enqueue grids adjacent to (r, c) if they're not already in the queue
 */
void
enqueue_adj_grids(int r, int c)
{
        static int r_dir[] = { -1, 0, 1, 0 }; /* up, right, down, left */
        static int c_dir[] = { 0, 1, 0, -1 }; /* up, right, down, left */
        static int dir[] = { 2, 3, 0, 1 }; /* corresponds to the last index
                                              of @nturns */
        static int skip[] = { 0, 1, 2, 3 }; /* corresponds to the last index
                                               of @nturns */

        int l = 0, i = 0, j = 0;
        int nr = 0, nc = 0, d = 0;

        l = level[r][c];

        /* for all adjacent grids
         */
        for (i = 0; i < 4; i++) {
                nr = r + r_dir[i];
                nc = c + c_dir[i];
                d = dir[i];

                if (G[nr][nc] == 'X') /* obstacle ? */
                        continue;

                /* level[nr][nc] == 0: this grid hasn't been visited before
                 * level[nr][nc] == l + 1: this grid is visited by [r][c]
                 *      again, and its depth is the same as previous one.
                 * NOTE that the depth at which a grid is visited the first
                 * time is guranteed to be the shortest path from start
                 * point. If level[nr][nc] == l + 1, then we know that we
                 * find another shortest path from @start to [nr][nc]
                 */
                if (level[nr][nc] != 0 && level[nr][nc] != l + 1)
                        continue;

                /* update @nturns for [nr][nc][d]
                 * we need to consider all possible paths from (r, c) to (nr, nc)
                 */
                for (j = 0; j < 4; j++) {
                        if (nturns[r][c][j] == -1 || j == skip[i])
                                continue;

                        int t = nturns[r][c][j];
                        if (j != d)
                                t++;
                        if (nturns[nr][nc][d] == -1 || nturns[nr][nc][d] > t)
                                nturns[nr][nc][d] = t;
                }

                /* enqueue if this grid hasn't been visited yet
                 */
                if (level[nr][nc] == 0) {
                        queue[tail].row = nr;
                        queue[tail].col = nc;
                        level[nr][nc] = l + 1;
                        tail++;
                }
        }
}

void
BFS(void)
{
        struct position p;
        int r = 0, c = 0;

        r = start.row, c = start.col;
        queue[0] = start;
        level[r][c] = 1;
        nturns[r][c][0] = nturns[r][c][1] = 0;
        nturns[r][c][2] = nturns[r][c][3] = 0;

        cur = 0, tail = 1;

        while (cur != tail) {
                p = queue[cur++];
                if (p.row == stop.row && p.col == stop.col)
                        break;
                enqueue_adj_grids(p.row, p.col);
        }

        /* draw paths and get the number of paths
         */
        int i = 0, mint = 0, npaths = 0;
        static int r_dir[] = { -1, 0, 1, 0 }; /* up, right, down, left */
        static int c_dir[] = { 0, 1, 0, -1 }; /* up, right, down, left */

        r = stop.row, c = stop.col;
        for (i = 0; i < 4; i++)
                if (nturns[r][c][i] != -1) {
                        mint = nturns[r][c][i];
                        break;
                }
        for (; i < 4; i++)
                if (nturns[r][c][i] != -1 &&
                    nturns[r][c][i] < mint)
                        mint = nturns[r][c][i];
        for (i = 0; i < 4; i++)
                if (nturns[r][c][i] == mint) {
                        int nr = r + r_dir[i], nc = c + c_dir[i];
                        draw_paths(nr, nc, mint, i);
                        npaths += is_draw[nr][nc][i];
                }

        /* print the result
         */
        printf("%d paths, %d points, %d turns\n", npaths, level[r][c], mint);
        for (i = 0; i <= rows; i++)
                printf("%s", G[i]);
}

int
main(int argc, char **argv)
{
        int is_end = 0, i = 0;

        while (1) {
                start.row = start.col = -1;
                stop.row = stop.col = -1;
                memset(G, 0, sizeof(G));
                memset(level, 0, sizeof(level));
                memset(queue, 0, sizeof(queue));
                memset(nturns, 0xFF, sizeof(nturns));
                memset(is_draw, 0, sizeof(is_draw));

                /* read in the graph, and set start and stop point
                 */
                for (i = 0; ; i++) {
                        if (fgets(G[i], sizeof(G[0]), stdin) == NULL) {
                                is_end = 1;
                                break;
                        }
                        if (G[i][0] == '_')
                                break;
                        char *m = NULL;
                        m = strchr(G[i], '@');
                        if (m == NULL)
                                continue;
                        if (start.row == -1) {
                                start.row = i;
                                start.col = m - G[i];
                        } else {
                                stop.row = i;
                                stop.col = m - G[i];
                                continue;
                        }
                        m = strchr(m + 1, '@');
                        if (m == NULL)
                                continue;
                        stop.row = i;
                        stop.col = m - G[i];
                }
                rows = i;
                if (is_end == 1)
                        break;
                BFS();
        }

        return 0;
}

