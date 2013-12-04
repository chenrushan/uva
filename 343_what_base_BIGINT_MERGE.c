/* 
 * Name: chen rushan
 * mail: chenrslucky@gmail.com
 * uva 343
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NDIGIT 500

typedef unsigned char uint8_t;

struct t_bigint {
    uint8_t val[MAX_NDIGIT];
    int len;
};

void
print_bigint(struct t_bigint *bi)
{
    int i = 0;
    if (bi->len == 0) {
        printf("0\n");
        return;
    }
    for (i = bi->len - 1; i > -1; --i) {
        printf("%c", bi->val[i] + '0');
    }
    printf("\n");
}

int
cmp_bigint(struct t_bigint *n1, struct t_bigint *n2)
{
    if (n1->len > n2->len) {
        return 1;
    } else if (n1->len < n2->len) {
        return -1;
    } else {
        int i = 0;
        for (i = n1->len; i > -1; --i) {
            if (n1->val[i] > n2->val[i]) {
                return 1;
            } else if (n1->val[i] < n2->val[i]) {
                return -1;
            }
        }
    }
    return 0;
}

void
int_to_bigint(int i, struct t_bigint *bi)
{
    int idx = 0;

    memset(bi, 0, sizeof(*bi));
    while (i != 0) {
        uint8_t d = i % 10;
        i /= 10;
        bi->val[idx++] = d;
    }
    bi->len = idx;
}

void
multiply_digit(struct t_bigint *num, uint8_t digit, int nzeros,
               struct t_bigint *res)
{
    int ires = 0, i = 0;
    uint8_t carry = 0;

    for (i = 0; i < nzeros; ++i) {
        res->val[i] = 0;
    }
    ires = i;

    for (i = 0; i < num->len; ++i) {
        uint8_t tmp = digit * num->val[i] + carry;
        carry = tmp / 10;
        res->val[ires++] = tmp % 10;
    }
    if (carry != 0) {
        res->val[ires++] = carry;
    }
    res->len = ires;
}

/* add @num2 to @num1 */
void
add(struct t_bigint *num1, struct t_bigint *num2)
{
    int i = 0;
    uint8_t carry = 0;

    for (i = 0; i < num2->len; ++i) {
        uint8_t tmp = num1->val[i] + num2->val[i] + carry;
        carry = tmp / 10;
        num1->val[i] = tmp % 10;
    }
    for (; i < num1->len; ++i) {
        uint8_t tmp = num1->val[i] + carry;
        carry = tmp / 10;
        num1->val[i] = tmp % 10;
    }
    if (carry != 0) {
        num1->val[i++] = carry;
    }
    num1->len = i;
}

void
multiply(struct t_bigint *num1, struct t_bigint *num2, struct t_bigint *res)
{
    struct t_bigint buf;
    int i = 0;

    memset(res, 0, sizeof(*res));
    memset(&buf, 0, sizeof(buf));
    for (i = 0; i < num2->len; ++i) {
        multiply_digit(num1, num2->val[i], i, &buf);
        add(res, &buf);
    }
}

/* ====================================================================== */

#define MAX_POW 300
#define MAX_NBASE 37
#define MAX_DIGIT_VALUE 35

/* map from a digit char (['0':'9'] and ['A':'Z']) to int */
int dig2i[256];
/* map from a digit char to bigint */
struct t_bigint dig2bi[256];

/* cache of base^n, where base ranges from 2 to 36, n from 0 to MAX_POW */
struct t_bigint pow_cache[MAX_NBASE][MAX_POW + 1];
/* the last pow that is cached for a given base */
int last_pow[MAX_NBASE];

void
init_dig2bi(void)
{
    char d = 0, n = 0;
    for (d = '0'; d <= '9'; ++d, ++n) {
        int_to_bigint(n, dig2bi + d);
        dig2i[d] = n;
    }
    for (d = 'A'; d <= 'Z'; ++d, ++n) {
        int_to_bigint(n, dig2bi + d);
        dig2i[d] = n;
    }
}

void
init_pow_cache(void)
{
    int b = 0;
    for (b = 2; b < MAX_NBASE; ++b) {
        last_pow[b] = 0;
        int_to_bigint(1, &(pow_cache[b][0]));
    }
}

/*
 * extend pow_cache for @base to @pow
 */
void
extend_pow_cache(int base, int pow)
{
    int p = 0;
    struct t_bigint base_bi;

    if (pow > last_pow[base]) {
        int_to_bigint(base, &base_bi);
        for (p = last_pow[base] + 1; p <= pow; ++p) {
            multiply(&base_bi, &(pow_cache[base][p - 1]), &(pow_cache[base][p]));
        }
        last_pow[base] = pow;
    }
}

void
to_base_10(char *digs, int ndigs, int base, struct t_bigint *bi)
{
    struct t_bigint buf;
    struct t_bigint digxbasen; /* dig * base^n */
    int i = 0;

    memset(&buf, 0, sizeof(buf));
    for (i = ndigs - 1; i >= 0; --i) {
        int pow = ndigs - i - 1;
        extend_pow_cache(base, pow);
        multiply(dig2bi + digs[i], &(pow_cache[base][pow]), &digxbasen);
        add(&buf, &digxbasen);
    }
    memcpy(bi, &buf, sizeof(buf));
}

int
get_min_base(char *digs, int ndigs)
{
    int i = 0;
    int base = 2;

    for (i = 0; i < ndigs; ++i) {
        if (base <= dig2i[digs[i]]) {
            base = dig2i[digs[i]] + 1;
        }
    }
    return base;
}

int
find_base(char *digs1, int ndigs1, char *digs2, int ndigs2)
{
    int b1 = get_min_base(digs1, ndigs1);
    int b2 = get_min_base(digs2, ndigs2);
    struct t_bigint bi1, bi2;

    to_base_10(digs1, ndigs1, b1, &bi1);
    to_base_10(digs2, ndigs2, b2, &bi2);

    while (b1 < MAX_NBASE && b2 < MAX_NBASE) {
        int c = cmp_bigint(&bi1, &bi2);
        if (c == 0) {
            break;
        } else if (c > 0) {
            b2 += 1;
            if (b2 >= MAX_NBASE) {
                break;
            }
            to_base_10(digs2, ndigs2, b2, &bi2);
        } else {
            b1 += 1;
            if (b1 >= MAX_NBASE) {
                break;
            }
            to_base_10(digs1, ndigs1, b1, &bi1);
        }
    }

    if (b1 != MAX_NBASE && b2 != MAX_NBASE) {
        printf("%s (base %d) = %s (base %d)\n", digs1, b1, digs2, b2);
    } else {
        printf("%s is not equal to %s in any base 2..36\n", digs1, digs2);
    }
}

int
main(int argc, char **argv)
{
    init_dig2bi();
    init_pow_cache();

    char digs1[MAX_NDIGIT], digs2[MAX_NDIGIT];
    int ndigs1 = 0, ndigs2 = 0;
    while (1) {
        if (scanf("%s%s", digs1, digs2) == -1) {
            break;
        }
        find_base(digs1, strlen(digs1), digs2, strlen(digs2));
    }

    return 0;
}

