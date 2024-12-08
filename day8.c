#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWIDTH    100

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct _antennas_tree_node {
    char antenna;
    Coordinate *nodes;
    int nodes_num;
    struct _antennas_tree_node *left;
    struct _antennas_tree_node *right;
} AntennasTree;

typedef struct _antinodes_tree_node {
    Coordinate coord;
    struct _antinodes_tree_node *left;
    struct _antinodes_tree_node *right;
} AntinodesTree;

AntennasTree *add_antennas(AntennasTree *antennas, char *line, int line_num);
AntinodesTree *calculate_antinodes(AntennasTree *antennas, AntinodesTree *antinodes, int *count);
void free_antennas(AntennasTree *);
void free_antinodes(AntinodesTree *);
void print_antennas(AntennasTree *antennas);
void print_antinodes(AntinodesTree *antinodes);

int width = 0;
int height;

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    char *line = (char *)malloc(MAXWIDTH);
    size_t maxlen = MAXWIDTH;
    int unique_antinodes = 0;
    int line_num = 0;
    AntennasTree *antennas = NULL;
    AntinodesTree *antinodes = NULL;

    while (getline(&line, &maxlen, stdin) > 0) {
        antennas = add_antennas(antennas, line, line_num++);
        if (width == 0)
            width = strlen(line) - 1;
    }

    height = line_num;

    print_antennas(antennas);

    antinodes = calculate_antinodes(antennas, antinodes, &unique_antinodes);
    printf("%p\n", antinodes);
    print_antinodes(antinodes);

    printf("The amount of unique antinodes places is\n"
           "************** %d ***************\n", unique_antinodes);

    free(line);
    free_antennas(antennas);
    free_antinodes(antinodes);
}

Coordinate calc_antinode(Coordinate a, Coordinate b) {
    a.x = 2 * a.x - b.x;
    a.y = 2 * a.y - b.y;

    return a;
}

int within_boundaries(Coordinate a) {
    return 0 <= a.x && a.x < width && 0 <= a.y && a.y < height;
}

#define compare_coord(a, b)     (a.x == b.x ? a.y - b.y : a.x - b.x)

AntinodesTree *add_antinode(AntinodesTree *antinodes, Coordinate antinode, int *count) {
    AntinodesTree *new_node;
    int comp;

    if (antinodes == NULL) {
        new_node = (AntinodesTree *)malloc(sizeof(AntinodesTree));
        new_node->coord = antinode;
        new_node->left = NULL;
        new_node->right = NULL;
        (*count)++;
        antinodes = new_node;
    } else if ((comp = compare_coord(antinodes->coord, antinode)) < 0)
        antinodes->left = add_antinode(antinodes->left, antinode, count);
    else if (comp > 0)
        antinodes->right = add_antinode(antinodes->right, antinode, count);

    return antinodes;
}

Coordinate diff(Coordinate a, Coordinate b) {
    a.x = a.x - b.x;
    a.y = a.y - b.y;
    return a;
}

Coordinate sum(Coordinate a, Coordinate b) {
    a.x = a.x + b.x;
    a.y = a.y + b.y;
    return a;
}

AntinodesTree *calculate_antinodes(AntennasTree *antennas, AntinodesTree *antinodes, int *count) {
    Coordinate new_antinode;
    Coordinate delta;

    if (antennas == NULL)
        return antinodes;

    for (int i = 0; i < antennas->nodes_num; i++)
        for (int j = i + 1; j < antennas->nodes_num; j++) {
            delta = diff(antennas->nodes[i], antennas->nodes[j]);
            new_antinode = antennas->nodes[i];
            while (within_boundaries(new_antinode)) {
                antinodes = add_antinode(antinodes, new_antinode, count);
                new_antinode = sum(new_antinode, delta);
            }
            new_antinode = antennas->nodes[j];
            while (within_boundaries(new_antinode)) {
                antinodes = add_antinode(antinodes, new_antinode, count);
                new_antinode = diff(new_antinode, delta);
            }
        }
    
    antinodes = calculate_antinodes(antennas->left, antinodes, count);
    antinodes = calculate_antinodes(antennas->right, antinodes, count);

    return antinodes;
}

#define MAXANT      2000

AntennasTree *add_antenna(AntennasTree *antennas, char ant, Coordinate coord) {
    AntennasTree *new_node;
    int comp;

    if (antennas == NULL) {
        new_node = (AntennasTree *)malloc(sizeof(AntennasTree));
        new_node->nodes = (Coordinate *)malloc(sizeof(Coordinate) * MAXANT);
        new_node->nodes_num = 0;
        new_node->nodes[new_node->nodes_num++] = coord;
        new_node->antenna = ant;
        new_node->left = NULL;
        new_node->right = NULL;
        antennas = new_node;
    } else if (antennas->antenna < ant)
        antennas->left = add_antenna(antennas->left, ant, coord);
    else if (antennas->antenna > ant)
        antennas->right = add_antenna(antennas->right, ant, coord);
    else
        antennas->nodes[antennas->nodes_num++] = coord;

    return antennas;
}

void print_antennas(AntennasTree *antennas) {
    if (antennas == NULL)
        return;

    print_antennas(antennas->left);
    printf("%c: ", antennas->antenna);
    for (int i = 0; i < antennas->nodes_num; i++) {
        printf("(%d,%d), ", antennas->nodes[i].x, antennas->nodes[i].y);
    }
    printf("\n");
    print_antennas(antennas->right);
}

void print_antinodes(AntinodesTree *antinodes) {
    if (antinodes == NULL)
        return;

    print_antinodes(antinodes->left);
    printf("(%d,%d)\n", antinodes->coord.x, antinodes->coord.y);
    print_antinodes(antinodes->right);
}

AntennasTree *add_antennas(AntennasTree *antennas, char *line, int line_num) {
    int len = strlen(line); // bad
    int col;

    Coordinate antenna_coord;


    for (col = 0; col < len; col++) {
        if (line[col] != '.' && line[col] != '\n') {
            antenna_coord.x = col;
            antenna_coord.y = line_num;
            antennas = add_antenna(antennas, line[col], antenna_coord);
        }
    }

    return antennas;
}

void free_antennas(AntennasTree *tree) {
    if (tree == NULL)
        return;

    free_antennas(tree->left);
    free_antennas(tree->right);

    free(tree->nodes);
    free(tree);
}

void free_antinodes(AntinodesTree *tree) {
    if (tree == NULL)
        return;

    free_antinodes(tree->left);
    free_antinodes(tree->right);

    free(tree);
}
