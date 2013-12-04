/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define N 10002
#define RN 102

int nums[N];

int rems[RN];
int rems_len;

int rems2[RN];
int rems2_len;
int rem_flg2[RN];

int
main(int argc, char **argv)
{
        int tc = 0;

        scanf("%d\n", &tc);
        while (tc--) {
                int n = 0, K = 0, i = 0, j = 0;

                scanf("%d%d", &n, &K);
                for (i = 0; i < n; i++)
                        scanf("%d", &nums[i]);

                memset(rem_flg2, 0, sizeof(rem_flg2));

                /* the first number
                 */
                rems[0] = nums[0] % K;
                if (rems[0] < 0)
                        rems[0] += K;
                rems_len = 1;
                rem_flg2[rems[0]] = 1;

                /* the rest of numbers
                 */
                for (i = 1; i < n; i++) {
                        rems2_len = 0;
                        memset(rem_flg2, 0, sizeof(rem_flg2));

                        for (j = 0; j < rems_len; j++) {
                                int r = 0;

                                r = (rems[j] + nums[i]) % K;
                                if (r < 0)
                                        r += K;
                                if (rem_flg2[r] == 0) {
                                        rems2[rems2_len++] = r;
                                        rem_flg2[r] = 1;
                                }

                                r = (rems[j] - nums[i]) % K;
                                if (r < 0)
                                        r += K;
                                if (rem_flg2[r] == 0) {
                                        rems2[rems2_len++] = r;
                                        rem_flg2[r] = 1;
                                }
                        }

                        memcpy(rems, rems2, sizeof(rems));
                        rems_len = rems2_len;
                }

                /* print the result
                 */
                if (rem_flg2[0] == 1)
                        printf("Divisible\n");
                else
                        printf("Not divisible\n");
        }

        return 0;
}

