/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char seq[81];
int num, alpcnt;
int curnum, last;

/**
 * @idx: the index of last char in @seq
 */
int
has_same_subseq(int idx)
{
        int i = 0, j = 0;

        if (idx == 0)
                return 0;

        for (i = idx; i >= idx / 2 + 1; i--) {
                int len = idx - i + 1, iseq = 0;
                j = i - len;
                iseq = memcmp(seq + j, seq + i, len);
                if (iseq == 0)
                        return 1;
        }

        return 0;
}

/**
 * Find the @n-th seq, store next char at @idx of @seq
 * Return 1 if the @n-th seq is found, 0 otherwise
 */
int
find_seq(int idx)
{
        int i = 0, isfound = 0;

        for (i = 0; i < alpcnt; i++) {
                seq[idx] = i;
                if (has_same_subseq(idx))
                        continue;
                if (++curnum == num) {
                        last = idx;
                        return 1;
                }
                isfound = find_seq(idx + 1);
                if (isfound == 1)
                        return 1;
        }
        return 0;
}

void
print()
{
        int i = 0;

        printf("%c", seq[0] + 'A');
        for (i = 1; i <= last; i++) {
                if ((i & 0x03) == 0) {
                        if ((i & 0x3F) == 0)
                                putchar('\n');
                        else
                                putchar(' ');
                }
                printf("%c", seq[i] + 'A');
        }
        printf("\n%d\n", last + 1);
}

int
main(int argc, char **argv)
{
        for (; ;) {
                scanf("%d %d\n", &num, &alpcnt);
                if (num == 0 && alpcnt == 0)
                        break;
                curnum = 0;
                find_seq(0);
                print();
        }

        return 0;
}

