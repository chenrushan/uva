/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_STR_LEN 12

/* 
 * The principle of generating knuth permutation can be demonstrated
 * by this example:
 *
 * Suppose we want to generate knuth permutation for 0123, and at some
 * point we get permutation 2103, then we can get the next permutation
 * by following these steps:
 *
 * First check if the last element is already at the end, if not, we
 * just move this element one step forward, so if the permutation is
 * 2310, then we can get its next 2130 with little effect. In the case
 * of 2103, 3 is already at the end, so its next permutation will have
 * 3 at the beginning, then we recursively find the next permutation
 * of 210 in the same way as above, then append it after 3, which
 * gives us 3120, and this is the next permutation of 2103.
 */

/**
 * @idxpos[i] is the position of index i in @idxarr, so
 * idxpos[5] = 0 means idxarr[0] = 5
 *
 * Return 0 if no next permutation, 1 otherwise.
 */
int
next_knuth_perm(int *idxarr, int *idxpos, int end)
{
        if (end == 0)
                return 0;

        if (idxpos[end] != end) {
                int endp = idxpos[end];
                idxarr[endp] = idxarr[endp + 1];
                idxarr[endp + 1] = end;
                idxpos[idxarr[endp]] = endp;
                idxpos[end] = endp + 1;

                return 1;
        }

        int ret = 0, i = 0;

        ret = next_knuth_perm(idxarr, idxpos, end - 1);
        if (ret == 0)
                return 0;
        memmove(idxarr + 1, idxarr, end * sizeof(int));
        idxarr[0] = end;
        for (i = 0; i < end; i++)
                idxpos[i]++;
        idxpos[end] = 0;

        return 1;
}

int
main(int argc, char **argv)
{
        int len = 0;
        int idxarr[MAX_STR_LEN], idxpos[MAX_STR_LEN];
        char str[MAX_STR_LEN];
        int blank = 0;

        while (1) {
                if (scanf("%s", str) == EOF)
                        break;
                len = strlen(str);

                int i = 0;
                for (i = 0; i < len; i++) {
                        idxarr[i] = len - i - 1;
                        idxpos[i] = len - i - 1;
                }

                if (blank)
                        putchar('\n');
                blank = 1;
                do {
                        for (i = 0; i < len; i++)
                                printf("%c", str[idxarr[i]]);
                        putchar('\n');
                } while (next_knuth_perm(idxarr, idxpos, len - 1));
        }

        return 0;
}

