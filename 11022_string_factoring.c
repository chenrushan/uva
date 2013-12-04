/*
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 * uva: 11022 string factoring
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 100

int weight[MAX_CHARS][MAX_CHARS];

void
DP(char *str, int len)
{
    int i = 0, j = 0, k = 0;

    /* init */
    memset(weight, 0, sizeof(weight));
    for (i = 0; i < len; ++i) {
        weight[i][i] = 1;
    }

    for (i = len - 2; i >= 0; --i) {
        for (j = i; j < len; ++j) {
            /* for str[i..j] */

            int min = weight[i][i] + weight[i + 1][j];
            for (k = i + 1; k <= j; ++k) {
                if (str[k] != str[i]) {
                    continue;
                }
                int l = k - i;
                if (k + l > j + 1 || memcmp(str + k, str + i, l)) {
                    continue;
                }
                int m = k + l;
                for (; m + l <= j + 1; m += l) {
                    if (memcmp(str + m, str + i, l)) {
                        break;
                    }
                }
                int w = weight[i][k - 1] + weight[m][j];
                if (w < min) {
                    min = w;
                }
            }
            weight[i][j] = min;
        }
    }
}

int
main(int argc, char **argv)
{
    char str[MAX_CHARS];
    int len = 0;

    while (1) {
        fgets(str, MAX_CHARS, stdin);
        len = strlen(str);
        str[--len] = '\0';
        if (str[0] == '*') {
            break;
        }
        DP(str, len);

        printf("%d\n", weight[0][len - 1]);
    }

    return 0;
}

