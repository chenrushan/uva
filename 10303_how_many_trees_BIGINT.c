/* 
 * Name: chen rushan
 * mail: chenrslucky@gmail.com
 * uva: 10303
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NDIGIT 1000
#define MAX_INTEGER 1001

typedef unsigned char uint8_t;

struct t_bigint {
    uint8_t val[MAX_NDIGIT];
    int len;
};

struct t_bigint result[MAX_INTEGER];

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

void
multiply_by_10(struct t_bigint *num)
{
    int i = 0;

    if (num->len == 0) {
        return;
    }
    for (i = num->len - 1; i > -1; --i) {
        num->val[i + 1] = num->val[i];
    }
    num->val[0] = 0;
    num->len += 1;
}

/* 
 * @n1 should be bigger than @n2
 */
void
subtract(struct t_bigint *n1, struct t_bigint *n2)
{
    uint8_t borrow = 0;
    int i = 0, j = 0;

    for (i = 0; i < n1->len; ++i) {
        uint8_t d1 = n1->val[i], d2 = n2->val[i];

        if (d1 == 0 && borrow == 1) {
            d1 = 9;
            borrow = 1;
            n1->val[i] = d1 - d2;
        } else {
            d1 -= borrow;
            if (d1 < d2) {
                n1->val[i] = d1 + 10 - d2;
                borrow = 1;
            } else {
                n1->val[i] = d1 - d2;
                borrow = 0;
            }
        }
    }
    while (n1->len > 0 && n1->val[n1->len - 1] == 0) {
        n1->len -= 1;
    }
}

void
divide(struct t_bigint *dividend, struct t_bigint *divisor,
       struct t_bigint *res, struct t_bigint *rem)
{
    int i = 0;

    memset(res, 0, sizeof(*res));
    memset(rem, 0, sizeof(*rem));

    for (i = dividend->len - 1; i > -1; --i) {
        uint8_t digit = dividend->val[i];

        multiply_by_10(rem);
        /* add digit to rem */
        if (digit != 0) { /* XXX: CAUTION */
            if (rem->len == 0) {
                rem->len = 1;
            }
            rem->val[0] = digit;
        }

        int cnt = 0;
        while (cmp_bigint(rem, divisor) >= 0) {
            /* subtract divisor from rem */
            subtract(rem, divisor);
            cnt += 1;
        }
        res->val[i] = cnt;
    }

    res->len = dividend->len;
    while (res->len > 0 && res->val[res->len - 1] == 0) {
        res->len -= 1;
    }
}

int
check_bigint(struct t_bigint *bi)
{
    int i = 0;
    for (i = 0; i < bi->len; ++i) {
        if (bi->val[i] > 9) {
            return -1;
        }
    }
    return 0;
}

void
get_tree_count(void)
{
    int n = 0, i = 0;
    struct t_bigint buf1, buf2, res, rem;

    int_to_bigint(1, result);
    int_to_bigint(1, result + 1);

    for (n = 2; n <= 1000; ++n) {
        int_to_bigint(4 * n - 2, &buf1);
        int_to_bigint(n + 1, &buf2);
        multiply(&buf1, result + n - 1, &res);
        divide(&res, &buf2, result + n, &rem);
    }
}

int
main(int argc, char **argv)
{
    int i = 0;

    get_tree_count();

    while (1) {
        if (scanf("%d", &i) == -1) {
            break;
        }
        print_bigint(result + i);
    }

    return 0;
}

