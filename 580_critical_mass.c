/* 
 * Author: Chen Rushan
 * E-Mail: chenrsstergmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_LEN 100000

/* c1 for sequences having only one consecutive 1
 * c2 for sequences having at least one subseq of two consecutive 1
 * c3 for sequences having at least one subseq of more than three consecutive 1
 *
 * The element of each arry stores the number of sequences
 * having some property.
 */

/* c1[][0]: for 0...1...
 * c1[][1]: for 1.......
 *
 * There are 3 ways to generate a sequence of length (l + 1) having
 * at least one 1 from a sequence of length l:
 *
 * 1. 0...1... ==> 0|0...1... and 1|0...1...
 * 2. 1....... ==> 0|1.......
 * 3. 0....... ==> 1|0.......
 *
 * c1[l][0] = c1[l - 1][0] + c1[l - 1][1]
 * c1[l][1] = c1[l - 1][0] + 1
 */
int c1[MAX_LEN][2];

/* c2[][0]: for 0...11..
 * c2[][1]: for 11......
 * c2[][2]: for 10..11..
 *
 * There are 6 ways to generate a sequence of length (l + 1)
 * having at least one 11 subseq from a sequence of length l:
 *
 * 1. 0...11.. ==> 0|0...11.. and 1|0...11..
 * 2. 11...... ==> 0|11......
 * 3. 10..11.. ==> 0|10..11.. and 1|10..11..
 * 4. 10..0... ==> 1|10...0..
 * 5. 100.1... ==> 1|100..1..
 * 6. 101..... ==> 1|101.....
 *
 * The last 3 seqs on the right are generated from seqs not
 * having 11, which must have 10 as their heads
 *
 * c2[l][0] = c2[l - 1][0] + c2[l - 1][1] + c2[l - 1][2]
 * c2[l][1] = c2[l - 1][2] + 1 + c1[l - 3][0] + c1[l - 3][1]
 * c2[l][2] = c2[l - 1][0]
 */
int c2[MAX_LEN][3];

/* c3[l] =
 *  2 * c3[l - 1]
 *  + 1
 *  + c1[l - 4][0] + c1[l - 4][1]
 *  + c2[l - 4][0] + c2[l - 4][1] + c2[l - 4][2]
 *
 * A seq of length (l + 1) having at least one 111 can be generated
 * from seq of length l in following 4 ways:
 *
 *  2 * c3[l - 1] is for
 *  (....111.... ==> 0|....111.... and 1|....111....)
 *
 *  1 is for:
 *  (110 0...... ==> 1|110 0......)
 *
 *  c1[l - 4][0] + c1[l - 4][1] is for: 
 *  (110 0..1... ==> 1|110 0..1...)
 *  (110 1...... ==> 1|110 1......)
 *
 *  c2[l - 4][0] + c2[l - 4][1] + c2[l - 4][2] is for:
 *  (110 0..11.. ==> 1|110 0..11..)
 *  (110 11..... ==> 1|110 11.....)
 *  (110 10..11. ==> 1|110 10..11.)
 */
int c3[MAX_LEN];

int last;

/**
 * Initialize c1, c2 and c3
 */
void
init(void)
{
        c1[0][0] = c1[0][1] = 0;
        c1[1][0] = 0;
        c1[1][1] = 1;
        c1[2][0] = c1[2][1] = 1;
        c1[3][0] = c1[3][1] = 2;

        c2[0][0] = c2[0][1] = c2[0][2] = 0;
        c2[1][0] = c2[1][1] = c2[1][2] = 0;
        c2[2][0] = c2[2][2] = 0;
        c2[2][1] = 1;
        c2[3][0] = c2[3][1] = 1;
        c2[3][2] = 0;

        c3[0] = c3[1] = c3[2] = 0;
        c3[3] = 1;

        last = 3;
}

int
get_number_of_combins(int L)
{
        int l = 0;

        if (L <= last)
                return c3[L];

        for (l = last + 1; l <= L; l++) {
                /* update c1
                 */
                c1[l][0] = c1[l - 1][0] + c1[l - 1][1];
                c1[l][1] = c1[l - 1][0] + 1;

                /* update c2
                 */
                c2[l][0] = c2[l - 1][0] + c2[l - 1][1] + c2[l - 1][2];
                c2[l][1] = c2[l - 1][2] + 1 + c1[l - 3][0] + c1[l - 3][1];
                c2[l][2] = c2[l - 1][0];

                /* update c3
                 */
                c3[l] = 2 * c3[l - 1]
                        + 1
                        + c1[l - 4][0] + c1[l - 4][1]
                        + c2[l - 4][0] + c2[l - 4][1] + c2[l - 4][2];
        }

        last = L;

        return c3[L];
}

int
main(int argc, char **argv)
{
        int l = 0;
        
        init();

        while (1) {
                scanf("%d", &l);
                if (l == 0)
                        break;
                int n = get_number_of_combins(l);
                printf("%d\n", n);
        }

        return 0;
}

