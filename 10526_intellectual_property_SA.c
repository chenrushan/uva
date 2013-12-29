/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#define MAX_LEN 100010
#define MAX_LINE_LEN 50010
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* ====================================================================== */
/*                      suffix array implementation                       */
/*                         only for ascii string                          */
/* ====================================================================== */

struct t_suffix_array {
    int *SA; /* suffix array */
    int *LCP; /* LCP[i] = lenght of LCP of S[i] and S[i-1] */
    int size; /* size of suffix array */

    int len; /* number of slots used */
    const char *str; /* SA is built for str */
    int vcb_size; /* size of vocabulary */
};

/* buffer used in counting sort, one buffer per thread */
struct t_sa_buf {
    /* index: rank
     * count[r] is the number of occurrence of rank <= r */
    int *count;

    /* SA for S_l:2l
     * index: rank
     * SA2[r] is the suffix at rank r */
    int *SA2;

    /* two rank buffer
     * index: string position
     * rank[i] is the rank of suffix i */
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
SA_create_cntsort_buf(struct t_suffix_array *sa, struct t_sa_buf **p_buf)
{
    struct t_sa_buf *buf = NULL;
    size_t memsz = sizeof(struct t_sa_buf) + 5 * sa->size * sizeof(int);

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
SA_init_cntsort_buf(struct t_sa_buf *buf)
{
    memset(buf + 1, 0, buf->size);
}

void
SA_printr_ank(const char *id, int *rank, const char *str, int len)
{
    int j = 0, i = 0;

    printf("\n%s:\n", id);
    for (i = 0; i < len; ++i) {
        printf("%d:\t", rank[i]);
        for (j = i; j < len; ++j) {
            if (str[j] == '\n') {
                printf("*");
            } else if (str[j] == '\001') {
                printf("+");
            } else {
                printf("%c", str[j]);
            }
        }
        printf("\n");
    }
}

void
SA_print_sa(const char *id, int *SA, const char *str, int len)
{
    int r = 0, i = 0;

    printf("--------------------------------------------------\n");

    printf("%s:\n", id);
    for (r = 0; r < len; ++r) {
        printf("%d ", SA[r]);
    }
    printf("\n");
    for (r = 0; r < len; ++r) {
        printf("%d:\t", SA[r]);
        for (i = SA[r]; i < len; ++i) {
            if (str[i] == '\n') {
                printf("*");
            } else if (str[i] == '\001') {
                printf("+");
            } else {
                printf("%c", str[i]);
            }
        }
        printf("\n");
    }

    printf("--------------------------------------------------\n");
}

static void
swap_pointer(int **p_p1, int **p_p2)
{
    int *_p = *p_p1;
    *p_p1 = *p_p2;
    *p_p2 = _p;
}

int
SA_create_sa_cntsort(struct t_suffix_array *sa, struct t_sa_buf *buf)
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

    /* prefix doubling
     * SA_l, count_l, rank_l are ready before each iteration
     */
    for (l = 1; l < len; l <<= 1) {
        int r_ = 0;

        /*
         * get SA_l:2l
         */
        for (r = 0, i = len - 1; i >= len - l; --i) {
            SA2[r++] = i;
        }
        for (r_ = 0; r_ < len; ++r_) {
            if (SA[r_] >= l) {
                SA2[r++] = SA[r_] - l;
            }
        }

        /*
         * get SA_2l
         * now SA[] is used to store SA_2l[]
         */
        for (r = len - 1; r >= 0; --r) {
            SA[--count[rank[SA2[r]]]] = SA2[r];
        }

        /*
         * get rank_l:2l
         */
        for (r_ = 0; r_ < l; ++r_) {
            rank2[SA2[r_]] = 0;
        }
        r = 1;
        rank2[SA2[l]] = r;
        for (r_ = l + 1; r_ < len; ++r_) {
            int i_cur = SA2[r_];
            int i_pre = SA2[r_ - 1];
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
        for (r_ = 1; r_ < len; ++r_) {
            int i_cur = SA[r_];
            int i_pre = SA[r_ - 1];
            if (rank[i_cur] != rank[i_pre] || rank2[i_cur] != rank2[i_pre]) {
                r += 1;
            }
            rank_[i_cur] = r;
        }
        swap_pointer(&rank, &rank_);

        /*
         * get the count array for S_2l
         */
        memset(count, 0, len * sizeof(int));
        for (i = 0; i < len; ++i) {
            count[rank[i]] += 1;
        }
        for (r = 1; r < len; ++r) {
            count[r] += count[r - 1];
        }
    }

    return 0;
}

static void
SA_print_lcp(struct t_suffix_array *sa)
{
    int r = 0, i = 0;
    const char *str = sa->str;

    for (r = 0; r < sa->len; ++r) {
        printf("%d:\t", sa->LCP[r]);
        for (i = sa->SA[r]; i < sa->len; ++i) {
            if (str[i] == '\n') {
                printf("*");
            } else if (str[i] == '\001') {
                printf("+");
            } else {
                printf("%c", str[i]);
            }
        }
        printf("\n");
    }
}

int
SA_create_lcp(struct t_suffix_array *sa, struct t_sa_buf *buf)
{
    int *LCP = sa->LCP;
    int *SA = sa->SA;
    int *rank = buf->rank;
    int len = sa->len;
    const char *str = sa->str;

    int i = 0, r = 0, lcp = 0;

    for (r = 0; r < len; ++r) {
        rank[SA[r]] = r;
    }

    /* before each iteration, lcp is set to the length
     * of longest common prefix of suffix (i-1) and SA[rank[i-1]-1]
     */
    for (i = 0; i < len; ++i) {
        if (rank[i] != 0) {
            int j = SA[rank[i] - 1];

            if (lcp != 0) {
                lcp -= 1;
            }
            while (str[i + lcp] == str[j + lcp]) {
                lcp += 1;
            }
            LCP[rank[i]] = lcp;
        }
    }
    LCP[0] = 0;

    return 0;
}

void
SA_free_cntsort_buf(struct t_sa_buf **p_buf)
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

struct t_match {
    int len;
    int pos;
};

char str[MAX_LEN];
int TDP_len, len;
int ncase, nfrags;
const int vcb_size = 128;

int flag[MAX_LINE_LEN];
struct t_match matchs[MAX_LINE_LEN];
int nmatchs;

int
match_cmp(const void *_m1, const void *_m2)
{
    struct t_match *m1 = (struct t_match *)_m1;
    struct t_match *m2 = (struct t_match *)_m2;
    if (m1->len != m2->len) {
        return m2->len - m1->len;
    } else {
        return m1->pos - m2->pos;
    }
}

/*
 * return 1 if number of fragments = 0, 0 otherwise
 * TDP and JCN are both stored in @str as "<TDP>\001<JCN>"
 */
int
read_code(void)
{
    int is_TDP_end = 0;

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
            if (is_TDP_end == 0 && ! strcmp(str + len, "END TDP CODEBASE\n")) {
                /* read "BEGIN JCN" */
                fgets(str + len, MAX_LEN - len, stdin);
                TDP_len = len;
                str[len++] = '\001';
                is_TDP_end = 1;
                continue;
            } else if (! strcmp(str + len, "END JCN CODEBASE\n") &&
                       is_TDP_end == 1) {
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

void
get_matchs(struct t_suffix_array *sa)
{
    int *SA = sa->SA;
    int *LCP = sa->LCP;
    int r = 0, i = 0, j = 0;

    memset(matchs, 0, sizeof(matchs));

    /* downward match, O(len)
     * the code snippet within the inner loop executes
     * at most (len - TDP_len) times */
    for (r = 0; r < len; ++r) {
        /* check if this suffix belongs to TDP */
        if (SA[r] >= TDP_len) {
            continue;
        }

        int r_ = 0, lcp = MAX_LEN;
        for (r_ = r + 1; r_ < len && SA[r_] > TDP_len; ++r_) {
            /* get lcp between SA[r] and SA[r_] */
            lcp = MIN(lcp, LCP[r_]);
            if (lcp == 0) {
                break;
            }
            j = SA[r_] - TDP_len - 1;
            matchs[j].pos = j;
            matchs[j].len = lcp;
        }
    }

    /* upward match and update @matchs */
    for (r = 1; r < len; ++r) {
        if (SA[r] >= TDP_len) {
            continue;
        }
        int r_ = 0, lcp = MAX_LEN;
        for (r_ = r; r_ >= 1 && SA[r_ - 1] > TDP_len; --r_) {
            lcp = MIN(lcp, LCP[r_]);
            if (lcp == 0) {
                break;
            }
            j = SA[r_ - 1] - TDP_len - 1;
            if (matchs[j].len < lcp) {
                matchs[j].pos = j;
                matchs[j].len = lcp;
            }
        }
    }

    /* compress matchs array */
    nmatchs = 0;
    for (i = 0; i < len - TDP_len - 1; ++i) {
        if (matchs[i].len != 0) {
            matchs[nmatchs++] = matchs[i];
        }
    }

    qsort(matchs, nmatchs, sizeof(*matchs), match_cmp);
}

void
print_match(void)
{
    int i = 0, n = 0;

    memset(flag, 0, sizeof(flag));

    for (i = 0; i < nmatchs && n < nfrags; ++i) {
        /* check if this match is contained within another match */
        int sum = matchs[i].pos + matchs[i].len;
        if (flag[sum] == 1) {
            continue;
        }
        flag[sum] = 1;

        n += 1;

        printf("INFRINGING SEGMENT %d LENGTH %d POSITION %d\n",
               n, matchs[i].len, matchs[i].pos);
        int j = 0;
        for (j = 0; j < matchs[i].len; ++j) {
            char c = str[TDP_len + 1 + matchs[i].pos + j];
            putchar(c);
        }
        putchar('\n');
    }
}

int
main(int argc, char **argv)
{
    int err = 0, is_first = 1;
    struct t_suffix_array *sa = NULL;
    struct t_sa_buf *buf = NULL;

    err = SA_new(&sa, MAX_LEN);
    assert(err == 0);
    err = SA_create_cntsort_buf(sa, &buf);
    assert(err == 0);

    while (! read_code()) {
        ncase += 1;
        if (! is_first) {
            printf("\n");
        }
        printf("CASE %d\n", ncase);

        /* print_code(); */

        SA_init(sa, str, len, vcb_size);
        SA_create_sa_cntsort(sa, buf);
        SA_create_lcp(sa, buf);

        /* SA_print_sa("SA", sa->SA, sa->str, sa->len); */
        /* SA_print_lcp(sa); */

        get_matchs(sa);
        print_match();
        is_first = 0;
    }

    SA_free_cntsort_buf(&buf);
    SA_free(&sa);

    return 0;
}

