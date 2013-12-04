/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int G[128][128];

/**
 * Return 1 if there's next permutation, 0 otherwise
 */
int
next_perm(char *str, int len)
{
        int i = 0, j = 0, k = 0;

        /* find the longest subseq in reverse order
         */
        for (i = len - 2; i >= 0; i--)
                if (str[i] < str[i + 1])
                        break;
        if (i == -1)
                return 0;

        /* reverse the order of subseq [i + 1, len - 1]
         */
        for (j = i + 1, k = len - 1; j < k; j++, k--) {
                char t = str[j];
                str[j] = str[k];
                str[k] = t;
        }

        /* find the first one in [i + 1, len - 1] that is
         * bigger than str[i]
         */
        for (j = i + 1; j < len; j++)
                if (str[j] > str[i])
                        break;

        /* swap str[j] and str[i], and we get the next
         * permutaion
         */
        char t = str[j];
        str[j] = str[i];
        str[i] = t;

        return 1;
}

int
get_bw(char *str, int len)
{
        int i = 0, j = 0;
        int max = 0;

        for (i = 0; i < len; i++) {
                for (j = len - 1; j >= i + 1; j--) {
                        if (G[str[i]][str[j]] == 1) {
                                if (j - i > max)
                                        max = j - i;
                                break;
                        }
                }
        }

        return max;
}

int
main(int argc, char **argv)
{
        int i = 0, len = 0;
        char G_str[1024];
        char min_perm[9], perm[9];
        int min_bw = 0;
        int exist[128];

        while (1) {
                scanf("%s", G_str);
                if (G_str[0] == '#')
                        break;
                
                memset(G, 0, sizeof(G));
                memset(exist, 0, sizeof(exist));
                memset(perm, 0, sizeof(perm));
                memset(min_perm, 0, sizeof(min_perm));
                len = 0;

                for (i = 0; G_str[i] != '\0'; i++) {
                        if (G_str[i] >= 'A' && G_str[i] <= 'Z'
                            && exist[G_str[i]] == 0) {
                                int k = 0;
                                for (k = len - 1; k >= 0 && perm[k] > G_str[i]; k--)
                                        perm[k + 1] = perm[k];
                                perm[k + 1] = G_str[i];
                                len++;
                                exist[G_str[i]] = 1;
                        }
                }
                i = 0;
                while (G_str[i] != '\0') {
                        char b = G_str[i], t = 0;

                        i += 2;
                        while (G_str[i] != ';' && G_str[i] != '\0') {
                                t = G_str[i++];
                                G[b][t] = G[t][b] = 1;
                        }
                        if (G_str[i] == ';')
                                i++;
                }

                min_bw = get_bw(perm, len);
                memcpy(min_perm, perm, len);
                while (next_perm(perm, len)) {
                        int bw = get_bw(perm, len);
                        if (bw < min_bw) {
                                min_bw = bw;
                                memcpy(min_perm, perm, len);
                        }
                }
                for (i = 0; i < len; i++)
                        printf("%c ", min_perm[i]);
                printf("-> %d\n", min_bw);
        }

        return 0;
}

