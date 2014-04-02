/* 
 * Author: Chen Rushan
 * E-Mail: juscodit@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_NNODES 100
#define MAX_NWORDS 200 /* max words in a node */
#define WORD_BUF_SIZE 1 * 1024 * 1024 /* 1M */

struct t_word {
    char *p; /* pointer to word content */
    int n; /* index of next node */
};

struct t_DAG {
    struct t_word DAG[MAX_NNODES][MAX_NWORDS]; /* structure of DAG */
    int nwords[MAX_NNODES]; /* nwords[i] is the number of words in ith node */
    int nnodes; /* number of nodes in DAG */
    char wordBuf[WORD_BUF_SIZE]; /* words are stored here */
    char *wordPtr; /* pointer to current position in @wordBuf */
};

int offset[MAX_NNODES][MAX_NWORDS];
struct t_DAG DAG;

void
DAG_init(struct t_DAG *G)
{
    G->nnodes = 0;
    memset(G->nwords, 0, sizeof(G->nwords));
    G->wordPtr = G->wordBuf;
}

void
DAG_add_word(struct t_DAG *G, int node, char *spec)
{
    char *p = spec;

    while (*p != ':') {
        p += 1;
    }
    *(p++) = '\0';

    int next = atoi(p);

    printf("%s %d\n", spec, next);
}

int
main(int argc, char **argv)
{
    char line[1024];
    int ncases = 0;

    scanf("%d\n", &ncases);

    while (ncases--) {
        memset(offset, 0, sizeof(offset));
        DAG_init(&DAG);

        while (1) {
            if (fgets(line, sizeof(line), stdin) == NULL) {
                break;
            }
            if (line[0] == '\n') {
                break;
            }
        }
    }
}

