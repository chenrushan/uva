/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(x, y) (x) < (y) ? (x) : (y)
#define MAX(x, y) (x) > (y) ? (x) : (y)

#define MAX_N_BOX 1002
#define MAX_CAPACITY 3001

short height[MAX_CAPACITY][MAX_N_BOX];
short weight[MAX_N_BOX];
short capacity[MAX_N_BOX];
int N, max_cap;

void
DP(void)
{
        int c = 0, i = 0;

        for (c = 0; c <= max_cap; c++)
                height[c][N] = 0;

        for (i = N - 1; i >= 1; i--) {
                for (c = 0; c <= max_cap; c++) {
                        if (c < weight[i]) {
                                height[c][i] = height[c][i + 1];
                        } else {
                                int cap = MIN(capacity[i], c - weight[i]);
                                height[c][i] = MAX(1 + height[cap][i + 1],
                                                   height[c][i + 1]);
                        }
                }
        }
}

int
main(int argc, char **argv)
{
        while (1) {
                int i = 0;

                scanf("%d", &N);
                if (N == 0)
                        break;
                max_cap = 0;
                for (i = 0; i < N; i++) {
                        scanf("%d %d", weight + i, capacity + i);
                        if (max_cap < capacity[i])
                                max_cap = capacity[i];
                }

                DP();

                int maxh = 0;
                maxh = 1 + height[capacity[0]][1];
                for (i = 1; i < N; i++)
                        maxh = MAX(maxh, 1 + height[capacity[i]][i + 1]);
                printf("%d\n", maxh);
        }


        return 0;
}

