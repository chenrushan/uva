/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int
gcd(int a, int b)
{
        int t = 0;

        while (b != 0) {
                t = b;
                b = a % b;
                a = t;
        }

        return a;
}

int
main(int argc, char **argv)
{
        int a = 0, b = 0;

        while (1) {
                if (scanf("%d %d", &a, &b) == EOF)
                        break;
                int g = gcd(a, b);
                printf("%10d%10d    ", a, b);
                if (g == 1)
                        printf("Good Choice\n");
                else
                        printf("Bad Choice\n");
                putchar('\n');
        }

        return 0;
}

