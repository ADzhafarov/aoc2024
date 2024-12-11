#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXWORD          100
#define DEFAULT_DEPTH    25
#define YEAR             2024

int getword(char *word, int maxlen);

typedef struct _tnode {
    unsigned long val;
    unsigned long *levels;
    struct _tnode *left;
    struct _tnode *right;
} TNode;

TNode *add(TNode *tree, unsigned long val, int level, unsigned long times);
void calc_level(TNode *subtree, TNode **tree, int level);
void sum_up_results(TNode *tree, unsigned long *result);

int depth;
void print_tree(TNode *tree);

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    char word[MAXWORD];
    unsigned long result;
    int i;
    TNode *tree = NULL;


    if (argc == 1)
        depth = DEFAULT_DEPTH;
    else
        depth = atoi(argv[1]);

    while (getword(word, MAXWORD) != EOF) {
        tree = add(tree, atol(word), 0, 1);
    }

    for (i = 1; i <= depth; i++) {
        calc_level(tree, &tree, i);
    }

    result = 0;
    sum_up_results(tree, &result);

    printf("Result: %lu\n", result);
}

TNode *add(TNode *tree, unsigned long val, int level, unsigned long times) {

    TNode *new_node;

    if (tree == NULL) {
        new_node = (TNode *)malloc(sizeof(TNode));
        new_node->val = val;
        new_node->levels = (unsigned long *)calloc(depth + 1, sizeof(long));
        new_node->levels[level] = times;
        new_node->left = NULL;
        new_node->right = NULL;
        tree = new_node;
    } else if (val < tree->val) {
        tree->left = add(tree->left, val, level, times);
    } else if (val > tree->val) { 
        tree->right = add(tree->right, val, level, times);
    } else {
        tree->levels[level] += times;
    }

    return tree;
}

void print_tree(TNode *tree) {
    int i;
    if (tree == NULL)
        return;

    print_tree(tree->left);
    printf("%15ld", tree->val);
    for (i = 0; i <= depth; i++)
        printf("%4lu", tree->levels[i]);
    printf("\n");
    print_tree(tree->right);
}

void calc_level(TNode *subtree, TNode **tree, int level) {

    unsigned long power;
    int order;
    if (subtree == NULL)
        return;

    if (subtree->levels[level-1] != 0) {
        if (subtree->val == 0)
            *tree = add(*tree, 1, level, subtree->levels[level-1]);
        else {
            power = 1;
            order = 0;
            while (power <= subtree->val) {
                power *= 10;
                order++;
            }

            if (order % 2) {
                *tree = add(*tree, subtree->val * YEAR, level, subtree->levels[level-1]);
            } else {
                order /= 2;
                while (--order >= 0)
                    power /= 10;
                *tree = add(*tree, subtree->val / power, level, subtree->levels[level-1]);
                *tree = add(*tree, subtree->val % power, level, subtree->levels[level-1]);
            }
        }
    }


    calc_level(subtree->left, tree, level);
    calc_level(subtree->right, tree, level);
}

void sum_up_results(TNode *tree, unsigned long *results) {
    if (tree == NULL)
        return;

    *results += tree->levels[depth];
    sum_up_results(tree->left, results);
    sum_up_results(tree->right, results);
}

int getword(char *word, int maxlen) {
    while (isblank(*word = getchar()))
        ;

    if (*word == EOF)
        return EOF;

    word++;
    maxlen--;

    while (--maxlen >= 0 && !isblank(*word = getchar()) && *word != EOF)
        word++;

    ungetc(*word, stdin);
    *word = '\0';
    
    return 1;
}
