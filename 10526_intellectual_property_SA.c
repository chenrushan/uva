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

    /* two rank buffer
     * index: string position
     * rank[i] means the rank of suffix i */
    int *rank;
    int *rank_;

    /* rank for S_l:2l
     * index: string position */
    int *rank2;

    /* size of the above 5 buf */
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
    size_t memsz = sizeof(struct t_sa_cntsort_buf) + 5 * sa->size * sizeof(int);

    buf = calloc(1, memsz);
    if (buf == NULL) {
        return -1;
    }
    buf->size = 5 * sa->size * sizeof(int);
    buf->count = (int *)(buf + 1);
    buf->SA2 = buf->count + sa->size;
    buf->rank = buf->SA2 + sa->size;
    buf->rank_ = buf->rank + sa->size;
    buf->rank2 = buf->rank_ + sa->size;

    *p_buf = buf;
    return 0;
}

static int
SA_init_cntsort_buf(struct t_sa_cntsort_buf *buf)
{
    memset(buf + 1, 0, buf->size);
}

void
SA_print(int *SA, const char *str, int len)
{
    int r = 0, i = 0;

    printf("--------------------------------------------------\n");

    printf("SA:\n");
    for (r = 0; r < len; ++r) {
        printf("%d ", SA[r]);
    }
    printf("\n");
    for (r = 0; r < len; ++r) {
        for (i = SA[r]; i < len; ++i) {
            if (str[i] == '\n') {
                printf("[\\n]");
            } else {
                printf("%c", str[i]);
            }
        }
        printf("\n");
    }
}

static void
swap_pointer(int **p_p1, int **p_p2)
{
    int *_p = *p_p1;
    *p_p1 = *p_p2;
    *p_p2 = _p;
}

int
SA_create_sa_cntsort(struct t_suffix_array *sa, struct t_sa_cntsort_buf *buf)
{
    int *SA = sa->SA;
    int *count = buf->count;
    int *SA2 = buf->SA2;
    int *rank = buf->rank;
    int *rank_ = buf->rank_;
    int *rank2 = buf->rank2;
    int len = sa->len;
    const char *str = sa->str;

    int l = 0, i = 0, r = 0;

    SA_init_cntsort_buf(buf);

    printf("%d: %s\n", len, str);

    /*
     * initialize SA[] and rank[] for l = 1
     */
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
    /* reset count */
    memset(count, 0, sizeof(int) * sa->size);
    for (i = 0; i < len; ++i) {
        count[rank[i]] += 1;
    }
    for (r = 1; r < sa->vcb_size; ++r) {
        count[r] += count[r - 1];
    }

    SA_print(SA, str, len);

    /* prefix doubling
     * SA_l, count_l, rank_l are ready before each iteration
     */
    for (l = 1; l < len; l <<= 1) {
        int _r = 0;

        printf("===== iteration %d =====\n", l);

        /*
         * get SA_l:2l
         */
        for (r = 0, i = len - 1; i >= len - l; --i) {
            SA2[r++] = i;
        }
        for (_r = 0; _r < len; ++_r) {
            if (SA[_r] >= l) {
                SA2[r++] = SA[_r] - l;
            }
        }
        SA_print(SA2, str, len);

        /*
         * get SA_2l
         * now SA[] is used to store SA_2l[]
        for (r = len - 1; r >= 0; --r) {
            printf("SA2: %d, rank: %d count: %d\n", SA2[r], rank[SA2[r]], count[rank[SA2[r]]]);
            SA[--count[rank[SA2[r]]]] = SA2[r];
        }
         */

        SA_print(SA, str, len);

        /*
         * get rank_l:2l
         */
        for (_r = 0; _r < l; ++_r) {
            rank2[SA2[_r]] = 0;
        }
        r = 1;
        rank2[SA2[l]] = r;
        for (; _r < len; ++_r) {
            int i_cur = SA2[_r];
            int i_pre = SA2[_r - 1];
            if (rank[i_cur + l] != rank[i_pre + l]) {
                r += 1;
            }
            rank2[i_cur] = r;
        }

        /*
         * get rank_2l
         * now rank[] is used to store rank_2l[]
         */
        r = 0;
        rank_[SA[0]] = 0;
        for (_r = 1; _r < len; ++_r) {
            int i_cur = SA[_r];
            int i_pre = SA[_r - 1];
            if (rank[i_cur] != rank[i_pre] || rank2[i_cur] != rank2[i_pre]) {
                r += 1;
            }
            rank_[i_cur] = r;
        }
        swap_pointer(&rank, &rank_);

        /*
         * get the count array for S_2l
         */
        for (i = 0; i < len; ++i) {
            count[rank[i]] += 1;
        }
        for (r = 0; r < len; ++r) {
            count[r] += count[r - 1];
        }
    }

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

