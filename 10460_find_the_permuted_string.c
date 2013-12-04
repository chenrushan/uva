/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_STR_LEN 30

int pos[MAX_STR_LEN];

/**
 * @idx: index of the permutation of str[0, @len - 1]
 * In fact, this function has nothing to do with string itself
 */
void
determine_pos_non_recusive(int idx, int len)
{
        int l = 0, next_idx = 0;

        pos[0] = 1;
        for (l = len; l >= 2; l--) {
                next_idx = idx / l;
                pos[l - 1] = idx % l;
                if (idx % l != 0)
                        next_idx++;
                else
                        pos[l - 1] = l;
                idx = next_idx;
        }
}

void
determine_pos(int idx, int len)
{
        int next_idx = 0;

        if (len == 1) {
                pos[0] = 1;
                return;
        }
        next_idx = idx / len;
        pos[len - 1] = idx % len;
        if (idx % len != 0)
                next_idx++;
        else
                pos[len - 1] = len;

        determine_pos(next_idx, len - 1);
}

int
main(int argc, char **argv)
{
        int tc = 0;
        char str[MAX_STR_LEN], newstr[MAX_STR_LEN];
        char used[MAX_STR_LEN];
        int len = 0, idx = 0, i = 0, j = 0, k = 0;
        
        scanf("%d", &tc);
        while (tc--) {
                scanf("%s", str);
                len = strlen(str);
                scanf("%d", &idx);

                memset(used, 0, sizeof(used));
                determine_pos_non_recusive(idx, len);

                for (i = len - 1; i >= 0; i--) {
                        k = 0;
                        for (j = 0; j < len; j++) {
                                if (! used[j] && ++k == pos[i])
                                        break;
                        }
                        newstr[j] = str[i];
                        used[j] = 1;
                }
                newstr[len] = '\0';
                printf("%s\n", newstr);
        }

        return 0;
}

