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
        int i = 0, j = 0, oi = 0, oj = 0;

        while (scanf("%d %d\n", &i, &j) != EOF) {
                int max_cl = 0; /* maximum cycle length */
                int cl = 0;

                oi = i, oj = j;
                if (i > j) {
                        int t = i;
                        i = j;
                        j = t;
                }
                for (; i <= j; i++) {
                        int t = i;
                        cl = 1;
                        while (t != 1) {
                                if (t & 0x1 != 0)
                                        t = 3 * t + 1;
                                else
                                        t >>= 1;
                                cl++;
                        }
                        max_cl = max_cl < cl ? cl : max_cl;
                }

                printf("%d %d %d\n", oi, oj, max_cl);
        }

        return 0;
}

