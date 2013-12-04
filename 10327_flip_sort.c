#include <stdio.h>

int arr[1001];
int N;

int
min_filp(int arr[], int N)
{
    int i = 0, j = 0;
    int nflips = 0;

    for (i = 1; i < N; ++i) {
        int n = 0;
        for (j = i - 1; j >= 0; --j) {
            if (arr[i] < arr[j]) {
                ++n;
            }
        }
        nflips += n;
    }

    return nflips;
}

int
main(int argc, char **argv)
{
    while (1) {
        if (scanf("%d", &N) == EOF) {
            break;
        }
        int i = 0;
        for (i = 0; i < N; i++) {
            scanf("%d", arr + i);
        }
        printf("Minimum exchange operations : %d\n", min_filp(arr, N));
    }
    return 0;
}

