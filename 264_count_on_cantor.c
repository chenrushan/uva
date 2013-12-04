/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define COUNT_SZ 5000

int count[COUNT_SZ];

int
bsrch(int k)
{
        int b = 1, e = COUNT_SZ - 1, m = 0;

        while (b <= e) {
                m = (b + e) / 2;
                if (count[m] < k)
                        b = m + 1;
                else if (count[m] > k)
                        e = m - 1;
                else
                        return m;
        }

        /* @b now is the first one bigger than @k
         * This can be proved:
         * When getting here, we know the above while loop
         * is terminated by violating the condition "b <= e",
         * and there are only two possible relationship between
         * b and e before the condition is violated, that is
         * b = e - 1 and b = e, now consider these 2 cases:
         * 1. if b = e - 1
         *    m = (b + e) / 2 = b
         *    (1) if (c[m] < k), then b will be m + 1 which is
         *    e and this results in the case b = e, and we will
         *    consider this shortly.
         *    (2) if (c[m] > k) which is the same as c[b] > k
         *    then e will be b - 1, and the loop terminates.
         * 
         * 2. if b = e
         *    m = (b + e) / 2 = b = e
         *    (1) if (c[m] < k), then b will be m + 1 which
         *    points to the first one bigger than k, and loop
         *    terminates.
         *    (2) if (c[m] > k), which is same as c[b] > k,
         *    and e will be b - 1, and loop terminates too.
         *
         * So no matter in which situation, the loop will
         * terminate with c[b] being the first one bigger than k
         */
        return b;
}


int
main(int argc, char **argv)
{
        int i = 0;

        /* initialize
         */
        count[1] = 1;
        for (i = 2; i < COUNT_SZ; i++)
                count[i] = i + count[i - 1];

        while (1) {
                int num = 0;

                if (scanf("%d", &num) == EOF)
                        break;
                i = bsrch(num);

                int j = num - count[i - 1];

                if (i % 2 == 0)
                        printf("TERM %d IS %d/%d\n", num, j, i - j + 1);
                else
                        printf("TERM %d IS %d/%d\n", num, i - j + 1, j);
        }

        return 0;
}

