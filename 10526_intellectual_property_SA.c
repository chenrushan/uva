/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 * Last modified: 2009 Jul 04 12:48:26
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

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
    const char *str; /* SA is built for str */
    int vcb_size; /* size of vocabulary */
};

/* buffer used in counting sort, one buffer per thread */
struct t_sa_cntsort_buf {
    /* index: rank
     * count[r] means the number of occurrence of rank r */
    int *count;

    /* SA for S_l:2l
     * index: rank
     * SA2[r] means the suffix at rank r */
    int *SA2;

    /* index: string position
     * rank[i] means the rank of suffix i */
    int *rank;

    /* rank for S_l:2l
     * index: string position */
    int *rank2;

    /* size of the above 4 buf */
    int size;
};

/* 
 * The returned object is meant to reusable
 */
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
SA_init(struct t_suffix_array *sa, const char *str, int len, int vcb_size)
{
    sa->len = len;
    sa->str = str;
    sa->vcb_size = vcb_size;
    return 0;
}

int
SA_create_cntsort_buf(struct t_suffix_array *sa, struct t_sa_cntsort_buf **p_buf)
{
    struct t_sa_cntsort_buf *buf = NULL;
    size_t memsz = sizeof(struct t_sa_cntsort_buf) + 4 * sa->size * sizeof(int);

    buf = calloc(1, memsz);
    if (buf == NULL) {
        return -1;
    }
    buf->size = 4 * sa->size * sizeof(int);
    buf->count = (int *)(buf + 1);
    buf->SA2 = buf->count + sa->size;
    buf->rank = buf->SA2 + sa->size;
    buf->rank2 = buf->rank + sa->size;

    *p_buf = buf;
    return 0;
}

static int
SA_init_cntsort_buf(struct t_sa_cntsort_buf *buf)
{
    memset(buf + 1, 0, buf->size);
}

void
SA_print(struct t_suffix_array *sa)
{
    int r = 0, i = 0;

    printf("SA:\n");
    for (r = 0; r < sa->len; ++r) {
        printf("%d ", sa->SA[r]);
    }
    printf("\n");
    for (r = 0; r < sa->len; ++r) {
        for (i = sa->SA[r]; i < sa->len; ++i) {
            if (sa->str[i] == '\n') {
                printf("[\\n]");
            } else {
                printf("%c", sa->str[i]);
            }
        }
        printf("\n");
    }
}

int
SA_create_sa_cntsort(struct t_suffix_array *sa, struct t_sa_cntsort_buf *buf)
{
    int *SA = sa->SA;
    int *count = buf->count;
    int *SA2 = buf->SA2;
    int *rank = buf->rank;
    int *rank2 = buf->rank2;
    int len = sa->len;
    const char *str = sa->str;

    int l = 0, i = 0, r = 0;

    SA_init_cntsort_buf(buf);

    /*
     * initialize SA[] and rank[] for l = 1
     */
    int _len = (len < sa->vcb_size ? sa->vcb_size : len);
    for (i = 0; i < len; ++i) {
        rank[i] = str[i];
        count[rank[i]] += 1;
    }
    for (r = 1; r < sa->vcb_size; ++r) {
        count[r] += count[r - 1];
    }
    for (i = 0; i < len; ++i) {
        SA[--count[rank[i]]] = i;
    }

    /* SA_print(sa); */

    return 0;
}

int
SA_create_lcp(struct t_suffix_array *sa)
{
    
}

void
SA_free_cntsort_buf(struct t_sa_cntsort_buf **p_buf)
{
    if (p_buf == NULL) {
        return;
    }
    free(*p_buf);
    *p_buf = NULL;
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
const int vcb_size = 128;

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
    int err = 0;
    struct t_suffix_array *sa = NULL;
    struct t_sa_cntsort_buf *buf = NULL;

    err = SA_new(&sa, MAX_LEN);
    assert(err == 0);
    err = SA_create_cntsort_buf(sa, &buf);
    assert(err == 0);

    while (! read_code()) {
        /* print_code(); */
        SA_init(sa, str, len, vcb_size);
        SA_create_sa_cntsort(sa, buf);
        printf("=======\n");
    }

    SA_free_cntsort_buf(&buf);
    SA_free(&sa);

    return 0;
}

