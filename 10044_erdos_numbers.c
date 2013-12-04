/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define SKIP_SPACE(str, i) \
        do { \
                while (isspace((str)[(i)]) || (str)[i] == ',') \
                        (i)++; \
        } while (0)

#define MAX_NAMES 5001          /* maximum number of names */
#define MAX_NAME_LEN 128        /* average name length */
#define NBUCKETS 10993          /* hash bucket size */
#define PAPER_LEN 20001

/* for graph
 */
short G[MAX_NAMES][600];        /* adjacent list */
int nadjs[MAX_NAMES];
char visit[MAX_NAMES];
int erdos_num[MAX_NAMES];
int erdos;

/* for hashtable
 */ 
int names[MAX_NAMES];           /* storing index of @strbuf */
char strbuf[MAX_NAMES * MAX_NAME_LEN];  /* storing name */
int buckets[NBUCKETS];          /* initially, they are all -1 */
int chain[MAX_NAMES];           /* initially, they are all -1 */
int names_idx;                  /* index of names, used when inserting
                                   to hash table */
int strbuf_idx;                 /* index of strbuf, used when inserting
                                   name to strbuf */

char paper[PAPER_LEN];
int nids[MAX_NAMES];
int nids_idx;

int queue[MAX_NAMES];         /* used for getting the erdos number */
char fstname[MAX_NAME_LEN];
char sndname[MAX_NAME_LEN];
char fullname[MAX_NAME_LEN];

unsigned
hash(void *string)
{
        unsigned hval = 0;
        unsigned g = 0;
        const char *str = (const char *) string;

        hval = 0;
        while (*str != '\0') {
                hval <<= 4;
                hval += (unsigned long) *str++;
                g = hval & ((unsigned long) 0xf << (32 - 4));
                if (g != 0) {
                        hval ^= g >> (32 - 8);
                        hval ^= g;
                }
        }
        
        return hval % NBUCKETS;
}

/**
 * Return -1 if not found, otherwise, the number for @name is returned
 */
int
hash_search(char *name)
{
        int h = hash(name), n = buckets[h];

        while (n != -1) {
                if (strcmp(name, strbuf + names[n]) == 0)
                        return n;
                n = chain[n];
        }
        return -1;
}

/**
 * Only call this function after you're sure there's no @name in hash table
 */
int
hash_insert(char *name)
{
        int h = hash(name), p = buckets[h];
        int l = strlen(name) + 1; /* 1 for '\0' */
        
        buckets[h] = names_idx;
        chain[names_idx] = p;
        names[names_idx] = strbuf_idx;
        memcpy(strbuf + strbuf_idx, name, l);

        strbuf_idx += l;
        names_idx++;

        return names_idx - 1;
}

void
get_erdos_number()
{
        int i = 0, t = 0;

        queue[t++] = erdos;
        visit[erdos] = 1;
        erdos_num[erdos] = 0;

        while (i != t) {
                int id = queue[i++], j = 0;

                for (j = 0; j < nadjs[id]; j++) {
                        int k = G[id][j];
                        if (visit[k] == 0) {
                                queue[t++] = k;
                                visit[k] = 1;
                                erdos_num[k] = erdos_num[id] + 1;
                        }
                }
        }
}

int
main(int argc, char **argv)
{
        int tc = 0, P = 0, N = 0;
        int i = 0, j = 0, l = 0, t = 0, S = 1, k = 0;
        char c;

        scanf("%d\n", &tc);
        while (tc--) {
                /* initialize
                 */
                memset(G, 0, sizeof(G));
                memset(nadjs, 0, sizeof(nadjs));
                memset(visit, 0, sizeof(visit));
                memset(erdos_num, 0xFF, sizeof(erdos_num));
                memset(names, 0, sizeof(names));
                memset(strbuf, 0, sizeof(strbuf));
                memset(buckets, 0xFF, sizeof(buckets));
                memset(chain, 0xFF, sizeof(chain));
                names_idx = 0;
                strbuf_idx = 0;

                /* read paper
                 */
                scanf("%d%d\n", &P, &N);
                for (i = 0; i < P; i++) {
                        nids_idx = 0;
                        fgets(paper, sizeof(paper), stdin);

                        j = 0;
                        while (1) {
                                /* get first name
                                 */
                                SKIP_SPACE(paper, j);
                                k = 0;
                                for (; ! isspace(paper[j]) && paper[j] != ','
                                       && paper[j] != ':'; j++)
                                        fstname[k++] = paper[j];
                                fstname[k] = '\0';
                                SKIP_SPACE(paper, j);
                                if (paper[j] == ':')
                                        break;

                                /* get last name
                                 */
                                k = 0;
                                for (; ! isspace(paper[j]) && paper[j] != ','
                                       && paper[j] != ':'; j++)
                                        sndname[k++] = paper[j];
                                sndname[k] = '\0';
                                SKIP_SPACE(paper, j);

                                int id = 0;
                                sprintf(fullname, "%s, %s", fstname, sndname);
                                if ((id = hash_search(fullname)) == -1)
                                        id = hash_insert(fullname);
                                nids[nids_idx++] = id;

                                if (paper[j] == ':')
                                        break;
                        }

                        /* constructing graph
                         */
                        for (k = 0; k < nids_idx; k++) {
                                for (j = k + 1; j < nids_idx; j++) {
                                        int i1 = nids[k], i2 = nids[j];
                                        G[i1][nadjs[i1]++] = i2;
                                        G[i2][nadjs[i2]++] = i1;
                                }
                        }
                }

                /* get the erdos number
                 */
                erdos = hash_search("Erdos, P.");
                if (erdos != -1)
                        get_erdos_number();

                /* print result
                 */
                printf("Scenario %d\n", S++);

                char name[MAX_NAME_LEN + 1];
                for (i = 0; i < N; i++) {
                        int id = 0;
                        char *com = NULL;

                        fgets(name, sizeof(name), stdin);
                        sscanf(name, "%s%s", fstname, sndname);
                        com = strrchr(fstname, ',');
                        if (com)
                                *com = '\0';
                        sprintf(fullname, "%s, %s", fstname, sndname);
                        id = hash_search(fullname);
                        if (id == -1 || erdos_num[id] == -1)
                                printf("%s infinity\n", fullname);
                        else
                                printf("%s %d\n", fullname, erdos_num[id]);
                }
        }

        return 0;
}

