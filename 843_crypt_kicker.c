/* 
 * Author: Chen Rushan
 * E-Mail: chenrsster@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_WORD_LEN 17
#define MAX_WORD_CNT 1001
#define MAX_LINE_LEN 82

struct word_t {
        int idx;
        struct word_t *next;
};

struct dict_t {
        struct word_t **lenarr;
        struct word_t *wrds;
        char *content;
};

struct cand_t {
        struct {
                struct word_t **candlst;
                int candlst_len;
        } *candarr;
        size_t candarr_sz;
        int candarr_len;
};

struct dict_t *
construct_dict(void)
{
        struct dict_t *dict = NULL;
        size_t memsz = 0;

        /* alloc memory
         */
        memsz = sizeof(*dict) +
                MAX_WORD_LEN * sizeof(*dict->lenarr) +
                MAX_WORD_CNT * sizeof(*dict->wrds) +
                (MAX_WORD_LEN * MAX_WORD_CNT + MAX_WORD_CNT) * sizeof(char);
        dict = calloc(1, memsz);
        dict->lenarr = (struct word_t **)(dict + 1);
        dict->wrds = (struct word_t *)(dict->lenarr + MAX_WORD_LEN);
        dict->content = (char *)(dict->wrds + MAX_WORD_CNT);

        /* insert words
         */
        int i = 0, nwrds = 0;
        int dict_wrds_idx = 0, dict_cont_idx = 0;
        char wrd[MAX_WORD_LEN + 1];
        scanf("%d\n", &nwrds);
        for (i = 0; i < nwrds; i++) {
                scanf("%s\n", wrd);

                /* insert into dict */
                int l = strlen(wrd);
                dict->wrds[dict_wrds_idx].next = dict->lenarr[l];
                dict->wrds[dict_wrds_idx].idx = dict_cont_idx;
                dict->lenarr[l] = dict->wrds + dict_wrds_idx;
                strcpy(dict->content + dict_cont_idx, wrd);

                dict_wrds_idx++;
                dict_cont_idx += l + 1;
        }

        return dict;
}

/**
 * @dec is 26-char long
 */
int
is_injective(const char *dec)
{
        char flg[26] = {};
        int i = 0;

        for (i = 0; i < 26; i++) {
                char c = dec[i];
                if (c == '\0')
                        continue;
                if (flg[c - 'a'] == 0)
                        flg[c - 'a'] = 1;
                else
                        return 0;
        }
        return 1;
}

int
is_match_dec(const char *enwrd, const char *wrd, int l, char *dec)
{
        int i = 0;
        for (i = 0; i < l; i++) {
                char c = enwrd[i];
                if (dec[c - 'a'] == '\0')
                        dec[c - 'a'] = wrd[i];
                else if (dec[c - 'a'] != wrd[i])
                        return 0;
        }
        return is_injective(dec);
}

/**
 * Check if an encrypted word @enwrd matches @wrd.
 * @l is the length of @wrd and @enwrd
 */
int
is_match(const char *enwrd, const char *wrd, int l)
{
        char dec[26] = {};

        return is_match_dec(enwrd, wrd, l, dec);
}

void
extract_cand(struct cand_t *cand, struct dict_t *dict, char **enwrds, int nenwrds)
{
        int i = 0;

        /* realloc candarr if needed
         */
        if (cand->candarr_sz < nenwrds) {
                cand->candarr = realloc(cand->candarr, nenwrds *
                                        sizeof(*cand->candarr));
                for (i = cand->candarr_sz; i < nenwrds; i++) {
                        cand->candarr[i].candlst = calloc(MAX_WORD_CNT,
                                sizeof(*(cand->candarr[i].candlst)));
                        cand->candarr[i].candlst_len = 0;
                }
                cand->candarr_sz = nenwrds;
        }

        /* extract candidates
         */
        for (i = 0; i < nenwrds; i++) {
                int l = strlen(enwrds[i]);
                struct word_t *p = NULL;
                struct word_t **clst = cand->candarr[i].candlst;
                int j = 0; /* index of clst */

                for (p = dict->lenarr[l]; p != NULL; p = p->next) {
                        int m = is_match(enwrds[i], dict->content + p->idx, l);
                        if (m == 1)
                                clst[j++] = p;
                }

                cand->candarr[i].candlst_len = j;
        }
        cand->candarr_len = nenwrds;
}

/**
 * @cont is dict->content
 * @idx is the index of enwrds and cand->candarr
 * @decry[i] is the index of cand->candarr[idx].candlst
 * @dec is 26-char long
 */
int
backtract(char **enwrds, int nenwrds, struct cand_t *cand, char *cont,
                                      int idx, int *decry, char *dec)
{
        if (idx == nenwrds) /* decryption found */
                return 1;
        int i = 0;
        int l = strlen(enwrds[idx]);
        struct word_t **clst = cand->candarr[idx].candlst;
        char dec_bak[26];

        memcpy(dec_bak, dec, 26);
        for (i = 0; i < cand->candarr[idx].candlst_len; i++) {
                int m = is_match_dec(enwrds[idx], cont + clst[i]->idx, l, dec);
                if (m == 1) {
                        decry[idx] = i;
                        int ok = backtract(enwrds, nenwrds, cand, cont, idx + 1,
                                                                decry, dec);
                        if (ok == 1)
                                return 1;
                }
                memcpy(dec, dec_bak, 26);
        }

        return 0;
}

int
main(int argc, char **argv)
{
        struct dict_t *dict = NULL;
        char line[MAX_LINE_LEN] = {};
        char *enwrds[MAX_LINE_LEN] = {};
        struct cand_t cand = {NULL, 0, 0};
        int *decry = NULL;
        int i = 0, decry_sz = 0;

        dict = construct_dict();

        while (fgets(line, MAX_LINE_LEN, stdin)) {
                int nenwrds = 0;

                /* parse line
                 */
                for (i = 0; line[i] != '\0';) {
                        enwrds[nenwrds++] = line + i;
                        while (line[i] != ' ' && line[i] != '\n')
                                i++;
                        line[i++] = '\0';
                }

                if (decry_sz < nenwrds) {
                        decry = realloc(decry, nenwrds * sizeof(*decry));
                        decry_sz = nenwrds;
                }

                /* extract candidates for each encrypted words
                 */
                extract_cand(&cand, dict, enwrds, nenwrds);

                /* search decryption
                 */
                int ok = 0;
                char dec[26] = {};
                ok = backtract(enwrds, nenwrds, &cand, dict->content, 0, decry, dec);
                if (ok == 1) {
                        for (i = 0; i < nenwrds; i++) {
                                int j = decry[i];
                                int ci = cand.candarr[i].candlst[j]->idx;
                                if (i == nenwrds - 1)
                                        printf("%s\n", dict->content + ci);
                                else
                                        printf("%s ", dict->content + ci);
                        }
                } else {
                        for (i = 0; i < nenwrds; i++) {
                                int l = strlen(enwrds[i]);
                                int j = 0;
                                for (j = 0; j < l; j++)
                                        putchar('*');
                                if (i == nenwrds - 1)
                                        putchar('\n');
                                else
                                        putchar(' ');
                        }
                }
        }

        /* free memory
         */
        free(dict);
        for (i = 0; i < cand.candarr_sz; i++)
                free(cand.candarr[i].candlst);
        free(cand.candarr);
        free(decry);

        return 0;
}

