/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

enum t_cat {
        HIGH_CARD, PAIR, TWO_PAIRS, THREE_OF_A_KIND,
        STRAIGHT, FLUSH, FULL_HOUSE, FOUR_OF_A_KIND,
        STRAIGHT_FLUSH,
};

struct hand_info {
        enum t_cat cat;

        char is_flush;
        char is_straight;

        char n_sgls;
        char n_pairs;
        char has_3ofak;
        char has_4ofak;

        char val_sgls[5];
        char val_pairs[2];
        char val_3ofak;
        char val_4ofak;
};

void
analy_hand(char *hand, struct hand_info *phi)
{
        int i = 0, nsame = 0, pair_beg = 0;

        memset(phi, 0, sizeof(*phi));
        phi->is_flush = 1;
        phi->is_straight = 1;

        nsame = 1;
        for (i = 1; i < 5; i++) {
                char cur_val = hand[i] >> 2, pre_val = hand[i - 1] >> 2;
                char cur_suit = hand[i] & 0x03, pre_suit = hand[i - 1] & 0x03;

                if (cur_val != pre_val + 1)
                        phi->is_straight = 0;
                if (cur_suit != pre_suit)
                        phi->is_flush = 0;

                if (cur_val == pre_val) {
                        nsame++;
                } else {
                        switch (nsame) {
                        case 1:
                                phi->val_sgls[phi->n_sgls++] = pre_val;
                                break;

                        case 2:
                                phi->val_pairs[phi->n_pairs++] = pre_val;
                                break;

                        case 3:
                                phi->has_3ofak = 1;
                                phi->val_3ofak = pre_val;
                                break;

                        case 4:
                                phi->has_4ofak = 1;
                                phi->val_4ofak = pre_val;
                                break;
                        }
                        nsame = 1;
                }
        }

        /* last case
         */
        switch (nsame) {
        case 1:
                phi->val_sgls[phi->n_sgls++] = hand[4] >> 2;
                break;

        case 2:
                phi->val_pairs[phi->n_pairs++] = hand[4] >> 2;
                break;

        case 3:
                phi->has_3ofak = 1;
                phi->val_3ofak = hand[4] >> 2;
                break;

        case 4:
                phi->has_4ofak = 1;
                phi->val_4ofak = hand[4] >> 2;
                break;
        }

        /* decide category
         */
        if (phi->is_flush && phi->is_straight)
                phi->cat = STRAIGHT_FLUSH;
        else if (phi->is_flush)
                phi->cat = FLUSH;
        else if (phi->is_straight)
                phi->cat = STRAIGHT;
        else if (phi->has_4ofak)
                phi->cat = FOUR_OF_A_KIND;
        else if (phi->n_pairs == 1 && phi->has_3ofak)
                phi->cat = FULL_HOUSE;
        else if (phi->has_3ofak)
                phi->cat = THREE_OF_A_KIND;
        else if (phi->n_pairs == 2)
                phi->cat = TWO_PAIRS;
        else if (phi->n_pairs == 1)
                phi->cat = PAIR;
        else
                phi->cat = HIGH_CARD;
}

void
ins_sort(char *hand)
{
        int i = 0, j = 0;

        for (i = 1; i < 5; i++) {
                char t = hand[i];
                for (j = i - 1; j >= 0; j--) {
                        if (hand[j] > t)
                                hand[j + 1] = hand[j];
                        else
                                break;
                }
                hand[j + 1] = t;
        }
}

int
cmp_arr(char *arr1, char *arr2, int n)
{
        int i = 0;

        for (i = n - 1; i >= 0; i--) {
                if (arr1[i] == arr2[i])
                        continue;
                if (arr1[i] > arr2[i])
                        return 1;
                else
                        return 2;
        }
        return 0;
}

int
cmp_num(char n1, char n2)
{
        if (n1 > n2)
                return 1;
        else if (n1 < n2)
                return 2;
        else
                return 0;
}

void
debug_print_hand(char *hand)
{
        int i = 0;
        for (i = 0; i < 5; i++)
                printf("%d ", hand[i]);
        putchar('\n');
}

int
main(int argc, char **argv)
{
        char c2val[128] = {};
        char hand1[5] = {}, hand2[5] = {};
        char line[128] = {};
        int i = 0;

        /* initialize c2val
         */
        /* values */
        for (i = 2; i <= 9; i++)
                c2val[i + '0'] = i;
        c2val['T'] = 10;
        c2val['J'] = 11;
        c2val['Q'] = 12;
        c2val['K'] = 13;
        c2val['A'] = 14;
        /* suits, the order doesn't matter */
        c2val['C'] = 0;
        c2val['D'] = 1;
        c2val['H'] = 2;
        c2val['S'] = 3;

        while (fgets(line, sizeof(line), stdin) != NULL) {
                int ncards = 0, j = 0;
                char *hand = hand1;

                /* read input
                 */
                for (i = 0; line[i] != '\n';) {
                        while (isspace(line[i]))
                                i++;
                        hand[j++] = (c2val[line[i]] << 2) + (c2val[line[i + 1]]);
                        ncards++;
                        if (ncards == 5) {
                                ncards = 0;
                                hand = hand2;
                                j = 0;
                        }
                        i += 2;
                }

                ins_sort(hand1);
                ins_sort(hand2);

                struct hand_info hi1, hi2;

                analy_hand(hand1, &hi1);
                analy_hand(hand2, &hi2);

                int res = 0;

                if (hi1.cat != hi2.cat) {
                        if (hi1.cat > hi2.cat)
                                res = 1;
                        else
                                res = 2;
                } else {
                        enum t_cat cat = hi1.cat;

                        switch (cat) {
                        case HIGH_CARD:
                        case STRAIGHT:
                        case FLUSH:
                        case STRAIGHT_FLUSH:
                                res = cmp_arr(hi1.val_sgls, hi2.val_sgls, 5);
                                break;

                        case PAIR:
                                if (hi1.val_pairs[0] > hi2.val_pairs[0])
                                        res = 1;
                                else if (hi1.val_pairs[0] < hi2.val_pairs[0])
                                        res = 2;
                                else
                                        res = cmp_arr(hi1.val_sgls,
                                                hi2.val_sgls, 3);
                                break;

                        case TWO_PAIRS:
                                res = cmp_arr(hi1.val_pairs, hi2.val_pairs, 2);
                                if (res == 0)
                                        res = cmp_arr(hi1.val_sgls,
                                                hi2.val_sgls, 2);
                                break;

                        case THREE_OF_A_KIND:
                                res = cmp_num(hi1.val_3ofak, hi2.val_3ofak);
                                break;

                        case FOUR_OF_A_KIND:
                                res = cmp_num(hi1.val_4ofak, hi2.val_4ofak);
                                break;

                        case FULL_HOUSE:
                                res = cmp_num(hi1.val_3ofak, hi2.val_3ofak);
                                break;
                        }
                }

                if (res == 1)
                        printf("Black wins.\n");
                else if (res == 2)
                        printf("White wins.\n");
                else
                        printf("Tie.\n");
        }

        return 0;
}

