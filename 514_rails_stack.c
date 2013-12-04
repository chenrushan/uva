/* 
 * Name: chen rushan
 * mail: chenrslucky@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 1001

int reorder[MAX_N];
int N;
int stack[MAX_N];

int
check(void)
{
    int top = -1, i = 0, j = 1;

    for (i = 0; i < N; ++i) {
        if (top == -1 || reorder[i] > stack[top]) {
            for (; j != reorder[i] && j <= N; ++j) {
                stack[++top] = j;
            }
            if (j == N + 1) {
                return 0;
            }
            ++j;
        } else if (reorder[i] == stack[top]) {
            --top;
        } else {
            return 0;
        }
    }

    return 1;
}

int
main(int argc, char **argv)
{
    int i = 0;

    while (1) {
        scanf("%d", &N);
        if (N == 0) {
            break;
        }

        while (1) {
            scanf("%d", reorder);
            if (reorder[0] == 0) {
                break;
            }
            for (i = 1; i < N; ++i) {
                scanf("%d", reorder + i);
            }
            if (check() == 1) {
                printf("Yes\n");
            } else {
                printf("No\n");
            }
        }
        printf("\n");
    }
    return 0;
}

