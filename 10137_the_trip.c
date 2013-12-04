/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

/**
 * I got WA at first, since I only consider those costs above average. I forgot
 * the fact that average is calculated by rounding up or down, so the sum of the
 * difference between the bigger cost and average might not be the same as the
 * sum of the difference between smaller cost and averge, and as the problem
 * points out, you should choose the smaller sum. For the input:
 * -------
 * 4
 * 15.00
 * 15.01
 * 3.00
 * 3.01
 * -------
 * both $12.01 and $11.99 are able to equalize all the students' costs within
 * one cent, but you should choose $11.99, since it's smaller.
 *
 * BTW, I avoid floating point arithmatic in this program. Of couse, this is not
 * necessary, if you do use floating point arithmatic, the following method can
 * be used to perform rounding when calculating average:
 *
 * 1. avg = (double)(long)(avg * 100) / 100
 *    However, casting to (long) simply discade the fraction part, so
 *    (long)15.98 is 15 not 16
 *
 * 2. char d[32];
 *    double t = 0;
 *    sprintf(d, "%.2lf\n", avg); 
 *    sscanf(d, "%lf", &t);
 *    
 *    This is a better way which performs correct rounding up and down, so
 *    15.975 will become 15.98
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAXSZ 1001

int costs[MAXSZ];

int
average(int n)
{
        int i = 0, avg = 0, rem = 0;
        long long sum = 0;

        for (i = 0; i < n; i++)
                sum += costs[i];
        avg = sum / n;
        rem = sum - avg * n;
        if (rem + rem >= n)
                avg++;
        return avg;
}

int
main(int argc, char **argv)
{
        int n = 0;
        char cst[128] = {};
        char *p = NULL;

        while (1) {
                /* input costs
                 */
                scanf("%d", &n);
                if (n == 0)
                        break;
                int i = 0, j = 0;
                for (i = 0; i < n; i++) {
                        scanf("%s", cst);
                        p = strchr(cst, '.');
                        *p = *(p + 1);
                        *(p + 1) = *(p + 2);
                        *(p + 2) = '\0';
                        costs[j++] = atoi(cst);
                }

                if (n == 1) {
                        printf("$0.00\n");
                        continue;
                }

                /* calcualte change
                 */
                int avg = average(n);
                int cha1 = 0, cha2 = 0;
                for (i = 0; i < n; i++)
                        if (costs[i] > avg)
                                cha1 += costs[i] - avg;
                        else
                                cha2 += avg - costs[i];

                /* output
                 */
                int change = cha1 < cha2 ? cha1 : cha2;
                sprintf(cst, "%03d", change);
                int l = strlen(cst);
                putchar('$');
                for (i = 0; i < l - 2; i++)
                        putchar(cst[i]);
                putchar('.');
                printf("%s\n", cst + l - 2);
        }

        return 0;
}

