/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int arr[105][105];
int subarr[105] = {
        0, -1, 2, -10, 3, -1, 0
};
int max;
int N;

/**
 * Return the max value continuous subsequenc of array @subarr
 */
int
get_max()
{
        /* m1 for max value ending at i
         * m2 for max value so far
         */
        int i = 0, m1 = 0, m2 = 0;

        for (i = 0; i < N; i++) {
                int t = m1 + subarr[i];
                m1 = t > 0 ? t : 0;
                m2 = t > m2 ? t : m2;
        }

        return m2;
}

int
main(int argc, char **argv)
{
        int i = 0, j = 0, k = 0;

        scanf("%d", &N);

        for (i = 0; i < N; i++)
                for (j = 0; j < N; j++)
                        scanf("%d", &arr[i][j]);

        /* define a sub array from row i to row j
         */
        for (i = 0; i < N; i++) {
                memset(subarr, 0, sizeof(subarr));
                for (j = i; j < N; j++) {
                        for (k = 0; k < N; k++)
                                subarr[k] += arr[j][k];
                        int m = get_max();
                        if (m > max)
                                max = m;
                }
        }

        printf("%d\n", max);

        return 0;
}

