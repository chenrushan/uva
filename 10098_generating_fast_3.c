/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_STR_LEN 20

/**
 * The principle of this function can be demonstrated using
 * the following example:
 * Suppose we want to find the next permutation of "acedb",
 * the following steps are taken:
 * 1. find the longest sequence in descending order starting
 *    from @end to 0, here it's "edb"
 * 2. reverse the order of this sequence, so we get "ac|bde".
 * 3. find the first one bigger than 'c' in "bde", which is 'd'
 * 4. swap 'c' and 'd', and we get "ad|bce", and this is the
 *    so-called next permutation
 * So if the whole string is in decsending order, then there's
 * no next permutation
 *
 * Return 0 if there's no next permutation, 1 otherwise
 * @end is the index of last character in @str, if @str has
 * no character, @end == -1
 */
int
next_perm(char *str, int end)
{
        int i = end, j = 0, k = 0;
        char t = 0;

        /* TODO:
         if (end == -1)
                return 0;
         */

        /* after this while, @i points to the last one
         * in reverse order
         */
        while (i - 1 >= 0 && str[i - 1] >= str[i])
                i--;

        /* if there's no more next permutation
         */
        if (i == 0)
                return 0;

        /* reverse the order of @str[i, end] so that it's
         * in acsending order
         */
        for (j = i, k = end; j <= (i + end) / 2; j++, k--) {
                t = str[j];
                str[j] = str[k];
                str[k] = t;
        }

        /* after this for, @j points to the first char
         * that is absolutely bigger than @str[i - 1]
         */
        for (j = i; j <= end; j++)
                if (str[j] > str[i - 1])
                        break;

        /* swap @j and @i - 1, and we get the next permutation
         */
        t = str[j];
        str[j] = str[i - 1];
        str[i - 1] = t;

        return 1;
}

int
main(int argc, char **argv)
{
        int tc = 0, i = 0, j = 0;
        char string[MAX_STR_LEN] = {};
        int len = 0;

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

                do {
                        printf("%s\n", string);
                } while (next_perm(string, len - 1));

                putchar('\n');
        }

        return 0;
}

