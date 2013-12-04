/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int
main(int argc, char **argv)
{
        int c[3][3], i = 0, j = 0;
        int move[6], mini = 0;
        char *clrs[] = {
                "BCG", "BGC", "CBG", "CGB", "GBC", "GCB",
        };

        while (1) {
                for (i = 0; i < 3; i++)
                        for (j = 0; j < 3; j++)
                                if (scanf("%d", &c[i][j]) == EOF)
                                        return 0;
                move[0] = c[1][0] + c[2][0] /* B */
                        + c[0][2] + c[2][2] /* C */
                        + c[0][1] + c[1][1] /* G */;
                move[1] = c[1][0] + c[2][0] /* B */
                        + c[0][1] + c[2][1] /* G */
                        + c[0][2] + c[1][2] /* C */;
                move[2] = c[1][2] + c[2][2] /* C */
                        + c[0][0] + c[2][0] /* B */
                        + c[0][1] + c[1][1] /* G */;
                move[3] = c[1][2] + c[2][2] /* C */
                        + c[0][1] + c[2][1] /* G */
                        + c[0][0] + c[1][0] /* B */;
                move[4] = c[1][1] + c[2][1] /* G */
                        + c[0][0] + c[2][0] /* B */
                        + c[0][2] + c[1][2] /* C */;
                move[5] = c[1][1] + c[2][1] /* G */
                        + c[0][2] + c[2][2] /* C */
                        + c[0][0] + c[1][0] /* B */;

                mini = 0;
                for (i = 1; i < 6; i++)
                        if (move[i] < move[mini])
                                mini = i;
                printf("%s %d\n", clrs[mini], move[mini]);
        }

        return 0;
}

