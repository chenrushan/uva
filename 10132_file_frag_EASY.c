/* 
 * Author: Chen Rushan
 * E-Mail: juscodit@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_SEG_SIZE 258
#define MAX_NSEGS 300

int len2seg[MAX_SEG_SIZE][MAX_NSEGS]; /* mapping from seglen to seg index in @segs */
int len2nsegs[MAX_NSEGS]; /* number of segs for the same length */
char segs[MAX_NSEGS][MAX_SEG_SIZE]; /* segs[i] for the ith seg */
int nsegs;
int filelen;
char lendone[MAX_SEG_SIZE]; /* lendone[i] = 1 also means lendone[filelen - i] = 1 */

char buf1[MAX_NSEGS][MAX_SEG_SIZE], buf2[MAX_NSEGS][MAX_SEG_SIZE]; /* buffers used to store intersections */
char (*old)[MAX_SEG_SIZE], (*new)[MAX_SEG_SIZE];

int
search(char *seg, char (*old)[MAX_SEG_SIZE], int nold)
{
    int i = 0;
    for (i = 0; i < nold; ++i) {
        if (! strcmp(seg, old[i])) {
            return 1;
        }
    }
    return 0;
}

void
findfile(void)
{
    int l = 0, i = 0, j = 0;
    int ninter = 0; /* number of intersections */
    char buf[MAX_SEG_SIZE];

    old = buf1;
    new = buf2;

    /* initialize new */
    for (l = 1; l <= filelen; ++l) {
        if (len2nsegs[l] != 0) {
            break;
        }
    }
    for (i = 0; i < len2nsegs[filelen - l]; ++i) {
        /* concat len2seg[l][0] with len2seg[filelen - l][i] */
        char *p = (char *)(new + ninter);
        memcpy(p, segs[len2seg[l][0]], l);
        memcpy(p + l, segs[len2seg[filelen - l][i]], filelen - l);
        p[filelen] = '\0';
        ninter += 1;

        /* concat len2seg[l][0] with len2seg[filelen - l][i] in another way */
        p = (char *)(new + ninter);
        memcpy(p, segs[len2seg[filelen - l][i]], filelen - l);
        memcpy(p + filelen - l, segs[len2seg[l][0]], l);
        p[filelen] = '\0';
        ninter += 1;
    }

    for (; l <= filelen; ++l) {
        if (lendone[filelen - l] == 1) {
            break;
        }
        /* swap old and new */
        char (*tmp)[MAX_SEG_SIZE] = old;
        old = new;
        new = tmp;

        int nold = ninter;
        ninter = 0;

        for (i = 0; i < len2nsegs[l]; ++i) {
            for (j = 0; j < len2nsegs[filelen - l]; ++j) {
                /* concat len2seg[l][i] with len2seg[filelen - l][j] */
                memcpy(buf, segs[len2seg[l][i]], l);
                memcpy(buf + l, segs[len2seg[filelen - l][j]], filelen - l);
                buf[filelen] = '\0';

                if (search(buf, old, nold) == 1) {
                    char *p = (char *)(new + ninter);
                    memcpy(p, buf, filelen + 1);
                    ninter += 1;
                }

                /* concat len2seg[l][i] with len2seg[filelen - l][j] in another way */
                memcpy(buf, segs[len2seg[filelen - l][j]], filelen - l);
                memcpy(buf + filelen - l, segs[len2seg[l][i]], l);
                buf[filelen] = '\0';

                if (search(buf, old, nold) == 1) {
                    char *p = (char *)(new + ninter);
                    memcpy(p, buf, filelen + 1);
                    ninter += 1;
                }
            }
        }
        lendone[l] = 1;
        if (ninter == 1) {
            break;
        }
    }

    printf("%s\n", new[0]);
}

int
main(int argc, char **argv)
{
    int ncases = 0;

    scanf("%d\n", &ncases);

    while (ncases--) {
        nsegs = 0;
        filelen = 0;
        memset(len2nsegs, 0, sizeof(len2nsegs));
        memset(lendone, 0, sizeof(lendone));

        while (1) {
            if (fgets(segs[nsegs], MAX_SEG_SIZE, stdin) == NULL) {
                break;
            }
            if (segs[nsegs][0] == '\n') {
                break;
            }
            int l = strlen(segs[nsegs]) - 1;
            segs[nsegs][l] = '\0'; /* remove '\n' */
            len2seg[l][len2nsegs[l]++] = nsegs;
            nsegs += 1;
            filelen += l;
        }
        filelen = (filelen << 1) / nsegs;

        /* print out segs by length

        int l = 0, j = 0;
        for (l = 1; l <= MAX_SEG_SIZE; ++l) {
            if (len2nsegs[l] == 0) {
                continue;
            }
            printf("%d: ", l);
            for (j = 0; j < len2nsegs[l]; ++j) {
                printf("%s ", segs[len2seg[l][j]]);
            }
            printf("\n");
        }
        printf("\n");
        */

        findfile();
        if (ncases != 0) {
            printf("\n");
        }
    }

    return 0;
}

