/* 
 * Name: chen rushan
 * mail: chenrslucky@gmail.com
 * uva: 11081
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N_CHARS 60

int count[2][MAX_N_CHARS + 1][MAX_N_CHARS + 1];

void
DP(char *s1, char *s2, char *s3)
{
    int l1 = strlen(s1), l2 = strlen(s2), l3 = strlen(s3);
    int b1 = 0, b2 = 0, b3 = 0;

    /* initialize */
    for (b1 = 0; b1 <= l1; ++b1) {
        for (b2 = 0; b2 <= l2; ++b2) {
            count[l3 & 1][b1][b2] = 1;
        }
    }

    for (b3 = l3 - 1; b3 >= 0; --b3) {
        for (b1 = l1; b1 >= 0; --b1) {
            for (b2 = l2; b2 >= 0; --b2) {
                /* count the ways to construct s3[b3:l3] from
                 * s1[b1:l1] and s2[b2:l2] */
                int n1 = 0, n2 = 0, i = 0;

                /* search in subspace 1 */
                for (i = b1; i < l1; ++i) {
                    if (s1[i] == s3[b3]) {
                        n1 += count[(b3 + 1) & 1][i + 1][b2];
                    }
                }

                /* search in subspace 2 */
                for (i = b2; i < l2; ++i) {
                    if (s2[i] == s3[b3]) {
                        n2 += count[(b3 + 1) & 1][b1][i + 1];
                    }
                }

                count[b3 & 1][b1][b2] = (n1 + n2) % 10007;
            }
        }
    }
}

int
main(int argc, char **argv)
{
    int N = 0;
    char s1[MAX_N_CHARS + 1], s2[MAX_N_CHARS + 1], s3[MAX_N_CHARS + 1];

    scanf("%d", &N);

    while (N--) {
        scanf("%s%s%s", s1, s2, s3);
        DP(s1, s2, s3);
        printf("%d\n", count[0][0][0]);
    }

    return 0;
}

