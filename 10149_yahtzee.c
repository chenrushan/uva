/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define NCAT 13
#define NDICE 5
#define NDICE_VAL 6
#define NCOMB (1 << 13)
#define BONUS_THR 63
#define BONUS_RND 5
#define BONUS 35

int dice[NCAT][NDICE];
int cat_val[NCAT][NCAT]; /* cat_val[i][j] means the value of the ith
                            dice under category j */

int *stages[NCAT + 1]; /* pointer to state */
int states[NCOMB]; /* store the index of max_score */
int max_score[NCOMB]; /* max score for each combination */
int rnd_added[NCOMB]; /* used to backtrack */

/**
 * @*pj is the index of @states
 * @cur_st is the current stage, it should not exceed @stage
 * @idx is the index of max_score
 * @cur_st, @idx and @ibeg are current state during recursion
 *
 * NOTE that @ibeg is necessary, or same pattens will be regenerated
 * over and over.
 */
void
gen_comb_idx(int *states, int *pj, int stage, int cur_st, int idx, int ibeg)
{
        int i = 0;

        if (cur_st > stage) {
                states[(*pj)++] = idx;
                return;
        }
        for (i = ibeg; i < NCAT; i++) {
                if ((idx & (1 << i)) == 0) {
                        idx |= 1 << i;
                        gen_comb_idx(states, pj, stage, cur_st + 1,
                                                idx, i + 1);
                        idx &= ~(1 << i);
                }
        }
}

int
sum_dice(int *freq)
{
        int i = 0, sum = 0;
        for (i = 0; i < NDICE_VAL; i++)
                sum += freq[i] * (i + 1);
        return sum;
}

/**
 * @i: number of dice
 * @j: number of category
 */
void
get_cat_val(int cat_val[NCAT][NCAT], int i, int j, int *freq)
{
        int k = 0, m = 0, seq_len = 0;
        int has_3 = 0, has_2 = 0; /* for full house */

        /* from ones to sixes
         */
        if (j <= 5) {
                cat_val[i][j] = freq[j] * (j + 1);
                return;
        }

        cat_val[i][j] = 0; /* default */
        switch (j) {
        case 6: /* chance */
                cat_val[i][j] = sum_dice(freq);
                break;

        case 7: /* three of a kind */
                for (k = 0; k < NDICE_VAL; k++)
                        if (freq[k] >= 3)
                                break;
                if (k != NDICE_VAL)
                        cat_val[i][j] = sum_dice(freq);
                break;

        case 8: /* four of a kind */
                for (k = 0; k < NDICE_VAL; k++)
                        if (freq[k] >= 4)
                                break;
                if (k != NDICE_VAL)
                        cat_val[i][j] = sum_dice(freq);
                break;

        case 9: /* five of a kind */
                for (k = 0; k < NDICE_VAL; k++)
                        if (freq[k] == 5)
                                break;
                if (k != NDICE_VAL)
                        cat_val[i][j] = 50;
                break;

        case 10: /* short straight */
                while (1) {
                        while (freq[k] == 0)
                                k++;
                        if (k > 2)
                                break;
                        for (m = k; m < k + 4; m++)
                                if (freq[m] == 0)
                                        break;
                        if (m == k + 4) {
                                cat_val[i][j] = 25;
                                break;
                        }
                        k = m + 1;
                }
                break;

        case 11: /* long straight */
                while (freq[k] == 0)
                        k++;
                if (k > 1)
                        break;
                for (m = k; m < k + 5; m++)
                        if (freq[m] != 1)
                                break;
                if (m == k + 5)
                        cat_val[i][j] = 35;
                break;

        case 12: /* full house */
                for (k = 0; k < NDICE_VAL; k++)
                        if (freq[k] == 3)
                                has_3 = 1;
                        else if (freq[k] == 2)
                                has_2 = 1;
                if (has_3 && has_2)
                        cat_val[i][j] = 40;
                break;
        }
}

void
DP(void)
{
        int i = 0, j = 0;
        int *pst = NULL;
        int res[NCAT], is_bon = 0;

        /* initialize max_score
         */
        memset(max_score, 0, sizeof(max_score));

        /* the first stage
         */
        for (i = 0, pst = stages[0]; pst < stages[1]; pst++, i++) {
                max_score[*pst] = cat_val[i][0];
                rnd_added[*pst] = i; /* from origin */
        }

        /* for all stages
         */
        for (i = 0; i < NCAT - 1; i++) {
                /* for all states in stage i
                 */
                for (pst = stages[i]; pst < stages[i + 1]; pst++) {
                        /* for all possible next state of pst
                         */
                        for (j = 0; j < NCAT; j++) {
                                if (((*pst) & (1 << j)) != 0)
                                        continue;

                                int nx_st = (*pst) | (1 << j);
                                int scr = max_score[*pst] + cat_val[j][i+1];
                                if (i == BONUS_RND - 1 && scr >= BONUS_THR)
                                        scr += BONUS;
                                if (scr > max_score[nx_st]) {
                                        max_score[nx_st] = scr;
                                        rnd_added[nx_st] = j;
                                }
                        }
                }
        }

        /* backtrack
         */
        int st = *(stages[NCAT - 1]);
        int r = rnd_added[st];
        res[NCAT - 1] = cat_val[r][NCAT - 1];

        for (i = NCAT - 2; i >= 0; i--) {
                st = st & ~(1 << r);
                r = rnd_added[st];
                res[i] = cat_val[r][i];
        }

        /* print the result
         */
        int tmp = 0;
        for (i = 0; i < NCAT; i++) {
                tmp += res[i];
                printf("%d ", res[i]);
        }
        int max = max_score[*(stages[NCAT - 1])];
        if (tmp < max)
                printf("%d ", BONUS);
        else
                printf("0 ");
        printf("%d\n", max);
}

int
main(int argc, char **argv)
{
        int i = 0, j = 0;
        int freq[NDICE_VAL]; /* freq[i] is the freq of value (i+1) */

        /* set up data structure
         */
        for (i = 0; i < NCAT; i++) {
                stages[i] = states + j;
                gen_comb_idx(states, &j, i, 0, 0, 0);
        }
        stages[i] = states + j;

        while (1) {
                /* accept input
                 */
                for (i = 0; i < NCAT; i++) {
                        for (j = 0; j < NDICE; j++) {
                                if (scanf("%d", &dice[i][j]) == EOF)
                                        break;
                        }
                        if (j == 0)
                                break;
                        memset(freq, 0, sizeof(freq));
                        for (j = 0; j < NDICE; j++)
                                freq[dice[i][j] - 1]++;
                        for (j = 0; j < NCAT; j++)
                                get_cat_val(cat_val, i, j, freq);
                }
                if (i == 0)
                        break;
                DP();
        }

        return 0;
}

