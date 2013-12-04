/* 
 * Name: chen rushan
 * mail: chenrslucky@gmail.com
 * uva: 172
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_CHARS 110
#define N_ALPHAS 26

char str[MAX_CHARS];
int idx;
int vars[N_ALPHAS], vars_bak[N_ALPHAS];
int is_change[N_ALPHAS];

int
skip_space(void)
{
    while (isspace(str[idx])) {
        idx += 1;
    }
}

int
number()
{
    int val = 0;
    int if_neg = 0;
    skip_space();
    if (str[idx] == '_') {
        if_neg = 1;
        idx += 1;
    }
    val = atoi(str + idx);
    while (isdigit(str[idx])) {
        idx += 1;
    }
    return if_neg ? -val : val;
}

char
variable()
{
    skip_space();
    return str[idx++];
}

char
operator()
{
    skip_space();
    return str[idx++];
}

int
is_operator()
{
    if (str[idx] == '*' || str[idx] == '+'
        || str[idx] == '-' || str[idx] == '/'
        || str[idx] == '=') {
        return 1;
    }
    return 0;
}

int
do_num(char op, int n1, int n2)
{
    switch (op) {
    case '*':
        return n1 * n2;
    case '+':
        return n1 + n2;
    case '-':
        return n1 - n2;
    case '/':
        return n1 / n2;
    }
}

int
do_var(char op, char var, int num)
{
    int i = var - 'A';
    switch (op) {
    case '=':
        vars[i] = num;
        return num;

    default:
        return do_num(op, vars[i], num);
    }
}

int order;
int
expression(void)
{
    int value = 0;
    char op = 0, ch = 0;

    skip_space();
    ch = str[idx];
    if (ch == '\0') {
        return 0;
    }

    if (ch == '(') {
        idx += 1; /* read '(' */
        value = expression();
        skip_space();
        idx += 1; /* read ')' */

        skip_space();
        if (is_operator()) {
            op = operator();
            /*printf("%d|%c ", order++, op);*/
            int v = expression();
            return do_num(op, value, v);
        } else {
            return value;
        }

    } else if (ch == '_' || isdigit(ch)) {
        int num = number();
        /*printf("%d|%d ", order++, num);*/
        skip_space();
        if (is_operator()) {
            op = operator();
            /*printf("%d|%c ", order++, op);*/
            value = expression();
            return do_num(op, num, value);
        } else {
            return num;
        }

    } else {
        char var = variable();
        /*printf("%d|%c ", order++, var);*/
        skip_space();
        if (is_operator()) {
            op = operator();
            /*printf("%d|%c ", order++, op);*/
            value = expression();
            return do_var(op, var, value);
        } else {
            return vars[var - 'A'];
        }
    }
}

int
main(int argc, char **argv)
{
    while (1) {
        /* initialize */
        memset(str, 0, sizeof(str));
        memcpy(vars_bak, vars, sizeof(vars));
        memset(is_change, 0, sizeof(is_change));
        idx = 0;

        fgets(str, sizeof(str)/sizeof(str[0]), stdin);
        if (str[0] == '#') {
            break;
        }

        expression();

        int i = 0, is_first = 1;
        for (i = 0; i < N_ALPHAS; ++i) {
            if (vars[i] != vars_bak[i]) {
                if (! is_first) {
                    printf(", ");
                }
                printf("%c = %d", i + 'A', vars[i]);
                is_first = 0;
            }
        }
        if (is_first) {
            printf("No Change");
        }
        printf("\n");
    }

    return 0;
}

