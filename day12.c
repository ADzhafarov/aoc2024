#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE     150
#define MAXHEIGHT   150

int width;
int height = 0;

#define r(i,j)      rows[i][j]

void paint(char *rows[], int i, int j, int regions, int *perimeter, int *area, int *sides);

int main(void) {
    size_t maxline = MAXLINE;
    char *rows[MAXHEIGHT] = {NULL};
    int line_len, i, j;
    int perimeter, area, sides;
    int region = 0;
    long no_discount = 0;
    long discount = 0;

    printf("Reading input... \n");
    while ((line_len = getline(&rows[height], &maxline, stdin)) > 0) {
        width = line_len - 1;
        height++;
    }
    printf("\tSuccess\n");

    
    printf("Calculating perimeters and areas...\n");
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
            if (r(i,j) > 0) {
                region = r(i,j);
                perimeter = area = sides = 0;
                paint(rows, i, j, region, &perimeter, &area, &sides);
                printf("\t%10s: %4c\n"
                       "\t\t%10s: %4d\n"
                       "\t\t%10s: %4d\n"
                       "\t\t%10s: %4d\n",
                       "Region", region,
                       "Perimeter", perimeter,
                       "Area", area,
                       "Sides", sides
                   );
                no_discount += area * perimeter;
                discount += area * sides;
            }
    printf("\tSuccess\n");

    printf("Calculating total cost...\n");
    printf("\tSuccess\n");

    printf("Freeing memory... \n");
    for (i = 0; i <= height; i++)
        free(rows[i]);
    printf("\tSuccess\n");


    printf("Total cost without disctount is:\n"
           "**** %ld ****\n", no_discount);

    printf("Total cost with disctount is:\n"
           "**** %ld ****\n", discount);
}

int calc_perim(char *rows[], int i, int j, int regions);
int calc_angles(char *rows[], int i, int j, int regions);

#define within(i,j)     ((0 <= (i)) && ((i) < height) && (0 <= (j)) && ((j) < width))

void paint(char *rows[], int i, int j, int region, int *perimeter, int *area, int *sides) {
    *perimeter += calc_perim(rows, i, j, region);
    *sides += calc_angles(rows, i, j, region);
    *area += 1;
    r(i,j) = -region;
    if (within(i+1,j) && (r(i+1,j) == region))
        paint(rows, i+1, j, region, perimeter, area, sides);
    if (within(i-1,j) && (r(i-1,j) == region))
        paint(rows, i-1, j, region, perimeter, area, sides);
    if (within(i,j+1) && (r(i,j+1) == region))
        paint(rows, i, j+1, region, perimeter, area, sides);
    if (within(i,j-1) && (r(i,j-1) == region))
        paint(rows, i, j-1, region, perimeter, area, sides);
}

#define fence(i,j)      (!within(i,j) || !((r(i,j) == region) || r(i,j) == -region))

int calc_angles(char *rows[], int i, int j, int region) {
    int res = 0;

    if (fence(i,j-1) && fence(i-1,j)) res++;

    if (!fence(i,j-1) && !fence(i-1,j) && !(r(i-1,j-1) == region || r(i-1,j-1) == -region)) res++;

    if (fence(i,j+1) && fence(i-1,j)) res++;

    if (!fence(i,j+1) && !fence(i-1,j) && !(r(i-1,j+1) == region || r(i-1,j+1) == -region)) res++;

    if (fence(i,j+1) && fence(i+1,j)) res++;

    if (!fence(i,j+1) && !fence(i+1,j) && !(r(i+1,j+1) == region || r(i+1,j+1) == -region)) res++;

    if (fence(i,j-1) && fence(i+1,j)) res++;

    if (!fence(i,j-1) && !fence(i+1,j) && !(r(i+1,j-1) == region || r(i+1,j-1) == -region)) res++;

    return res;
}

int calc_perim(char *rows[], int i, int j, int region) {
    int res = 0;

    if (fence(i+1,j)) res++;
    if (fence(i-1,j)) res++;
    if (fence(i,j+1)) res++;
    if (fence(i,j-1)) res++;

    return res;
}

