/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define DEF_STK_SIZE 1000
#define DEF_BUF_SIZE 2000
#define FACTOR 1.7

struct stack {
        int *stk;
        int cap;
        int top;
};

struct stack *
stack_new(int cap)
{
        struct stack *stk = NULL;

        stk = calloc(1, sizeof(*stk));
        if (stk == NULL) {
                /* never appear for on-line judge */
        }
        stk->stk = calloc(cap, sizeof(*stk->stk));
        if (stk->stk == NULL) {
                /* ... */
        }
        stk->cap = cap;
        stk->top = 0;

        return stk;
}

void
stack_push(struct stack *stk, int val)
{
        if (stk->top == stk->cap) {
                stk->cap *= FACTOR;
                stk->stk = realloc(stk->stk, stk->cap * sizeof(*stk->stk));
                if (stk->stk == NULL) {
                        /* ... */
                }
        }
        stk->stk[stk->top++] = val;
}

int
stack_pop(struct stack *stk)
{
        if (stk->top == 0)
                return;
        return stk->stk[stk->top--];
}

/**
 * Make sure you have something in stack
 */
int
stack_top(struct stack *stk)
{
        return stk->stk[stk->top - 1];
}

void
stack_empty(struct stack *stk)
{
        stk->top = 0;
}

int
stack_is_empty(struct stack *stk)
{
        return stk->top == 0 ? 1 : 0;
}

int
find_N(char *expr, int N, struct stack *stk)
{
        int i = 0, sz = strlen(expr), val = 0;
        int nempty = 0; /* the number of empty trees in a non-empty tree */

        stack_empty(stk);

        while (1) {
                int done = 0;

                switch(expr[i]) {
                case '(':
                        i++;
                        break;

                case ')':
                        if (expr[i - 1] == '(') { /* empty tree */
                                if (++nempty == 2 && stack_top(stk) == N)
                                        return 1;
                        } else {
                                stack_pop(stk);
                                nempty = 0; /* clear count before leaving a
                                               non-empty tree */
                        }
                        i++;
                        break;

                case '\0':
                        done = 1;
                        break;

                default:
                        val = atoi(expr + i);
                        while (expr[i] != '(')
                                i++;
                        if (stack_is_empty(stk))
                                stack_push(stk, val);
                        else
                                stack_push(stk, stack_top(stk) + val);
                        nempty = 0; /* clear count before entering a new
                                       non-empty tree */
                        break;
                }
                if (done == 1)
                        break;
        }

        return 0;
}

int
main(int argc, char **argv)
{
        struct stack *stk = NULL;
        int N = 0, nlp = 0, nrp = 0, i = 0;
        char c = 0, *buf = NULL;
        int bufsz = DEF_BUF_SIZE, bufidx = 0;

        stk = stack_new(DEF_STK_SIZE);
        buf = calloc(bufsz, sizeof(*buf));
        if (buf != NULL) {
                /* ... */
        }

        while (1) {
                if (scanf("%d", &N) == EOF)
                        break;

                /* initialize
                 */
                nlp = nrp = 0;
                bufidx = 0;

                /* read S-expression into buf
                 */
                while (isspace(c = getchar()))
                        ;
                nlp++;
                buf[bufidx++] = '(';
                while (nlp != nrp) {
                        c = getchar();
                        if (c == '(')
                                nlp++;
                        else if (c == ')')
                                nrp++;
                        if (! isspace(c)) {
                                if (bufidx + 1 == bufsz) { /* 1 for '\0' */
                                        bufsz *= FACTOR;
                                        buf = realloc(buf, bufsz);
                                        if (buf == NULL) {
                                                /* ... */
                                        }
                                }
                                buf[bufidx++] = c;
                        }
                }
                buf[bufidx] = '\0';

                if (find_N(buf, N, stk) == 1)
                        printf("yes\n");
                else
                        printf("no\n");
        }

        return 0;
}

