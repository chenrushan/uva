/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_STR_LEN 20

char string[MAX_STR_LEN];
int len;
char perm[MAX_STR_LEN];
char used[MAX_STR_LEN];

void
DFS(int idx)
{
        char lstc = 0;
        int i = 0;

        if (idx == len) {
                printf("%s\n", perm);
                return;
        }

        for (i = 0; i < len; i++) {
                if (string[i] == lstc || used[i] == 1)
                        continue;
                used[i] = 1;
                perm[idx] = string[i];
                DFS(idx + 1);
                used[i] = 0;
                lstc = perm[idx];
        }
}

int
main(int argc, char **argv)
{
        int tc = 0, i = 0, j = 0;

        scanf("%d\n", &tc);
        while (tc--) {
                scanf("%s", string);
                len = strlen(string);

                for (i = 1; i < len; i++) {
                        char k = string[i];
                        for (j = i - 1; j >= 0; j--) {
                                if (string[j] <= k)
                                        break;
                                string[j + 1] = string[j];
                        }
                        string[j + 1] = k;
                }
                memset(perm, 0, sizeof(perm));
                memset(used, 0, sizeof(used));
                DFS(0);
                putchar('\n');
        }

        return 0;
}

