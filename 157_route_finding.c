/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_VCNT 513

typedef char int8;

struct vertex {
        char vname[2];
        int vnum;
};

struct vertex num2name[MAX_VCNT];
struct vertex vert[MAX_VCNT];
int8 G[MAX_VCNT][MAX_VCNT];
short P[MAX_VCNT][MAX_VCNT];
int next_vnum;
int path[MAX_VCNT];
int path_len;

int cstats[MAX_VCNT];

/**
 * Search @vert, return the index of @vert
 */
int
bsrch(char *n)
{
        int b = 0, e = next_vnum - 1, m = 0;
        int c = 0;

        while (b <= e) {
                m = (b + e) / 2;
                c = memcmp(vert[m].vname, n, 2);

                if (c > 0)
                        e = m - 1;
                else if (c < 0)
                        b = m + 1;
                else
                        return m;
        }

        return -1;
}

void
floyd(void)
{
        int i = 0, j = 0, k = 0;
        int N = next_vnum;

        for (k = 0; k < N; k++) {
                for (i = 0; i < N; i++) {
                        for (j = 0; j < N; j++) {
                                if (G[i][k] == -1 || G[k][j] == -1)
                                        continue;
                                int t = G[i][k] + G[k][j];
                                if (G[i][j] == -1 || t < G[i][j]) {
                                        G[j][i] = G[i][j] = t;
                                        P[j][i] = P[i][j] = k;
                                }
                        }
                }
        }
}

void
set_union(int r1, int r2)
{
        cstats[r2] = r1;
}

int
set_find(int i)
{
        while (cstats[i] != -1)
                i = cstats[i];
        return i;
}

void
get_path(int s, int d)
{
        if (P[s][d] == -1) {
                return;
        } else {
                int k = P[s][d];
                get_path(s, k);
                path[path_len++] = k;
                get_path(k, d);
        }
}

int
main(int argc, char **argv)
{
        int nrut = 0, pvnum = -1, i = 0, j = 0, k = 0;
        char line[1024];
        
        memset(G, 0xFF, sizeof(G));
        memset(P, 0xFF, sizeof(P));
        memset(cstats, 0xFF, sizeof(cstats));

        scanf("%d\n", &nrut);

        /* construct graph
         */
        while (nrut--) {
                fgets(line, sizeof(line), stdin);
                i = 2;

                while (line[i] != '\n') {
                        char n[2];

                        n[0] = line[0];
                        if (line[i] == '=') {
                                n[0] = line[++i];
                                n[1] = line[++i];
                        } else {
                                n[1] = line[i];
                        }
                        i++;

                        int idx = bsrch(n);

                        /* add new vertex if necessary
                         */
                        if (idx == -1) {
                                /* add to @vert[]
                                 */
                                for (j = next_vnum - 1; j >= 0; j--)
                                        if (memcmp(vert[j].vname, n, 2) > 0)
                                                memcpy(vert + j + 1, vert + j,
                                                       sizeof(vert[j]));
                                        else
                                                break;
                                memcpy(vert[j + 1].vname, n, 2);
                                vert[j + 1].vnum = next_vnum;

                                /* add to @num2name[]
                                 */
                                memcpy(num2name[next_vnum].vname, n, 2);
                                num2name[next_vnum].vnum = next_vnum;

                                next_vnum++;
                                idx = j + 1;
                        }

                        /* update graph
                         */
                        int vnum = vert[idx].vnum;
                        if (pvnum == -1) {
                                pvnum = vnum;
                                G[vnum][vnum] = 0;
                                continue;
                        }
                        if (n[0] != line[0]) { /* a connecting station */
                                int r1 = set_find(pvnum), r2 = set_find(vnum);
                                if (r1 != r2)
                                        set_union(r1, r2);
                        } else { /* a normal station */
                                G[pvnum][vnum] = G[vnum][pvnum] = 1;
                                pvnum = vnum;
                        }
                        G[vnum][vnum] = 0;
                }
                pvnum = -1;
        }

        /* deal with those connecting stations
         */
        for (i = 0; i < next_vnum; i++) {
                for (j = 0; j < next_vnum; j++) {
                        int r1 = set_find(i), r2 = set_find(j);
                        if (r1 == r2)
                                G[i][i] = G[j][i] = 3;
                }
        }

        floyd();

        /* read queries
         */
        char sn[2], dn[2];
        int sv = 0, dv = 0;

        while (1) {
                fgets(line, sizeof(line), stdin);
                if (line[0] == '#' && line[1] == '\n')
                        break;

                sn[0] = line[0], sn[1] = line[1];
                dn[0] = line[2], dn[1] = line[3];

                if (memcmp(sn, dn, 2) == 0) {
                        printf("  0: %c%c\n", sn[0], sn[1]);
                        continue;
                }

                sv = vert[bsrch(sn)].vnum;
                dv = vert[bsrch(dn)].vnum;

                printf("%3d: ", G[sv][dv]);

                path_len = 0;
                path[path_len++] = sv;
                get_path(sv, dv);
                path[path_len++] = dv;

                char pr = 0;
                for (i = 0; i < path_len; i++) {
                        int v = path[i];
                        if (pr != 0) {
                                if (num2name[v].vname[0] != pr) {
                                        printf("=%c%c", num2name[v].vname[0],
                                                        num2name[v].vname[1]);
                                        pr = num2name[v].vname[0];
                                } else {
                                        printf("%c", num2name[v].vname[1]);
                                }
                        } else {
                                printf("%c%c", num2name[v].vname[0],
                                               num2name[v].vname[1]);
                                pr = num2name[v].vname[0];
                        }
                }
                putchar('\n');
        }

        return 0;
}

