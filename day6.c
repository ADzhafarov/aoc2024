#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT      140
#define WIDTH       140

typedef struct _coords {
    int i;
    int j;
} Coordinates;

typedef struct _tnode {
    Coordinates value;
    struct _tnode *left;
    struct _tnode *right;
} TNode;

int compare_coords(const void *a, const void *b) {
    Coordinates *p1 = (Coordinates *)a;
    Coordinates *p2 = (Coordinates *)b;

    if (p1->i == p2->i)
        return p1->j - p2->j;
    else
        return p1->i - p2->i;
}

void turn_direction(Coordinates *direction) {
    int tmp = direction->i;
    direction->i = direction->j;
    direction->j = -tmp;
}

TNode *add_coordinate(TNode *visited, Coordinates coord, int *new);
void free_tree(TNode *tree);
Coordinates sum_coord(Coordinates x, Coordinates y);
int is_obstacle(Coordinates coord, char *rows[]);
int is_outside(Coordinates coord, int height, int width);
void print_coord_tree(TNode *visited);
int check_for_loop_option(
    Coordinates current,
    TNode *visited,
    Coordinates direction,
    char *rows[],
    int height,
    int width,
    Coordinates start
);
Coordinates step(Coordinates current_coord, Coordinates *direction, int height, int width, char *rows[], int *full_turn);


int main(void) {
    char *rows[HEIGHT] = {NULL};
    size_t maxrowlen = WIDTH;
    int i = 0, j, line_len;
    Coordinates current_coord, direction, next_coord, prev_coord, start;
    TNode *visited;
    int num_visited = 0, count_unique = 0, loop_options = 0;
    int full_turn = 0;
    int new;
    char symbol;
    int loop_possible;
    int real_width, real_height;

    printf("Reading lines of input... ");
    while (i < HEIGHT && (line_len = getline(rows + i++, &maxrowlen, stdin)) > 0)
        for (j = 0; j < line_len; j++) {
            symbol = rows[i-1][j];
            if (symbol == '<' || symbol == '^' || symbol == '>' || symbol == 'v' || symbol == 'V') {
                current_coord.i = i-1;
                current_coord.j = j;
                start.i = i-1;
                start.j = j;
                direction.i = symbol == '^' ? -1 : (symbol == 'v' || symbol == 'V' ? 1 : 0);
                direction.j = symbol == '<' ? -1 : (symbol == '>' ? 1 : 0);
            }
        }
    printf("Success\n");

    real_height = i - 1;
    real_width = strlen(rows[0]) - 1;

    printf("Width: %d\nHeight: %d\n", real_width, real_height);

    printf("start: (%d, %d)\n", current_coord.i, current_coord.j);
    printf("direction: (%d, %d)\n", direction.i, direction.j);

    visited = NULL;

    printf("Simulating guards path...\n");
    prev_coord.i = -1;
    prev_coord.j = -1;
    while (1) {
        visited = add_coordinate(visited, current_coord, &new);
        /*printf("\tGuard at (%d, %d) [%s]\n", current_coord.i, current_coord.j, new ? "new" : "old");*/
        count_unique += new;

        printf("\t\tCheking for loop possibility... ");
        loop_possible = check_for_loop_option(current_coord, visited, direction, rows, real_height, real_width, start);
        loop_options += loop_possible;
        printf("%s", loop_possible ? "Possible\n" : "Impossible\n");

        current_coord = step(current_coord, &direction, real_height, real_width, rows, &full_turn);

        if (is_outside(current_coord, real_height, real_width))
            break;

        prev_coord = current_coord;
    }
    printf("Success\n");

    printf("****** The guard visited %d distinct points ******\n", count_unique);
    printf("****** There are %d options to loop the guard ******\n", loop_options);

    free_tree(visited);
    while (--i >= 0)
        free(rows[i]);
}

Coordinates step(Coordinates current_coord, Coordinates *direction, int height, int width, char *rows[], int *full_turn) {
    Coordinates next_coord = sum_coord(current_coord, *direction);
    if (is_outside(next_coord, height, width))
        return next_coord;
    if (is_obstacle(next_coord, rows)) {
        turn_direction(direction);
        next_coord = sum_coord(current_coord, *direction);
        if (is_obstacle(next_coord, rows)) {
            turn_direction(direction);
            next_coord = sum_coord(current_coord, *direction);
            *full_turn = 1;
            if (is_obstacle(next_coord, rows)) {
                turn_direction(direction);
                next_coord = sum_coord(current_coord, *direction);
                *full_turn = 0;
            }
        } else
            *full_turn = 0;
    } else
        *full_turn = 0;

    return next_coord;

}

int find_in_tree(TNode *visited, Coordinates coord);

int check_for_loop_option(Coordinates current, TNode *visited, Coordinates direction, char *rows[], int height, int width, Coordinates start) {
    TNode *loop_visited = NULL;
    Coordinates new_obstacle = sum_coord(current, direction);
    Coordinates fast, slow;
    Coordinates fast_dir, slow_dir;
    int full_turn, i;
    int verbose = 0;

    /*if (new_obstacle.i == 11 && new_obstacle.j == 12) {*/
    /*    verbose = 1;*/
    /*}*/

    fast_dir = slow_dir = direction;
    fast = slow = current;

    if (is_outside(new_obstacle, height, width))
        return 0;

    if (is_obstacle(new_obstacle, rows)) {
        turn_direction(&direction);
        new_obstacle = sum_coord(current, direction);
    }

    if (compare_coords(&new_obstacle, &start) == 0)
        return 0;

    if (find_in_tree(visited, new_obstacle))
        return 0;

    rows[new_obstacle.i][new_obstacle.j] = '#';

    while (1) {
        slow = step(slow, &slow_dir, height, width, rows, &full_turn);
        for (i = 0; i < 2; i++) {
            fast = step(fast, &fast_dir, height, width, rows, &full_turn);
            if (is_outside(fast, height, width)) {
                rows[new_obstacle.i][new_obstacle.j] = '.';
                return 0;
            }
        }
        if (verbose)
            printf("\t\t fast: (%d, %d), slow: (%d, %d)\n", fast.i, fast.j, slow.i, slow.j);
        if (compare_coords(&slow, &fast) == 0 && compare_coords(&slow_dir, &fast_dir) == 0) {
            rows[new_obstacle.i][new_obstacle.j] = '.';
            printf(" (%d, %d) ", new_obstacle.i, new_obstacle.j);

            return 1;
        }
    }
}

void print_coord_tree(TNode *visited) {
    if (visited == NULL)
        return;

    print_coord_tree(visited->left);
    printf("(%d, %d)\n", visited->value.i, visited->value.j);
    print_coord_tree(visited->right);
}

TNode *add_coordinate(TNode *visited, Coordinates coord, int *new) {
    TNode *new_node;
    int comp;

    if (visited == NULL) {
        new_node = (TNode *)malloc(sizeof(TNode));
        new_node->value = coord;
        new_node->left = NULL;
        new_node->right = NULL;
        *new = 1;
        return new_node;
    } else if ((comp = compare_coords(&visited->value, &coord)) < 0)
        visited->left = add_coordinate(visited->left, coord, new);
    else if (comp > 0)
        visited->right = add_coordinate(visited->right, coord, new);
    else {
        *new = 0;
    }

    return visited;
}

int find_in_tree(TNode *visited, Coordinates coord) {
    int comp;

    if (visited == NULL)
        return 0;
    else if ((comp = compare_coords(&visited->value, &coord)) < 0)
        return find_in_tree(visited->left, coord);
    else if (comp > 0)
        return find_in_tree(visited->right, coord);
    else
        return 1;
}

void free_tree(TNode *tree) {
    if (tree == NULL)
        return;
    free_tree(tree->left);
    free_tree(tree->right);

    free(tree);
}

int is_obstacle(Coordinates coord, char *rows[]) {
    return rows[coord.i][coord.j] == '#';
}

int is_outside(Coordinates coord, int height, int width) {
    return coord.i < 0 || coord.j < 0 || coord.i >= height || coord.j >= width;
}

Coordinates sum_coord(Coordinates x, Coordinates y) {
    x.i += y.i;
    x.j += y.j;
    return x;
}
