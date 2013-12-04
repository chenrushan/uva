#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 26

typedef unsigned char uint8_t;

uint8_t G[MAX_NODES][MAX_NODES]; /* adjacent list */
uint8_t nadjs[MAX_NODES];
uint8_t visited[MAX_NODES];
uint8_t N; /* number of nodes */

void
DFS(uint8_t b)
{
    uint8_t i = 0;
    visited[b] = 1;
    for (; i < nadjs[b]; ++i) {
        uint8_t e = G[b][i];
        if (visited[e] == 0) {
            DFS(e);
        }
    }
}

int
num_of_connected_comp()
{
    int i = 0;
    int num = 0;

    for (; i < N; ++i) {
        if (visited[i] == 0) {
            DFS(i);
            num += 1;
        }
    }
    return num;
}

int
main(int argc, char **argv)
{
    int ncases = 0;
    char in[20];
    char c;

    scanf("%d\n\n", &ncases);
    while (ncases--) {
        memset(nadjs, 0, MAX_NODES * sizeof(*nadjs));
        memset(visited, 0, MAX_NODES * sizeof(*visited));
        memset(G, 0, MAX_NODES * MAX_NODES * sizeof(G[0][0]));

        if (fgets(in, sizeof(in)/sizeof(in[0]), stdin) == NULL) {
            break;
        }
        N = in[0] - 'A' + 1;

        while (1) {
            if (fgets(in, sizeof(in)/sizeof(in[0]), stdin) == NULL) {
                break;
            }
            if (in[0] == '\n') {
                break;
            }
            uint8_t b = in[0] - 'A', e = in[1] - 'A';
            int i = 0;
            for (i = 0; i < nadjs[b]; ++i) {
                if (G[b][i] == e) {
                    break;
                }
            }
            if (i == nadjs[b]) {
                G[b][nadjs[b]++] = e;
                G[e][nadjs[e]++] = b;
            }
        }
        printf("%d\n", num_of_connected_comp());
        if (ncases != 0) {
            printf("\n");
        }
    }
    return 0;
}

