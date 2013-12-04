/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define LINE_LEN 1200

char fstline[LINE_LEN];
char sndline[LINE_LEN];
int fstcnt[26];
int sndcnt[26];

int
main(int argc, char **argv)
{
        while (1) {
                int i = 0, fl = 0, sl = 0;

                if (fgets(fstline, sizeof(fstline), stdin) == NULL)
                        break;
                fgets(sndline, sizeof(sndline), stdin);

                memset(fstcnt, 0, sizeof(fstcnt));
                memset(sndcnt, 0, sizeof(sndcnt));

                fl = strlen(fstline);
                sl = strlen(sndline);
                fstline[(fl--) - 1] = '\0'; /* get rid of '\n' */
                sndline[(sl--) - 1] = '\0';

                if (fl == 0 || sl == 0) {
                        putchar('\n');
                        continue;
                }
                for (i = 0; i < fl; i++)
                        fstcnt[fstline[i] - 'a']++;
                for (i = 0; i < sl; i++)
                        sndcnt[sndline[i] - 'a']++;
                for (i = 0; i < 26; i++) {
                        int j = 0, min = 0;

                        min = fstcnt[i] < sndcnt[i] ? fstcnt[i] : sndcnt[i];
                        for (j = 0; j < min; j++)
                                printf("%c", i + 'a');
                }
                putchar('\n');
        }

        return 0;
}

