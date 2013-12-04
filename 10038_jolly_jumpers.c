/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define FLAGS_SZ 400

char flags[FLAGS_SZ];
int N, nums[3200];

int
main(int argc, char **argv)
{
        while (1) {
                int i = 0;

                /* initialize
                 */
                memset(nums, 0, sizeof(nums));
                memset(flags, 0, sizeof(flags));

                /* read input
                 */
                if (scanf("%d", &N) == EOF)
                        break;
                for (i = 0; i < N; i++)
                        scanf("%d", nums + i);

                /* begin process
                 */
                if (N == 1) {
                        printf("Jolly\n");
                        continue;
                }
                for (i = 1; i < N; i++) {
                        int d = nums[i] - nums[i - 1];
                        int idx = 0, off = 0;

                        d = d < 0 ? -d : d;
                        idx = d >> 3;
                        off = d & 7;
                        if (idx <= FLAGS_SZ - 1)
                                flags[idx] |= (0x01 << off);
                }

                /* check if it's jolly
                 * VERY VERY clear about what each variable denotes
                 * m: the index of the last bit, so (m + 1) is the total number
                 *
                 * And be careful of the precedence between & and ==
                 */
                int m = N - 1, k = (m + 1) >> 3, c = 0, l = 0;

                flags[0] |= 0x01; /* a trick */
                for (i = 0; i < k; i++) {
                        if ((flags[i] & 0xFF) != 0xFF) {
                                printf("Not jolly\n");
                                break;
                        }
                }
                if (i != k)
                        continue;
                c = (m + 1) & 7;
                for (i = 0; i < c; i++)
                        l |= (0x01 << i);
                if ((flags[k] & l) != l)
                        printf("Not jolly\n");
                else
                        printf("Jolly\n");
        }

        return 0;
}

