/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct order {
        int ord;
        int stridx;
};

struct order ords[100];
int map2idx[128];               /* map 'A', 'C', 'G', 'T' to 0, 1, 2, 3 */
int letter_count[4][51];        /* letter_count[i][j]: the number of i before and
                                   including position j */
char DNAs[100][51];

int
main(int argc, char **argv)
{
        int N = 0, len = 0, nstr = 0;
        int i = 0, j = 0;

        map2idx['A'] = 0;
        map2idx['C'] = 1;
        map2idx['G'] = 2;
        map2idx['T'] = 3;

        scanf("%d\n", &N);

        while (N--) {
                /* accept input
                 */
                if (scanf("%d %d\n", &len, &nstr) == EOF)
                        break;
                for (i = 0; i < nstr; i++)
                        scanf("%s\n", DNAs[i]);

                /* calculate the order of sortness
                 */
                for (i = 0; i < nstr; i++) {
                        int ord = 0, k = 0;

                        for (k = 0; k < 4; k++)
                                letter_count[k][len] = 0;
                        for (j = len - 1; j >= 0; j--) {
                                int idx = map2idx[DNAs[i][j]];
                                for (k = 0; k < 4; k++)
                                        letter_count[k][j] = letter_count[k][j + 1];
                                letter_count[idx][j] += 1;
                        }
                        for (j = 0; j < len - 1; j++) {
                                int idx = map2idx[DNAs[i][j]];
                                for (k = 0; k <= idx - 1; k++)
                                        ord += letter_count[k][j + 1];
                        }
                        ords[i].ord = ord;
                        ords[i].stridx = i;
                }

                /* insertion sort the order
                 */
                for (i = 1; i < nstr; i++) {
                        for (j = i - 1; j >= 0; j--) {
                                if (ords[j].ord <= ords[i].ord)
                                        break;
                        }
                        struct order tmp = ords[i];
                        memmove(ords + j + 2, ords + j + 1,
                                sizeof(tmp) * (i - j - 1));
                        ords[j + 1] = tmp;
                }

                /* output result
                 */
                for (i = 0; i < nstr; i++) {
                        j = ords[i].stridx;
                        printf("%s\n", DNAs[j]);
                }
                if (N != 0)
                        putchar('\n');
        }

        return 0;
}

