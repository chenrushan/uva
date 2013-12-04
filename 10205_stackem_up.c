/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_CARD 53
#define MAX_SHUFFLE 105

char shuffles[MAX_SHUFFLE][MAX_CARD];
int nshuf;

char *values[] = {
        "2", "3", "4", "5", "6", "7", "8", "9",
        "10", "Jack", "Queen", "King", "Ace"
};
char *suits[] = {
        "Clubs", "Diamonds", "Hearts", "Spades"
};

int
main(int argc, char **argv)
{
        int tc = 0, i = 0, j = 0;
        char num[128];
        char *d1 = NULL, *d2 = NULL;
        char deck1[MAX_CARD], deck2[MAX_CARD];
        int blank = 0;

        scanf("%d\n", &tc);
        while (tc--) {
                /* initialize
                 */
                memset(shuffles, 0, sizeof(shuffles));

                scanf("%d", &nshuf);
                for (i = 1; i <= nshuf; i++) {
                        for (j = 1; j <= 52; j++)
                                scanf("%d", &shuffles[i][j]);
                }
                fgets(num, sizeof(num), stdin); /* read trailing '\n' */

                d1 = deck2, d2 = deck1;
                for (i = 1; i <= 52; i++)
                        d1[i] = d2[i] = i;
                while (1) {
                        int n = 0;

                        if (fgets(num, sizeof(num), stdin) == NULL)
                                break;
                        if (num[0] == '\n')
                                break;
                        n = atoi(num);

                        /* swap @d1 and @d2
                         */
                        char *t = d1;
                        d1 = d2;
                        d2 = t;
                        for (i = 1; i <= 52; i++)
                                d2[i] = d1[shuffles[n][i]];

                }

                if (blank)
                        putchar('\n');
                blank = 1;

                /* print the result in @d2
                 */
                for (i = 1; i <= 52; i++) {
                        int c = d2[i];
                        printf("%s of ", values[(c - 1) % 13]);
                        printf("%s\n", suits[(c - 1) / 13]);
                }
        }

        return 0;
}

