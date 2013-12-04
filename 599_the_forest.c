/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char G[26][26];
char exist[26], visit[26];

void
DFS(int v)
{
        int i = 0;

        visit[v] = 1;
        for (i = 0; i < 26; i++) {
                if (G[v][i] && ! visit[i])
                        DFS(i);
        }
}

int
main(int argc, char **argv)
{
        int tc = 0, i = 0, j = 0;
        char line[1000];

        scanf("%d\n", &tc);
        while (tc--) {
                memset(G, 0, sizeof(G));
                memset(exist, 0, sizeof(exist));
                memset(visit, 0, sizeof(visit));

                while (1) {
                        char b = 0, e = 0;

                        fgets(line, sizeof(line), stdin);
                        if (line[0] == '*')
                                break;
                        sscanf(line, "(%c,%c)", &b, &e);
                        G[b - 'A'][e - 'A'] = 1;
                        G[e - 'A'][b - 'A'] = 1;
                }
                fgets(line, sizeof(line), stdin);
                for (i = 0; line[i] != '\n'; i++) {
                        if (line[i] == ',')
                                continue;
                        exist[line[i] - 'A'] = 1;
                }

                int nt = 0, na = 0;

                for (i = 0; i < 26; i++) {
                        if (! (exist[i] && ! visit[i]))
                                continue;
                        for (j = 0; j < 26; j++)
                                if (G[i][j])
                                        break;
                        if (j == 26) {
                                na++;
                        } else {
                                nt++;
                                DFS(i);
                        }
                }
                printf("There are %d tree(s) and %d acorn(s).\n", nt, na);
        }

        return 0;
}

