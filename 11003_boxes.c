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
int N;

int
height_DP(int cap, int i)
{
        if (i > N - 1) {
                height[cap][i] = 0;
                return 0;
        }
        if (cap < weight[i]) {
                if (height[cap][i + 1] == -1)
                        height[cap][i + 1] = height_DP(cap, i + 1);
                return height[cap][i + 1];
        } else {
                if (height[cap][i + 1] == -1)
                        height[cap][i + 1] = height_DP(cap, i + 1);
                int c = MIN(cap - weight[i], capacity[i]);
                if (height[c][i + 1] == -1)
                        height[c][i + 1] = height_DP(c, i + 1);
                
                return MAX(height[cap][i + 1], 1 + height[c][i + 1]);
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
                for (i = 0; i < N; i++)
                        scanf("%d %d", weight + i, capacity + i);
                memset(height, 0xFF, sizeof(height));

                int maxh = 0;

                maxh = 1 + height_DP(capacity[0], 1);
                for (i = 1; i < N; i++) {
                        int h = 0;
                        h = 1 + height_DP(capacity[i], i + 1);
                        maxh = MAX(maxh, h);
                }
                printf("%d\n", maxh);
        }

        return 0;
}

