/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int board[10][10];
int cols[10];
int row, col;
int num;

void
print(void)
{
        int i = 0;

        printf("%2d      ", ++num);
        for (i = 1; i <= 7; i++)
                printf("%d ", cols[i]);
        printf("%d\n", cols[8]);
}

void
eight_queen(int c)
{
        int r = 0;

        if (c == col) {
                if (c < 8)
                        eight_queen(c + 1);
                if (c == 8)
                        print();
        } else {
                /* (r, c) identify a position
                 */
                for (r = 1; r <= 8; r++) {
                        int i = 0, j = 0, is_ok = 1;

                        for (i = 1; i <= 8; i++) /* i is row number */
                                if (i != r && board[i][c] == 1)
                                        is_ok = 0;
                        if (is_ok == 0)
                                continue;
                        for (i = 1; i <= 8; i++) /* i is col number */
                                if (i != c && board[r][i] == 1)
                                        is_ok = 0;
                        if (is_ok == 0)
                                continue;
                        for (i = r - 1, j = c - 1; i >= 1 && j >= 1; i--, j--)
                                if (board[i][j] == 1)
                                        is_ok = 0;
                        if (is_ok == 0)
                                continue;
                        for (i = r + 1, j = c + 1; i <= 8 && j <= 8; i++, j++)
                                if (board[i][j] == 1)
                                        is_ok = 0;
                        if (is_ok == 0)
                                continue;
                        for (i = r - 1, j = c + 1; i >= 1 && j <= 8; i--, j++)
                                if (board[i][j] == 1)
                                        is_ok = 0;
                        if (is_ok == 0)
                                continue;
                        for (i = r + 1, j = c - 1; i <=8 && j >= 1; i++, j--)
                                if (board[i][j] == 1)
                                        is_ok = 0;
                        if (is_ok == 0)
                                continue;

                        cols[c] = r;
                        board[r][c] = 1;
                        if (c < 8)
                                eight_queen(c + 1);
                        if (c == 8)
                                print();
                        board[r][c] = 0;
                }
        }
}

int
main(int argc, char **argv)
{
        int ncases = 0;

        scanf("%d\n", &ncases);

        while (ncases--) {
                int i = 0;

                memset(board, 0, sizeof(board));
                num = 0;

                scanf("%d %d\n", &row, &col);
                board[row][col] = 1;
                cols[col] = row;

                printf("SOLN       COLUMN\n");
                printf(" #      1 2 3 4 5 6 7 8\n\n");
                eight_queen(1);
                if (ncases != 0)
                        putchar('\n');
        }

        return 0;
}

