/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_STR_LEN 12

int fact[MAX_STR_LEN + 1];

void
generate_all_knuth_perm(char *str, int len)
{
        int i = 0, j = 0, k = 0;
        char used[MAX_STR_LEN] = {};
        char newstr[MAX_STR_LEN] = {};

        for (i = 0; i < fact[len]; i++) {
                int num = i;

                memset(used, 0, sizeof(used));

                /* generate a knuth permutation at index @num
                 */
                for (j = len; j >= 1; j--) {
                        int idx = num % j;

                        /* find the position to put str[j - 1]
                         */
                        for (k = 0; k < len; k++) {
                                if (used[k] == 1)
                                        continue;
                                if (idx != 0) {
                                        idx--;
                                        continue;
                                }
                                newstr[k] = str[j - 1];
                                used[k] = 1;
                                break;
                        }

                        /* the index of knuth permutation for str[0, j - 2]
                         */
                        num /= j;
                }
                printf("%s\n", newstr);
        }
}

int
main(int argc, char **argv)
{
        char str[MAX_STR_LEN];
        int len = 0, i = 0, blank = 0;

        fact[0] = 1;
        for (i = 1; i <= MAX_STR_LEN; i++)
                fact[i] = fact[i - 1] * i;

        while (1) {
                if (scanf("%s", str) == EOF)
                        break;
                len = strlen(str);

                if (blank)
                        putchar('\n');
                blank = 1;
                generate_all_knuth_perm(str, len);
        }

        return 0;
}

