/* 
 * Name: chen rushan
 * mail: chenrslucky@gmail.com
 * uva: 352
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIM 25

struct t_pixel {
    int x, y;
};

int img[MAX_DIM][MAX_DIM];
int img_dim;
struct t_pixel queue[MAX_DIM * MAX_DIM];
int npix; /* number of pixels in queue */
int img_no; /* number of image */

void
print_img()
{
    int i = 0, j = 0;
    for (i = 0; i < img_dim; ++i) {
        for (j = 0; j < img_dim; ++j) {
            printf("%d", img[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void
add_to_queue(int x, int y)
{
    queue[npix].x = x;
    queue[npix].y = y;
    npix += 1;
    img[x][y] = 0;
}

/*
 * (@x, @y) is starting point to flood fill
 */
void
flood_fill(int x, int y)
{
    npix = 0;
    add_to_queue(x, y);

    while (npix != 0) {
        struct t_pixel p = queue[npix - 1];
        npix -= 1;

        /* left */
        if (p.x > 0 && img[p.x - 1][p.y] == 1) {
            add_to_queue(p.x - 1, p.y);
        }
        /* right */
        if (p.x < img_dim - 1 && img[p.x + 1][p.y] == 1) {
            add_to_queue(p.x + 1, p.y);
        }
        /* up */
        if (p.y > 0 && img[p.x][p.y - 1] == 1) {
            add_to_queue(p.x, p.y - 1);
        }
        /* down */
        if (p.y < img_dim - 1 && img[p.x][p.y + 1] == 1) {
            add_to_queue(p.x, p.y + 1);
        }
        /* up-right */
        if (p.x > 0 && p.y < img_dim - 1 && img[p.x - 1][p.y + 1] == 1) {
            add_to_queue(p.x - 1, p.y + 1);
        }
        /* up-left */
        if (p.x > 0 && p.y > 0 && img[p.x - 1][p.y - 1] == 1) {
            add_to_queue(p.x - 1, p.y - 1);
        }
        /* down-left */
        if (p.y > 0 && p.x < img_dim - 1 && img[p.x + 1][p.y - 1] == 1) {
            add_to_queue(p.x + 1, p.y - 1);
        }
        /* down-right */
        if (p.x < img_dim - 1 && p.y < img_dim - 1 && img[p.x + 1][p.y + 1] == 1) {
            add_to_queue(p.x + 1, p.y + 1);
        }
    }
}

void
get_n_eagles(void)
{
    int i = 0, j = 0;
    int cnt = 0;

    for (i = 0; i < img_dim; ++i) {
        for (j = 0; j < img_dim; ++j) {
            if (img[i][j] == 1) {
                flood_fill(i, j);
                cnt += 1;
            }
        }
    }

    printf("Image number %d contains %d war eagles.\n", img_no, cnt);
}

int
main(int argc, char **argv)
{
    int i = 0, j = 0;
    char line[MAX_DIM + 10];

    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        img_dim = atoi(line);
        for (i = 0; i < img_dim; ++i) {
            fgets(line, sizeof(line), stdin);
            for (j = 0; line[j] != '\n'; ++j) {
                img[i][j] = line[j] - '0';
            }
        }
        img_no += 1;

        get_n_eagles();
    }
    return 0;
}

