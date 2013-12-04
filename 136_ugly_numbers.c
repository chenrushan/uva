/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

int
min(int i1, int i2, int i3)
{
        int t = i1 < i2 ? i1 : i2;
        return t < i3 ? t : i3;
}

/**
 * Generate the first @cnt ugly numbers, and store'em into @arr
 */
void
gen_ugly_numbers(int *arr, int cnt)
{
        /* The following 3 index point to the first one that's
         * bigger than the last integer in @arr after multipling
         * by 2, 3, or 5
         */
        int i2 = 0, i3 = 0, i5 = 0;
        int lst = 0;

        arr[0] = 1;
        lst = 1;

        while (lst < cnt) {
                int m = min(arr[i2] * 2, arr[i3] * 3, arr[i5] * 5);
                arr[lst++] = m;

                while (arr[i2] * 2 <= arr[lst - 1])
                        i2++;
                while (arr[i3] * 3 <= arr[lst - 1])
                        i3++;
                while (arr[i5] * 5 <= arr[lst - 1])
                        i5++;
        }
}

int
main(int argc, char **argv)
{
        int i = 0, n = 0;

        /*
        n = 1;
        for (i = 2; i < INT_MAX; i++) {
                int k = i;
                while (k % 2 == 0)
                        k /= 2;
                while (k % 3 == 0)
                        k /=3;
                while (k % 5 == 0)
                        k /=5;
                if (k == 1) {
                        n++;
                        printf("%d ", i);
                        if (n == 1500) {
                                putchar('\n');
                                printf("The 1500'th ugly number is %d.\n", i);
                                break;
                        }
                }
        }
        */

        int arr[1500];
        gen_ugly_numbers(arr, sizeof(arr) / sizeof(int));
        printf("The 1500'th ugly number is %d.\n", arr[1499]);
        /* printf("The 1500'th ugly number is 859963392.\n"); */

        return 0;
}

