/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define ARRSZ 40

#define SKIP_SPACE(line, i) \
        while (isspace((line)[i])) \
                (i)++; \
        if ((line)[i] == '\0') \
                break; \

typedef char int8;

int8 edges[ARRSZ][ARRSZ];       /* adjacent list not matrix
                                   edges[i][j] store the node adjacent to i */
char vars[ARRSZ];               /* the name of each node */
int8 map2idx[ARRSZ];            /* map char to index in @vars */
int8 indeg[ARRSZ];
int8 visit[ARRSZ];
int8 nadjs[ARRSZ];              /* number of adjacent nodes */
int8 nvars;                     /* totoal number of variables */
char path[ARRSZ];

void
print(void)
{
        int i = 0;
        for(; i < nvars; i++)
                putchar(path[i]);
        putchar('\n');
}

/**
 * the caller should call this function with @level = 0
 */
void
topsort(int level)
{
        int i = 0, j = 0;

        if (level == nvars) {
                print();
                return;
        }

        for (i = 0; i < nvars; i++) {
                if (indeg[i] != 0 || visit[i] == 1)
                        continue;
                path[level] = vars[i];

                visit[i] = 1;
                for (j = 0; j < nadjs[i]; j++)
                        indeg[edges[i][j]]--;

                topsort(level + 1);

                visit[i] = 0;
                for (j = 0; j < nadjs[i]; j++)
                        indeg[edges[i][j]]++;
        }
}

int
main(int argc, char **argv)
{
        char fstline[256], sndline[256];
        char sort[ARRSZ];
        int i = 0, j = 0;
        int blank = 0;

        while (1) {
                /* read input
                 */
                if (fgets(fstline, sizeof(fstline), stdin) == NULL)
                        break;
                fgets(sndline, sizeof(sndline), stdin);


                /* initialize
                 */
                nvars = 0;
                memset(edges, 0, sizeof(edges));
                memset(vars, 0, sizeof(vars));
                memset(map2idx, 0, sizeof(map2idx));
                memset(indeg, 0, sizeof(indeg));
                memset(visit, 0, sizeof(visit));
                memset(nadjs, 0, sizeof(nadjs));
                memset(path, 0, sizeof(path));
                memset(sort, 0, sizeof(sort));

                /* parser first line to get all variables
                 */
                for (i = 0; fstline[i] != '\0'; i++) {
                        SKIP_SPACE(fstline, i);
                        vars[nvars++] = fstline[i];
                }
                /* sort variables
                 */
                for (i = 0; i < nvars; i++)
                        sort[vars[i] - 'a'] = 1;
                nvars = 0;
                for (i = 0; i < ARRSZ; i++) {
                        if (sort[i] == 1) {
                                vars[nvars] = i + 'a';
                                map2idx[i] = nvars;
                                nvars++;
                        }
                }
                /* parser second line to generate the graph
                 */
                for (i = 0; sndline[i] != '\0'; i++) {
                        char b = 0, e = 0;
                        int bi = 0, ei = 0;

                        SKIP_SPACE(sndline, i);
                        b = sndline[i++];
                        SKIP_SPACE(sndline, i);
                        e = sndline[i];
                        
                        bi = map2idx[b - 'a'];
                        ei = map2idx[e - 'a'];
                        edges[bi][nadjs[bi]++] = ei;
                        indeg[ei]++;
                }

                /* SHIT! I got rejected 3 times just because the blank line.
                 */
                if (blank)
                        putchar('\n');
                blank = 1;
                topsort(0);
        }

        return 0;
}

