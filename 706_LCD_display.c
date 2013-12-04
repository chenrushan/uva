/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char flag[10][8] = {
        {0, 1, 1, 1, 0, 1, 1, 1},
        {0, 0, 0, 1, 0, 0, 1, 0},
        {0, 1, 0, 1, 1, 1, 0, 1},
        {0, 1, 0, 1, 1, 0, 1, 1},
        {0, 0, 1, 1, 1, 0, 1, 0},
        {0, 1, 1, 0, 1, 0, 1, 1},
        {0, 1, 1, 0, 1, 1, 1, 1},
        {0, 1, 0, 1, 0, 0, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 0, 1, 1},
};

int
main(int argc, char **argv)
{
        int s = 0, n = 0;
        char nstr[12];

        while (1) {
                int i = 0, j = 0, k = 0, l = 0;

                scanf("%d ", &s);
                if (s == 0)
                        break;

                char c = 0;
                while ((c = (char)getchar()) != '\n')
                        nstr[i++] = c - '0';
                l = i;

                /* 1st line
                 */
                for (j = 0; j < l; j++) {
                        putchar(' ');
                        for (i = 0; i < s; i++)
                                putchar(flag[nstr[j]][1] ? '-' : ' ');
                        putchar(' ');
                        if (j != l - 1)
                                putchar(' ');
                }
                putchar('\n');

                /* 2nd to (s + 1)th line
                 */
                for (j = 0; j < s; j++) {
                        for (i = 0; i < l; i++) {
                                putchar(flag[nstr[i]][2] ? '|' : ' ');
                                for (k = 0; k < s; k++)
                                        putchar(' ');
                                putchar(flag[nstr[i]][3] ? '|' : ' ');
                                if (i != l - 1)
                                        putchar(' ');
                        }
                        putchar('\n');
                }

                /* (s + 2)th line
                 */
                for (j = 0; j < l; j++) {
                        putchar(' ');
                        for (i = 0; i < s; i++)
                                putchar(flag[nstr[j]][4] ? '-' : ' ');
                        putchar(' ');
                        if (j != l - 1)
                                putchar(' ');
                }
                putchar('\n');

                /* (s + 3)th to (2s + 2)th line
                 */
                for (j = 0; j < s; j++) {
                        for (i = 0; i < l; i++) {
                                putchar(flag[nstr[i]][5] ? '|' : ' ');
                                for (k = 0; k < s; k++)
                                        putchar(' ');
                                putchar(flag[nstr[i]][6] ? '|' : ' ');
                                if (i != l - 1)
                                        putchar(' ');
                        }
                        putchar('\n');
                }

                /* (2s + 3)th line
                 */
                for (j = 0; j < l; j++) {
                        putchar(' ');
                        for (i = 0; i < s; i++)
                                putchar(flag[nstr[j]][7] ? '-' : ' ');
                        putchar(' ');
                        if (j != l - 1)
                                putchar(' ');
                }
                putchar('\n');

                putchar('\n');
        }

        return 0;
}

