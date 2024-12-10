#include <stdio.h>
#include <stdlib.h>

#define MAXWIDTH        57
#define MAXHEIGHT       57

void visit(int i, int j, char *map[], int *result);

int width;
int height = 0;

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    char *map[MAXHEIGHT] = {NULL};
    size_t max_width = MAXWIDTH;
    int i, j;
    int p, q;
    int result = 0;
    int len;

    printf("Reading input... ");
    while (height < MAXHEIGHT && (len = getline(&map[height++], &max_width, stdin)) > 0)
        width = len - 1;

    printf("Success\n");
    printf("Height: %d\tWidth: %d\n", height, width);

    printf("Traversing... ");
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
            if (map[i][j] == '0') {
                visit(i, j, map, &result);
            }
    printf("Success\n");

    printf("Freeing memory... ");
    for (i = 0; i < height; i++)
        free(map[i]);
    printf("Success\n");

    printf("The total score is\n"
           "****** %d ******\n", result);
}

#define within(i,j)     ((0 <= (i)) && ((i) < height) && (0 <= (j)) && ((j) < width))
#define m(i,j)          map[i][j]

void visit(int i, int j, char *map[], int *result) {
    printf("\n\tAt (%d, %d)", i+1, j+1);
    int x, y;

    if (map[i][j] == '9') {
        (*result)++;
        return;
    }

    x = i + 1;
    y = j;
    if (within(x,y) && (m(x,y) - m(i,j) == 1))
        visit(x, y, map, result);
        
    x = i;
    y = j + 1;
    if (within(x,y) && (m(x,y) - m(i,j) == 1))
        visit(x, y, map, result);

    x = i - 1;
    y = j;
    if (within(x,y) && (m(x,y) - m(i,j) == 1))
        visit(x, y, map, result);

    x = i;
    y = j - 1;
    if (within(x,y) && (m(x,y) - m(i,j) == 1))
        visit(x, y, map, result);
}
