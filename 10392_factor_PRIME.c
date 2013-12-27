/* 
 * Author: Chen Rushan
 * E-Mail: juscodit@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX 1000001

char not_a_prime[MAX];

int main(int argc, char **argv)
{
    int i = 0, j = 0;

    /* initialize not_a_prime */
    not_a_prime[1] = 1;
    not_a_prime[2] = 0;
    for (i = 2; i < MAX; ++i) {
        if (! not_a_prime[i]) {
            for (j = i + i; j < MAX; j += i) {
                not_a_prime[j] = 1;
            }
        }
    }

    long long num = 0;
    while (scanf("%lld", &num) != EOF) {
        if (num < 0) {
            break;
        }
        for (i = 2; i < MAX; ++i) {
            if (not_a_prime[i]) {
                continue;
            }
            while (num % i == 0) {
                num /= i;
                printf("    %d\n", i);
            }
        }
        if (num != 1) {
            printf("    %lld\n", num);
        }
        printf("\n");
    }

    return 0;
}

