/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct record {
        int wrdidx;     /* index of @key_words */
        int ttlidx;     /* index of @titles */
        int stridx;     /* index of a keyword in a title */
};

char ign_words[600];
int ign_wrd_idxs[50];   /* store the index of ign_words, which is used to 
                           find a ingored word */
int ign_idxs_sz;

/* multiple occurrences of a keyword are stored multiple times
 * for convenience
 */
char key_words[10001];
struct record key_wrd_idxs[5000];
char titles[10001];

/**
 * This function search @wrd in @wrds.
 * @sz is the size of @idxs.
 * Return the index of @idxs, -1 is returned if
 * @wrd is not in @wrds.
 */
int
bsrch(int *idxs, int sz, char *wrds, char *wrd)
{
        int b = 0, e = sz - 1, m = 0;
        int res = 0;

        while (b <= e) {
                m = (b + e) / 2;
                res = strcmp(wrds + idxs[m], wrd);
                if (res == 0)
                        return m;
                else if (res < 0)
                        b = m + 1;
                else
                        e = m - 1;
        }

        return -1;
}

int
cmp_ign(const void *i1, const void *i2)
{
        int idx1 = *(int *)i1;
        int idx2 = *(int *)i2;

        return strcmp(ign_words + idx1, ign_words + idx2);
}

int
cmp_key(const void *k1, const void *k2)
{
        struct record *r1 = (struct record *)k1;
        struct record *r2 = (struct record *)k2;

        return strcmp(key_words + r1->wrdidx, key_words + r2->wrdidx);
}

int
main(int argc, char **argv)
{
        int i = 0, j = 0, k = 0, ti = 0;

        /* read words to ingore
         */
        while (1) {
                scanf("%s\n", ign_words + i);
                if (ign_words[i] == ':' && ign_words[i + 1] == ':')
                        break;
                ign_wrd_idxs[j++] = i;
                i += strlen(ign_words + i) + 1; /* 1 for '\0' */
        }
        qsort(ign_wrd_idxs, j, sizeof(int), cmp_ign);
        ign_idxs_sz = j;

        i = j = 0;

        /* For this while loop:
         * j: index of @key_words
         * k: index of @key_wrd_idxs
         */
        while (1) {
                if (fgets(titles + ti, 10001, stdin) == NULL)
                        break;
                char *base = titles + ti; /* begin of a title */

                base[strlen(base) - 1] = '\0'; /* get rid of '\n' */

                i = 0;
                while (1) {
                        while (base[i] == ' ')
                                i++;
                        if (base[i] == '\0')
                                break;

                        /* get a word
                         */
                        key_wrd_idxs[k].wrdidx = j;
                        key_wrd_idxs[k].ttlidx = ti;
                        key_wrd_idxs[k++].stridx = i + ti;
                        while (base[i] != '\0' && base[i] != ' ')
                                key_words[j++] = tolower(base[i++]);
                        key_words[j++] = '\0';

                        /* check if this keyword should be ignored
                         */
                        if (bsrch(ign_wrd_idxs, ign_idxs_sz, ign_words,
                                  key_words + key_wrd_idxs[k - 1].wrdidx) != -1)
                                k--;
                }
                ti += strlen(base) + 1; /* 1 for '\0' */
        }
        qsort(key_wrd_idxs, k, sizeof(struct record), cmp_key);

        for (i = 0; i < k; i++) {
                for (j = key_wrd_idxs[i].ttlidx; titles[j] != '\0'; ) {
                        if (j == key_wrd_idxs[i].stridx) {
                                int t = j;
                                while (titles[t] != '\0' && titles[t] != ' ')
                                        putchar(toupper(titles[t++]));
                                j = t;
                        } else {
                                putchar(tolower(titles[j++]));
                        }
                }
                putchar('\n');
        }

        return 0;
}

