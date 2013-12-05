/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 * Last modified: 2009 Jul 04 12:48:26
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_LEN 100010
#define MAX_LINE_LEN 50010

char str[MAX_LEN];
int TDP_len, len;
int ncase, nfrags;

/*
 * return 1 if number of fragments = 0, 0 otherwise
 * TDP and JCN are both stored in @str as "<TDP>\001<JCN>"
 */
int
read_code(void)
{
    TDP_len = len = 0;

    scanf("%d", &nfrags);
    if (nfrags == 0) {
        return 1;
    }

    /* read "BEGIN TDP" */
    fgets(line + len, MAX_LEN - len, stdin);
    while (1) {
        fgets(line + len, MAX_LEN - len, stdin);

        if (line[len] == 'E' && line[len + 1] == 'N') {
            if (! strcmp(line + len, "END TDP CODEBASE")) {
                /* read "BEGIN JCN" */
                fgets(line + len, MAX_LEN - len, stdin);
                TDP_len = len;
                line[len++] = '\001';
                continue;
            } else if (! strcmp(line + len, "END JCN CODEBASE")) {
                break;
            }
        }
        len += strlen(line + len);
    }

    return 0;
}

void
print_code(void)
{
    int i = 0;

    for (i = 0; i < TDP_len; ++i) {
        putchar(str[i]);
    }
    putchar('\n');
    for (i = TDP_len + 1; i < len; ++i) {
        putchar(str[i]);
    }
}

int
main(int argc, char **argv)
{
    read_code();
    print_code();
    return 0;
}

