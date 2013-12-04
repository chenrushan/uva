/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_LEN 15

char num1[MAX_LEN], num2[MAX_LEN];
int n1_len, n2_len;

int
main(int argc, char **argv)
{
        while (1) {
                int c = 0, i = 0;

                n1_len = n2_len = 0;
                memset(num1, 0, sizeof(num1));
                memset(num2, 0, sizeof(num2));

                while ((c = getchar()) != ' ')
                        num1[n1_len++] = c - '0';
                while ((c = getchar()) == ' ')
                        ;
                num2[n2_len++] = c - '0';
                while ((c = getchar()) != '\n')
                        num2[n2_len++] = c - '0';

                if (n1_len == 1 && n2_len == 1
                    && num1[0] == 0 && num2[0] == 0)
                        break;

                for (i = 0; i < n1_len / 2; i++) {
                        char t = num1[i];
                        num1[i] = num1[n1_len - i - 1];
                        num1[n1_len - i - 1] = t;
                }
                for (i = 0; i < n2_len / 2; i++) {
                        char t = num2[i];
                        num2[i] = num2[n2_len - i - 1];
                        num2[n2_len - i - 1] = t;
                }

                int carry = 0, min_len = 0, times = 0;

                min_len = n1_len < n2_len ? n1_len : n2_len;

                for (i = 0; i < min_len; i++) {
                        if (num1[i] + num2[i] + carry >= 10)
                                carry = 1, times++;
                        else
                                carry = 0;
                }
                for (; i < n1_len; i++) {
                        if (carry == 0)
                                break;
                        else
                                if (num1[i] + carry == 10)
                                        carry = 1, times++;
                                else
                                        carry = 0;
                }
                for (; i < n2_len; i++) {
                        if (carry == 0)
                                break;
                        else
                                if (num2[i] + carry == 10)
                                        carry = 1, times++;
                                else
                                        carry = 0;
                }

                if (times == 0)
                        printf("No carry operation.\n");
                else if (times == 1)
                        printf("1 carry operation.\n");
                else
                        printf("%d carry operations.\n", times);
        }

        return 0;
}

