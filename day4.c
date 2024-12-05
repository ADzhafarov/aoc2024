#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXROWS     140
#define MAXROWLEN   140

#define r(i,j)              rows[(i)][(j)]
#define mas(m,a,s)       ((m) == 'M') && ((a) == 'A') && ((s) == 'S')
#define ms(x,y)       (((x) == 'M') && ((y) == 'S') || ((x) == 'S') && ((y) == 'M'))

int main(void) {
    char *rows[MAXROWS];
    size_t max_row_len = MAXROWLEN;
    int height, width;
    int i, j;
    int left_possible, right_possible, up_possible, down_possible;
    int xmas_counter, x_mas_counter;

    printf("Allocating memory for storing rows... ");
    for (i = 0; i < MAXROWS; i++)
        rows[i] = (char *)malloc(MAXROWLEN + 2);
    printf("Success!\n");

    printf("Scaning lines of input into memory... ");
    i = 0;
    while (i < MAXROWS && getline(rows + i++, &max_row_len, stdin) > 0)
        ;
    printf("Successfully scanned %d lines ", i);

    height = i;
    width = strlen(rows[0]);

    printf("of length %d\n", width);

    xmas_counter = 0;
    printf("Counting XMASes... ");
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++) {

            if (r(i,j) != 'X')
                continue;

            left_possible = j >= 3;
            right_possible = j <= width - 4;
            up_possible = i >= 3;
            down_possible = i <= height - 4;

            // horizontal
            if (left_possible && mas(r(i,j-1), r(i, j-2), r(i, j-3)))
                xmas_counter += 1;
            if (right_possible && mas(r(i,j+1), r(i, j+2), r(i, j+3)))
                xmas_counter += 1;

            // vertical
            if (up_possible && mas(r(i-1,j), r(i-2, j), r(i-3, j)))
                xmas_counter += 1;
            if (down_possible && mas(r(i+1,j), r(i+2, j), r(i+3, j)))
                xmas_counter += 1;

            // diagonal
            if (left_possible && up_possible && mas(r(i-1,j-1), r(i-2,j-2), r(i-3,j-3)))
                xmas_counter +=1;
            if (right_possible && up_possible && mas(r(i-1,j+1), r(i-2,j+2), r(i-3,j+3)))
                xmas_counter +=1;
            if (right_possible && down_possible && mas(r(i+1,j+1), r(i+2,j+2), r(i+3,j+3)))
                xmas_counter +=1;
            if (left_possible && down_possible && mas(r(i+1,j-1), r(i+2,j-2), r(i+3,j-3)))
                xmas_counter +=1;
        }
    printf("success!\n");

    printf("Counting X-MASes... ");
    x_mas_counter = 0;
    for (i = 1; i < height - 1; i++)
        for (j = 1; j < width - 1; j++) {
            if (r(i,j) == 'A' && ms(r(i-1,j-1), r(i+1,j+1)) && ms(r(i-1,j+1), r(i+1,j-1))) {
                x_mas_counter += 1;
            }
        }


    printf("Freeing memory... ");
    for (i = 0; i < MAXROWS; i++)
        free(rows[i]);
    printf("success!\n");

    printf("The amount of XMAS found is\n"
           "***************************** %d *****************************\n", xmas_counter);
    printf("The amount of X-MAS found is\n"
           "***************************** %d *****************************\n", x_mas_counter);
}
