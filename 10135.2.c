/* 
 * Author: Chen Rushan
 * E-Mail: juscodit@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#define MAX_NPOINT 1002

struct t_point {
    float x;
    float y;
};

/**
 * RETURN:
 *  = 0: (p, q, r) on a line
 *  < 0: clockwise
 *  > 0: counterclockwise
 */
int
point_orient(struct t_point *p, struct t_point *q, struct t_point *r)
{
    double v = (r->y - q->y) * (q->x - p->x) - (q->y - p->y) * (r->x - q->x);
    if (v < 0) {
        return -1;
    } else if (v > 0) {
        return 1;
    }
    return 0;
}

/**
 * RETURN:
 *   1: if @p is lower than @q
 *   0: if @p is higher than @q
 */
int
point_is_lower(struct t_point *p, struct t_point *q)
{
    if (p->y != q->y) {
        return p->y < q->y;
    } else {
        return p->x < q->x;
    }
}

/**
 * compare points by coordinate
 * RETURN:
 *   1: if @p is larger than @q
 *   0: if @p is equal @q
 *  -1: if @p is smaller than @q
 */
int
point_cmp_by_coor(struct t_point *p, struct t_point *q)
{
    if (p->y != q->y) {
        return p->y > q->y;
    } else if (p->x != q->x) {
        return p->x > q->x;
    }
    return 0;
}

float
point_dist(struct t_point *p, struct t_point *q)
{
    return sqrt((p->y - q->y) * (p->y - q->y) +
                (p->x - q->x) * (p->x - q->x));
}

struct t_point_set {
    struct t_point points[MAX_NPOINT];
    int npoints;
    int order[MAX_NPOINT]; /* order according to angle with the lowest point */
    int order0[MAX_NPOINT]; /* order according to angle with the origin */
    int idx2order0[MAX_NPOINT]; /* map from index in @points to order in @order0 */
    int lowest; /* the index of lowest point in @points */
    int origin; /* the index of origin in @points */
};

struct t_point_set pset;

static int
qsort_cmp_by_coor(const void *p, const void *q)
{
    return point_cmp_by_coor((struct t_point *)p, (struct t_point *)q);
}

static int
qsort_cmp_by_lowest(const void *_i, const void *_j)
{
    int i = *(int *)_i, j = *(int *)_j;

    return point_orient(pset.points + i, pset.points + pset.lowest,
                        pset.points + j);
}

static int
qsort_cmp_by_origin(const void *_i, const void *_j)
{
    
}

static void
pset_remove_dup(struct t_point_set *pset)
{
    int i = 0, nps = 0;

    /* sort point set first */
    qsort(pset->points, pset->npoints, sizeof(*pset->points), qsort_cmp_by_coor);

    /* remove dup */
    for (i = 1; i < pset->npoints; ++i) {
        if (point_cmp_by_coor(pset->points + i, pset->points + nps)) {
            pset->points[++nps] = pset->points[i];
        }
    }
    pset->npoints = nps + 1;
}

void
pset_init(struct t_point_set *pset)
{
    int i = 0;

    memset(pset, 0, sizeof(*pset));

    /* read in point set */
    scanf("%d", &pset->npoints);
    for (i = 0; i < pset->npoints; ++i) {
        float x, y;
        scanf("%f%f", &x, &y);
        pset->points[i].x = x;
        pset->points[i].y = y;
        pset->order[i] = i;
        pset->order0[i] = i;
    }

    /* append origin */
    pset->points[pset->npoints].x = 0;
    pset->points[pset->npoints].y = 0;
    pset->origin = pset->npoints;
    pset->npoints += 1;

    pset_remove_dup(pset);

    /* set lowest */
    pset->lowest = 0;

    /* set origin */
    for (i = 0 ; i < pset->npoints; ++i) {
        if (pset->points[i].x == 0 && pset->points[i].y == 0) {
            pset->origin = i;
            break;
        }
    }
}

/**
 * set pset->order[]
 */
void
pset_order_by_lowest(struct t_point_set *pset)
{
    /* sort pset->order[], pset->points[0] is already the lowest */
    qsort(pset->order + 1, pset->npoints - 1, sizeof(*pset->order),
          qsort_cmp_by_lowest);
}

void
pset_order_by_origin(struct t_point_set *pset)
{
    /* swap the position of origin and 0 */
    pset->order0[0] = pset->origin;
    pset->order0[pset->origin] = 0;

    /* order0[0] is the orgin */
    qsort(pset->order0 + 1, pset->npoints - 1, sizeof(*pset->order0),
          qsort_cmp_by_origin);
}

void
pset_print(struct t_point_set *pset)
{
    int i = 0;

    for (i = 0; i < pset->npoints; ++i) {
        printf("(%.2f %.2f)\n", pset->points[i].x, pset->points[i].y);
    }
    printf("\n");
}

int
main(int argc, char **argv)
{
    int ncases = 0;

    scanf("%d", &ncases);
    while (ncases--) {
        pset_init(&pset);
    }
}

