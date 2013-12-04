/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_COMPUTERS 5000

short S[MAX_COMPUTERS];

/**
 * @i and @j must be root
 */
void
set_union(int i, int j)
{
        S[j] = i;
}

int
set_find(int i)
{
        while (S[i] != 0)
                i = S[i];
        return i;
}

int
main(int argc, char **argv)
{
        int tc = 0, N = 0, c1 = 0, c2 = 0;
        char o = 0;
        char buf[256];
        int t = 0, f = 0;
        int blank = 0, is_end = 0;

        scanf("%d\n", &tc);
        while (tc--) {
                if (scanf("%d\n", &N) == EOF)
                        break;

                memset(S, 0, sizeof(S));
                t = f = 0;

                /**
                 * Deal with all pairs
                 */
                while (1) {
                        if (fgets(buf, sizeof(buf), stdin) == NULL)
                                is_end = 1;
                        if (buf[0] == '\n' || is_end == 1)
                                break;
                        sscanf(buf, "%c %d %d", &o, &c1, &c2);
                        int r1 = set_find(c1), r2 = set_find(c2);

                        if (o == 'c') {
                                if (r1 != r2)
                                        set_union(r1, r2);
                        } else {
                                r1 == r2 ? t++ : f++;
                        }
                }
                if (blank == 1)
                        putchar('\n');
                blank = 1;
                printf("%d,%d\n", t, f);

                if (is_end == 1)
                        break;
        }

        return 0;
}

