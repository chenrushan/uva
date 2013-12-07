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

/* ====================================================================== */
/*                      suffix array implementation                       */
/*                         only for ascii string                          */
/* ====================================================================== */

struct t_suffix_array {
    int *SA; /* suffix array */
    int *LCP; /* longest common prefix array */
    int size; /* size of suffix array */
    int len; /* number of slots used */
};

/* buffer used in counting sort, one buffer per thread */
struct t_sa_cntsort_buf {
    int *count;
    int *SA2; /* SA for S_l:2l */
    int *rank;
    int *rank2; /* rank for S_l:2l */
    int size;
};

int
SA_new(struct t_suffix_array **p_sa, int size)
{
    struct t_suffix_array *sa = NULL;
    size_t memsz = sizeof(*sa) + sizeof(int) * size * 2;

    sa = calloc(1, memsz);
    if (sa == NULL) {
        return -1;
    }
    sa->SA = (int *)(sa + 1);
    sa->LCP = (int *)(sa->SA + size);
    sa->size = size;

    *p_sa = sa;
    return 0;
}

int
SA_create_cntsort_buf(struct t_suffix_array *sa, struct t_sa_cntsort_buf **p_buf)
{
    struct t_sa_cntsort_buf *buf = NULL;
    size_t memsz = 4 * sa->size;

    buf = calloc(1, memsz);
    if (buf == NULL) {
        return -1;
    }
    buf->count = (int *)buf;
    buf->SA2 = buf->count + sa->size;
    buf->rank = buf->SA2 + sa->size;
    buf->rank2 = buf->rank + sa->size;
    buf->size = sa->size;

    *p_buf = buf;
    return 0;
}

int
SA_init_cntsort_buf(struct t_sa_cntsort_buf *buf)
{
    memset(buf, 0, 4 * buf->size);
}

int
SA_init(struct t_suffix_array *sa)
{
    sa->len = 0;
    return 0;
}

int
SA_create_sa_cntsort(struct t_suffix_array *sa, const char *str, int len,
                     struct t_sa_cntsort_buf *buf)
{
    int l = 0, i = 0, r = 0;

    sa->len = len;

    SA_init_cntsort_buf(buf);

    /* initialize SA[] and rank[] for l = 1 */
    for (i = 0; i < len; ++i) {
        buf->rank[i] = str[i];
        buf->count[str[i]] += 1;
    }
    for (i = 1; i < len; ++i) {
        buf->count[i] += buf->count[i - 1];
    }
}

int
SA_create_lcp(struct t_suffix_array *sa)
{
    
}

void
SA_free(struct t_suffix_array **p_sa)
{
    if (p_sa == NULL) {
        return;
    }
    free(*p_sa);
    *p_sa = NULL;
}

/* ====================================================================== */

char str[MAX_LEN];
int TDP_len, len;
int ncase, nfrags;
int SA[MAX_LEN], LCP[MAX_LEN];

/*
 * return 1 if number of fragments = 0, 0 otherwise
 * TDP and JCN are both stored in @str as "<TDP>\001<JCN>"
 */
int
read_code(void)
{
    TDP_len = len = 0;

    scanf("%d\n", &nfrags);
    if (nfrags == 0) {
        return 1;
    }

    /* read "BEGIN TDP" */
    fgets(str + len, MAX_LEN - len, stdin);
    while (1) {
        fgets(str + len, MAX_LEN - len, stdin);

        if (str[len] == 'E' && str[len + 1] == 'N') {
            if (! strcmp(str + len, "END TDP CODEBASE\n")) {
                /* read "BEGIN JCN" */
                fgets(str + len, MAX_LEN - len, stdin);
                TDP_len = len;
                str[len++] = '\001';
                continue;
            } else if (! strcmp(str + len, "END JCN CODEBASE\n")) {
                break;
            }
        }
        len += strlen(str + len);
    }
    str[len] = '\0';

    return 0;
}

void
print_code(void)
{
    int i = 0;

    putchar('{');
    for (i = 0; i < TDP_len; ++i) {
        putchar(str[i]);
    }
    putchar('\n');
    for (i = TDP_len + 1; i < len; ++i) {
        putchar(str[i]);
    }
    putchar('}');
    putchar('\n');
}

int
main(int argc, char **argv)
{
    while (! read_code()) {
        print_code();
    }

    return 0;
}

