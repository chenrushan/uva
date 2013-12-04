/* 
 * uva 10220
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NDIGIT 3000
#define MAX_INTEGER 1001

typedef unsigned char uint8_t;

struct t_bigint {
    uint8_t val[MAX_NDIGIT];
    int len;
};

int digit_sum[MAX_INTEGER];

void
int_to_bigint(int i, struct t_bigint *bi)
{
    int idx = 0;
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

    memset(&buf, 0, sizeof(buf));
    for (i = 0; i < num2->len; ++i) {
        multiply_digit(num1, num2->val[i], i, &buf);
        add(res, &buf);
    }
}

int
get_digit_sum(struct t_bigint *bi)
{
    int i = 0, sum = 0;
    for (i = 0; i < bi->len; ++i) {
        sum += bi->val[i];
    }
    return sum;
}

int
main(int argc, char **argv)
{
    struct t_bigint num, res;

    memset(&num, 0, sizeof(num));
    memset(&res, 0, sizeof(res));

    int i = 0;
    digit_sum[1] = 1;
    int_to_bigint(1, &res);
    for (i = 2; i < MAX_INTEGER; ++i) {
        struct t_bigint buf = res;
        memset(&res, 0, sizeof(res));
        int_to_bigint(i, &num);
        multiply(&buf, &num, &res);
        digit_sum[i] = get_digit_sum(&res);
    }

    while (scanf("%d", &i) != EOF) {
        printf("%d\n", digit_sum[i]);
    }

    return 0;
}

