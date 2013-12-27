/* 
 * Author: Chen Rushan
 * E-Mail: juscodit@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define ARRSZ 256

int bean_cnts[ARRSZ];
int share_cnts[ARRSZ];
int total_guess;
int total_share;
char line[2048];

int
main(int argc, char **argv)
{
    int ncases = 0;
    int i = 0, n = 0;
    char c;

    scanf("%d\n\n", &ncases);

    for (n = 0; n < ncases; ++n) {
        if (n != 0) {
            printf("\n");
        }

        memset(bean_cnts, 0, sizeof(bean_cnts));
        memset(share_cnts, 0, sizeof(share_cnts));
        total_guess = total_share = 0;

        fgets(line, sizeof(line) / sizeof(line[0]), stdin);

        for (i = 0; line[i] != '\n'; ++i) {
            bean_cnts[line[i]] += 1;
        }

        while (fgets(line, sizeof(line) / sizeof(line[0]), stdin) != NULL) {
            if (line[0] == '\n') {
                break;
            }
            char c = line[0];

            for (i = 2; line[i] != '\n'; ) {
                char b = line[i];
                i += 2;
                int g = atoi(line + i);

                /* update total_guess */
                total_guess += 1;

                /* count share */
                int diff = g - bean_cnts[b];
                if (diff < 0) {
                    diff = -diff;
                }
                int s = 0;
                if (diff == 1) {
                    s = 1;
                } else if (diff == 0) {
                    s = 2;
                }
                share_cnts[c] += s;
                total_share += s;

                while (line[i] >= '0' && line[i] <= '9') {
                    i += 1;
                }
                if (line[i] == '\n') {
                    break;
                }
                i += 1;
            }
        }

        float common = total_guess * 2.0 / total_share;
        for (c = 'A'; c <= 'Z'; ++c) {
            if (share_cnts[c] != 0) {
                float w = common * share_cnts[c];
                printf("%c %.2f\n", c, w);
            }
        }
    }

    return 0;
}

