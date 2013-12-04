/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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

