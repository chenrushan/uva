#include <stdio.h>
#include <string.h>

#define MAX_LEN 1000010

char text[MAX_LEN];
int match_len[MAX_LEN];

int
build_match_len_2(void)
{
    int i = 0, j = 0;

    match_len[0] = -1;
    for (j = -1, i = 1; text[i - 1] != '\n'; ) {
        if (j != -1 && text[i - 1] == text[j]) {
            match_len[i] = j + 1;
            i++, j++;
        } else {
            if (j == -1) {
                match_len[i] = 0;
                i++, j++;
            } else {
                j = match_len[j];
            }
        }
    }
    match_len[0] = 0;

    return i - 1;
}

int
build_match_len(void)
{
    int i = 0, j = 0;

    match_len[0] = -1;
    for (i = 1; text[i - 1] != '\n'; ++i) {
        j = match_len[i - 1];
        while (j != -1 && text[i - 1] != text[j]) {
            j = match_len[j];
        }
        match_len[i] = j + 1;
    }
    match_len[0] = 0;

    return i - 1;
}

int
main(int argc, char **argv)
{
    while (1) {
        fgets(text, MAX_LEN, stdin);
        if (text[0] == '.' && text[1] == '\n') {
            break;
        }

        int text_len = build_match_len_2();
        if (text_len % (text_len - match_len[text_len]) == 0) {
            printf("%d\n", text_len / (text_len - match_len[text_len]));
        } else {
            printf("1\n");
        }
    }

    return 0; /* ERROR 3 */
}

