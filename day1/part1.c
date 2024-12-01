#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define FILENAME    "input.txt"

typedef struct {
    int *values;
    int size;
} Column;

int fill_arrays(FILE *fp, Column *left, Column *right);
int comp_int(int *a, int *b);
Column *initialize_column();
void free_column(Column *col);
unsigned long calc_res(Column *left, Column *right, int len);
void print_column(Column *col, int len);

int main(int argc, char *argv[]) {
    FILE *fp;
    int len;
    char *filename;
    Column *left, *right;
    unsigned long distance;

    if (argc == 1) {
        filename = FILENAME;
        printf("Default filename is used: %s\n", filename);
    } else {
        filename = argv[1];
        printf("Filename derived from command-line arguements: %s\n", filename);
    }


    // open file
    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("hh: can't open file %s\n", filename);
        exit(1);
    } else
        printf("File %s opened successfully\n", filename);
    
    printf("Initializing left column... ");
    left = initialize_column();
    printf("Initializing right column... ");
    right = initialize_column();
    
    // scan file into two dynamic arrays
    len = fill_arrays(fp, left, right);

    // sort both arrays

    printf("Sorting arrays... ");
    qsort(left->values, len, sizeof(int), (__compar_fn_t) comp_int);
    /*print_column(left, len);*/
    qsort(right->values, len, sizeof(int), (__compar_fn_t) comp_int);
    /*print_column(right, len);*/
    printf("Success\n");

    // iterate over arrays and compute the result
    distance = calc_res(left, right, len);

    printf("Freeing memory... ");
    free_column(left);
    free_column(right);
    fclose(fp);
    printf("Success\n");

    printf("The distance between left and right lists is\n"
           "**************************** %lu ****************************\n", distance);
    exit(0);
}

void print_column(Column *col, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d\n", col->values[i]);
    }
}

#define START_SIZE  250

Column *initialize_column() {
    Column *col = (Column *)malloc(sizeof(Column));
    col->size = START_SIZE;
    col->values = (int *)malloc(START_SIZE * sizeof(int));
    if (col->values == NULL) {
        printf("Error initializing column\n");
    } else {
        printf("Success\n");
    }

    return col;
}

void free_column(Column *col) {
    free(col->values);
    free(col);
}

int comp_int(int *a, int *b) {
    if (*a < *b)
        return -1;
    else if (*a > *b)
        return 1;
    else
        return 0;
}

#define MAXLINE     100
int add_numbers(Column *left, Column *right, char *left_p, char *right_p, int len);

int fill_arrays(FILE *fp, Column *left, Column *right) {
    int len = 0;
    size_t maxline = MAXLINE;
    char *line = (char *)malloc(maxline);

    printf("Reading columns from input...\n");

    printf("\tAllocating line buffer... ");
    if (line == NULL) {
        printf("fill_arrays: can't allocate memory for line buffer\n");
        free_column(left);
        free_column(right);
        exit(2);
    } else {
        printf("Success\n");
    }

    printf("\tReading lines...\n");
    while (fgets(line, MAXLINE, fp)) {
        char *left_num, *right_num;

        // relying on input a bit, but whatever
        for(left_num = right_num = strtok(line, " \t\n"); right_num != NULL; right_num = strtok(NULL, " \t\n"))
            if (strlen(right_num) != 0 && right_num != left_num)
                break;

        // Logging first 20 lines
        if (len < 20) {
            printf("\t\tLine %d:\n", len + 1);
            printf("\t\t\tleft: %s\n\t\t\tright: %s\n", left_num, right_num);
        } else if (len == 20) {
            printf("\t\t..........\n");
        }

        len = add_numbers(left, right, left_num, right_num, len);
        if (len == -1) {
            free(line);
            free_column(left);
            free_column(right);
        }
    }


    free(line);

    printf("\tRead %d lines\n", len);
    return len;
}

int add_numbers(Column *left, Column *right, char *left_p, char *right_p, int len) {
    if (len >= left->size) {
        printf("\t\tReallocating arrays... ");
        left->size *= 2;
        right->size *= 2;

        left->values = realloc(left->values, left->size * sizeof(int));
        right->values = realloc(right->values, right->size * sizeof(int));

        if (left->values == NULL || right->values == NULL) {
            printf("Error reallocating columns\n");
            return -1;
        } else {
            printf("Success\n");
        }

    }

    left->values[len] = atoi(left_p);
    right->values[len] = atoi(right_p);

    return len + 1;
}

unsigned long calc_res(Column *left, Column *right, int len) {
    unsigned long res = 0;

    printf("Aggregating results... ");
    for (int i = 0; i < len; i++) {
        res += abs(left->values[i] - right->values[i]);
    }

    printf("Success\n");

    return res;
}

