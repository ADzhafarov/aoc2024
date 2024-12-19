#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXTOWELS       3000
#define MAXTOWEL        10

#define ALPHABET        6
#define MAXLINELEN      100
enum alphabet { W=1, U=2, B=3, R=4, G=5, S=6 };

typedef struct _tnode {
    struct _tnode **children;
} Node;

size_t max_line = 3000;
Node *towels = NULL;
char *line_buf;
unsigned long **cache;
FILE *f;

void get_towels(void);
void count_possible_designs(int start, int finish, unsigned long *result, Node *tree);
unsigned long count_total_possible_designs(int len);
void print_tree(Node *tree);

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    int i;
    line_buf = (char *)malloc(max_line);

    printf("Opening the file...\n");
    if (argc == 1)
        f = fopen("input.txt", "r");
    else if (argv[1][0] == '-' && argv[1][1] == 't')
        f = fopen("test-input.txt", "r");
    else {
        printf("Usage: ./day19 [-t]\n");
        exit(1);
    }
    if (f == NULL) {
        printf("\tCouldn't open the file\n");
        exit(2);
    }
    printf("\tSuccess\n");

    towels = (Node *)malloc(sizeof(Node));
    towels->children = (Node **)malloc(sizeof(Node *) * ALPHABET);
    for (i = 0; i < ALPHABET; i++) {
        towels->children[i] = NULL;
    }

    printf("Getting towels\n");
    get_towels();
    printf("\tSuccess\n");

    print_tree(towels);

    printf("Analysing patterns...\n");
    unsigned long result = 0;
    unsigned long count;
    cache = (unsigned long **)malloc(MAXLINELEN * sizeof(unsigned long *));

    for (i = 0; i < MAXLINELEN; i++)
        cache[i] = (unsigned long *)calloc(MAXLINELEN, sizeof(unsigned long));
    int len;
    while (len = getline(&line_buf, &max_line, f), len > 0) {
        count = 0;
        line_buf[len-1] = '\0';
        count = count_total_possible_designs(len-1);
        result += count;
    }
    printf("\tSuccess\n");


    printf("The result is %lu\n", result);

    printf("Freeing memory...\n");
    free(towels);
    free(line_buf);
    for (i = 0; i < MAXLINELEN; i++)
        free(cache[i]);
    free(cache);
    printf("\tSuccess\n");
}

int get_letter_index(char c) {
    switch (c) {
        case 'w':
            return 1;
        case 'u':
            return 2;
        case 'b':
            return 3;
        case 'r':
            return 4;
        case 'g':
            return 5;
        case '\0':
            return 6;
        default:
            return 0;
    }
}

char get_letter_by_index(int i) {
    switch (i) {
        case 1:
            return 'w';
        case 2:
            return 'u';
        case 3:
            return 'b';
        case 4:
            return 'r';
        case 5:
            return 'g';
        case 6:
            return '\0';
        default:
            return 0;
    }
}

typedef struct _lnode {
    char c;
    struct _lnode *next;
} LNode;

LNode *cur_word = NULL;

void push(char c) {
    LNode *new_node;

    new_node = (LNode *)malloc(sizeof(LNode));
    new_node->c = c;
    new_node->next = cur_word;

    cur_word = new_node;
}

void print_llist(LNode *llist) {
    if (llist == NULL)
        return;

    print_llist(llist->next);
    printf("%c", llist->c);

}

char pop(void) {
    char c = cur_word->c;

    LNode *tmp;

    tmp = cur_word;
    cur_word = cur_word->next;
    free(tmp);

    return c;
}

void print_tree(Node *tree) {
    if (tree == NULL)
        return;
    for (int i = 0; i < ALPHABET-1; i++) {
        if (tree->children[i] != NULL) {
            push(get_letter_by_index(i));
            print_tree(tree->children[i]);
            pop();
        }
    }
    int index = ALPHABET - 1;
    if (tree->children[index] != NULL) {
        print_llist(cur_word);
    }
}


void add_towel(char *p, Node *towels) {
    Node *new_node;
    int index, i;

    index = get_letter_index(*p);

    if (towels->children[index] == NULL) {
        new_node = (Node *)malloc(sizeof(Node));
        new_node->children = (Node **)malloc(sizeof(Node *) * ALPHABET);
        for (i = 0; i < ALPHABET; i++) {
            new_node->children[i] = NULL;
        }
        towels->children[index] = new_node;
    }

    if (*p != '\0') {
        add_towel(p+1, towels->children[index]);
    }
}

int longest_towel = 0;

#define max(a, b)       (a) > (b) ? (a) : (b)

void get_towels(void) {
    int len;
    char *p;

    len = getline(&line_buf, &max_line, f);

    line_buf[len-1] = '\0';

    longest_towel = max(len-1, longest_towel);

    p = strtok(line_buf, ", ");

    while (p != NULL) {
        printf("%s\n", p);
        add_towel(p, towels);
        p =  strtok(NULL, ", ");
    }

    getline(&line_buf, &max_line, f);

}

unsigned long count_total_possible_designs(int len) {
    unsigned long count;
    int i, j, diff;

    for (i = 0; i < MAXLINELEN; i++)
        for (j = 0; j < MAXLINELEN; j++)
            cache[i][j] = 0;

    for (diff = 0; diff <= len-1; diff++) {
        for (i = 0, j = i + diff; j <= len-1; i++, j++) {
            count_possible_designs(i, j, &cache[i][j], towels);
        }
    }

    return cache[0][len-1];
}

void count_possible_designs(int start, int finish, unsigned long *result, Node *tree) {
    int index = get_letter_index('\0');

    if (tree->children[index] != NULL) {
        if (start == finish+1) {
            (*result)++;
            return;
        } else {
            *(result) += cache[start][finish];
        }
    }

    index  = get_letter_index(line_buf[start]);

    if (tree->children[index] != NULL) {
        count_possible_designs(start + 1, finish, result, tree->children[index]);
    }
}

