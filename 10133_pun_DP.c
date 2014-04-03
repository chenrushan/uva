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

    int offset[MAX_NNODES][MAX_NWORDS];
};

struct t_DAG DAG;

void
DAG_init(struct t_DAG *G)
{
    G->nnodes = 0;
    memset(G->nwords, 0, sizeof(G->nwords));
    G->wordPtr = G->wordBuf;
    memset(G->offset, 0, sizeof(G->offset));
}

void
DAG_set_nnode(struct t_DAG *G, int node)
{
    if (node >= G->nnodes) {
        G->nnodes = node + 1;
    }
}

void
DAG_add_word(struct t_DAG *G, int node, char *word)
{
    char *p = word;

    while (*p != ':') {
        p += 1;
    }
    *(p++) = '\0';

    int next = atoi(p);
    if (*p == '$') {
        next = -1;
    }

    /* set DAG structure */
    G->DAG[node][G->nwords[node]].p = G->wordPtr;
    G->DAG[node][G->nwords[node]].n = next;
    G->nwords[node] += 1;

    /* copy word content */
    memcpy(G->wordPtr, word, p - word);
    G->wordPtr += (p - word);
}

void
DAG_print(struct t_DAG *G)
{
    int i = 0, j = 0;

    for (i = 0; i < G->nnodes; ++i) {
        printf("node: %d\n", i);
        for (j = 0; j < G->nwords[i]; ++j) {
            printf("\t%s:%d\n", G->DAG[i][j].p, G->DAG[i][j].n);
        }
    }
}

void
DAG_build_offset(struct t_DAG *G)
{
    int i = 0, j = 0;
    int count[MAX_NNODES] = {};

    /* initialize */
    i = G->nnodes - 1;
    for (j = 0; j < G->nwords[i]; ++j) {
        G->offset[i][j] = 1;
        count[i] += 1;
    }

    for (i = G->nnodes - 2; i >= 0; --i) {
        for (j = 0; j < G->nwords[i]; ++j) {
            if (G->DAG[i][j].n == -1) {
                G->offset[i][j] = 1;
                count[i] += 1;
            } else {
                G->offset[i][j] = count[G->DAG[i][j].n];
                count[i] += count[G->DAG[i][j].n];
            }
        }
    }

    /* turn count to offset */
    for (i = G->nnodes - 1; i >= 0; --i) {
        int off = 0;
        for (j = 0; j < G->nwords[i]; ++j) {
            int c = G->offset[i][j];
            G->offset[i][j] = off;
            off += c;
        }
    }

    /*
    for (i = G->nnodes - 1; i >= 0; --i) {
        for (j = 0; j < G->nwords[i]; ++j) {
            printf("%d ", G->offset[i][j]);
        }
        printf("\n");
    }
    */
}

int
DAG_get_pun(struct t_DAG *G, int ord)
{
    int node = 0;
    int i = 0, j = 0;

    ord -= 1;
    while (node != -1) {
        for (j = 0; j < G->nwords[node]; ++j) {
            if (ord < G->offset[node][j]) {
                break;
            }
        }
        j -= 1;

        /* print pun */
        if (node != 0) {
            printf("-");
        }
        printf("%s", G->DAG[node][j].p);

        /* move to next node */
        ord -= G->offset[node][j];
        node = G->DAG[node][j].n;
    }
    printf("\n");
}

int
main(int argc, char **argv)
{
    char line[1024];
    int ncases = 0;

    scanf("%d\n", &ncases);

    while (ncases--) {
        DAG_init(&DAG);
        int node = 0;

        while (1) {
            if (fgets(line, sizeof(line), stdin) == NULL) {
                break;
            }

            /* node */
            if (strlen(line) == 4 && line[2] == ':') {
                node = atoi(line);
                DAG_set_nnode(&DAG, node);

            /* word list */
            } else if (line[0] == ' ' && line[1] == ' ') {
                /* p points to the first non-space char */
                char *p = line + 2, *q = p;

                while (*p != '\n') {
                    /* move q to space */
                    while (*q != ' ' && *q != '\n') {
                        q += 1;
                    }
                    char c = *q;
                    *(q++) = '\0';

                    /* now p points to the word spec */
                    DAG_add_word(&DAG, node, p);

                    if (c == '\n') {
                        break;
                    }
                    /* move q to next non-space char */
                    while (*q == ' ') {
                        q += 1;
                    }
                    p = q;
                }

            /* query */
            } else {
                break; /* query found */
            }
        }

        DAG_build_offset(&DAG);

        while (1) {
            int qry = atoi(line);
            DAG_get_pun(&DAG, qry);

            if (fgets(line, sizeof(line), stdin) == NULL
                || line[0] == '\n') {
                break;
            }
        }

        if (ncases != 0) {
            printf("\n");
        }

    }
}

