#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILENAME    "input.txt"

typedef struct {
    int *values;
    int size;
} Column;

typedef struct _tree_node {
    int val;
    int count;
    struct _tree_node *left;
    struct _tree_node *right;
} TNode;

int fill_arrays(FILE *fp, Column *left, Column *right, TNode **right_tree);
int comp_int(int *a, int *b);
Column *initialize_column(void);
TNode *initialize_tree(void);
void free_column(Column *col);
void free_tree(TNode *tree);
int get_count(TNode *tree, int val);
TNode *add_val(TNode *tree, int val);
unsigned long calc_dist(Column *left, Column *right, int len);
unsigned long calc_sim(Column *left, TNode *right_tree, int len);
void print_column(Column *col, int len);
void print_tree(TNode *tree);

int main(int argc, char *argv[]) {
    FILE *fp;
    int len;
    char *filename;
    Column *left, *right;
    unsigned long distance, similarity;
    TNode *right_tree;

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
    printf("Initializing right tree... ");
    right_tree = initialize_tree();
    
    // scan file into two dynamic arrays
    len = fill_arrays(fp, left, right, &right_tree);

    // sort both arrays
    printf("Sorting arrays... ");
    qsort(left->values, len, sizeof(int), (__compar_fn_t) comp_int);
    /*print_column(left, len);*/
    qsort(right->values, len, sizeof(int), (__compar_fn_t) comp_int);
    /*print_column(right, len);*/
    printf("Success\n");

    // iterate over arrays and compute the result
    distance = calc_dist(left, right, len);
    similarity = calc_sim(left, right_tree, len);

    printf("Freeing memory... ");
    free_column(left);
    free_column(right);
    free_tree(right_tree);
    fclose(fp);
    printf("Success\n");

    printf("The distance between left and right lists is\n"
           "**************************** %lu ****************************\n", distance);
    printf("The similarity score between left and right lists is\n"
           "**************************** %lu ****************************\n", similarity);
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

TNode *initialize_tree(void) {
    return NULL;
}

void free_tree(TNode *tree) {
    if (tree == NULL)
        return;
    free_tree(tree->left);
    free_tree(tree->right);
    free(tree);
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
int add_numbers(Column *left, Column *right, TNode **right_tree, char *left_p, char *right_p, int len);

int fill_arrays(FILE *fp, Column *left, Column *right, TNode **right_tree) {
    int len = 0;
    size_t maxline = MAXLINE;
    char *line = (char *)malloc(maxline);
    char *left_num, *right_num;

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
        // relying on input a bit, but whatever
        for(left_num = right_num = strtok(line, " \t\n"); right_num != NULL; right_num = strtok(NULL, " \t\n"))
            if (strlen(right_num) != 0 && right_num != left_num)
                break;

        /*// Logging first 20 lines*/
        /*if (len < 20) {*/
        /*    printf("\t\tLine %d:\n", len + 1);*/
        /*    printf("\t\t\tleft: %s\n\t\t\tright: %s\n", left_num, right_num);*/
        /*} else if (len == 20) {*/
        /*    printf("\t\t..........\n");*/
        /*}*/

        len = add_numbers(left, right, right_tree, left_num, right_num, len);
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

int add_numbers(Column *left, Column *right, TNode **right_tree, char *left_p, char *right_p, int len) {
    int right_value;

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

    right_value = atoi(right_p);
    right->values[len] = right_value;
    *right_tree = add_val(*right_tree, right_value);

    return len + 1;
}

int get_count(TNode *tree, int val) {
    if (tree == NULL)
        return 0;
    else if (val < tree->val)
        return get_count(tree->left, val);
    else if (val > tree->val)
        return get_count(tree->right, val);
    else
        return tree->count;
}

TNode *add_val(TNode *tree, int val) {
    TNode *new_node;

    if (tree == NULL) {
        new_node = (TNode *)malloc(sizeof(TNode));
        new_node->val = val;
        new_node->count = 1;
        new_node->left = NULL;
        new_node->right = NULL;
        return new_node;
    } else if (val < tree->val) {
        tree->left = add_val(tree->left, val);
    } else if (val > tree->val) {
        tree->right = add_val(tree->right, val);
    } else {
        tree->count++;
    }
    return tree;
}

unsigned long calc_sim(Column *left, TNode *right_tree, int len) {
    int prev = -1;
    int count = 0;
    unsigned long sim = 0;

    printf("Calculating distance... ");
    for (int i = 0; i < len; i++) {
        if (left->values[i] == prev) {
            count++;
        } else {
            if (prev != -1)
                sim += count * prev * get_count(right_tree, prev);
            prev = left->values[i];
            count = 1;
        }
    }

    sim += count * prev * get_count(right_tree, prev);
    printf("Success\n");
    return sim;
}

unsigned long calc_dist(Column *left, Column *right, int len) {
    unsigned long res = 0;

    printf("Calculating distance... ");
    for (int i = 0; i < len; i++) {
        res += abs(left->values[i] - right->values[i]);
    }

    printf("Success\n");

    return res;
}

