#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int side;
int nanosec;

char **rows;
char *line_buf;
int **min_path;
size_t max_line = 6;
char filename[20]; 

typedef struct {
    int x;
    int y;
} Coord;

typedef struct _lnode {
    Coord coord;
    struct _lnode *prev;
} Node;


Node *cur_min_path = NULL;
Coord start, finish;

void calculate_min_path(Coord position, int path_len);
Coord add_byte(char *line_buf, FILE *f);
int on_path(Coord byte, Node *path);
void reinitialize(Node **path);

int main(int argc, char **argv) {
    setbuf(stdout, NULL);
    int i, j;
    int x, y;
    FILE *f;

    start.x = 1;
    start.y = 1;


    printf("Analyzing command-line arguements...\n");
    if (argc == 1) {
        side = 73;
        nanosec = 1024;
        finish.x = 71;
        finish.y = 71;
        sprintf(filename, "input.txt");
    }
    else if (argv[1][0] == '-' && argv[1][1] == 't') {
        side = 9;
        nanosec = 12;
        finish.x = 7;
        finish.y = 7;
        sprintf(filename, "test-input.txt");
    } else {
        printf("Usage: ./day15 [-t]\n");
        exit(1);
    }
    printf("\tSuccess\n");

    printf("Initializing space...\n");
    rows = (char **)malloc(sizeof(char *) * side);
    min_path = (int **)malloc(sizeof(int *) * side);

    for (i = 0; i < side; i++) {
        rows[i] = (char *)malloc(side);
        min_path[i] = (int *)malloc(side * sizeof(int));
        for (j = 0; j < side; j++) {
            min_path[i][j] = -1;
            if (i == 0 || i == side - 1 || j == 0 || j == side - 1)
                rows[i][j] = '#';
            else
                rows[i][j] = '.';
        }
    }
    printf("\tSuccess\n");

    printf("Opening %s file...\n", filename);
    f = fopen(filename, "r");
    if (f == NULL) {
        printf("\tCouldn't open %s\n", filename);
        exit(2);
    }
    printf("\tSuccess\n");

    printf("Getting input...\n");
    line_buf = (char *)malloc(max_line);
    for (i = 0; i < nanosec; i++) {
        add_byte(line_buf, f);
    }
    printf("\tSuccess\n");

    printf("Grid after %d nanoseconds:\n", nanosec);
    for (i = 0; i < side; i++) {
        for (j = 0; j < side; j++)
            printf("%c", rows[i][j]);
        printf("\n");
    }

    int first = 1;
    Coord new_byte;
    do {
        if (!first) {
            new_byte = add_byte(line_buf, f);
            if (!on_path(new_byte, cur_min_path))
                continue;
        }

        reinitialize(&cur_min_path);
        for (i = 0; i < side; i++)
            for (j = 0; j < side; j++)
                min_path[i][j] = -1;
        calculate_min_path(start, 0);
        if (first) {
            printf("The min_path takes %d steps\n", min_path[finish.y][finish.x]); 
            first = 0;
        }
    } while (min_path[finish.y][finish.x] != -1);

    printf("The first byte to block the road is %d,%d\n", new_byte.x - 1, new_byte.y - 1);

    printf("The min_path takes %d steps\n", min_path[finish.y][finish.x]); 

    printf("Freeing memory...\n");
    for (i = 0; i < side; i++) {
        free(min_path[i]);
        free(rows[i]);
    }
    free(min_path);
    free(rows);
    free(line_buf);
    printf("\tSuccess\n");
    
}

int on_path(Coord byte, Node *path) {
    if (path == NULL)
        return 0;

    if (path->coord.x == byte.x && path->coord.y == byte.y)
        return 1;

    return on_path(byte, path->prev);
}

Coord add_byte(char *line_buf, FILE *f) {
    Coord byte;
    getline(&line_buf, &max_line, f);
    byte.x = atoi(strtok(line_buf, ",")) + 1;
    byte.y = atoi(strtok(NULL, ",")) + 1;
    rows[byte.y][byte.x] = '#';
    return byte;
}

Node *add_step(Node *path, Coord coord) {
    Node *new_node;
    new_node = (Node *)malloc(sizeof(Node));
    new_node->coord = coord;
    new_node->prev = path;
    return new_node;
}

Node *remove_step(Node *path) {
    Node *tmp = path->prev;
    free(path);
    return tmp;
}

void copy_path(Node *from, Node **to) {
    reinitialize(to);
    
    while (from != NULL) {
        *to = add_step(*to, from->coord);
        from = from->prev;
    }
}

void reinitialize(Node **path) {
    Node *tmp;

    while (*path != NULL) {
        tmp = *path;
        *path = (*path)->prev;
        free(tmp);
    }
}

Coord rotate_clockwise(Coord direction) {
    int tmp = direction.x;
    direction.x = direction.y;
    direction.y = -tmp;

    return direction;
}

Coord add(Coord a, Coord b) {
    a.x += b.x;
    a.y += b.y;

    return a;
}

#define free_path(a)    (rows[a.y][a.x] != '#')

Node *cur_path = NULL;

void calculate_min_path(Coord position, int path_len) {

    /*printf("(%d, %d)\n", position.y, position.x);*/
    /*for (int i = 0; i <= side; i++) {*/
    /*    for (int j = 0; j <= side; j++)*/
    /*        if (i == 0 || j == 0 || i == side || j == side)*/
    /*            printf("#");*/
    /*        else*/
    /*            printf("%c", min_path[i][j] == -1 ? ' ' : '*');*/
    /*    printf("\n");*/
    /*}*/

    /*char c = getchar();*/
    /*if (c == EOF)*/
    /*    exit(0);*/
    
    Coord direction, next;
    int i;

    if (path_len < min_path[position.y][position.x] || min_path[position.y][position.x] == -1) {
        min_path[position.y][position.x] = path_len;
        if (finish.y == position.y && finish.x == position.x)
            copy_path(cur_path, &cur_min_path);
    } else
        return;

    cur_path = add_step(cur_path, position);

    direction.x = 1;
    direction.y = 0;
    for (i = 0; i < 4; i++) {
        direction = rotate_clockwise(direction);
        next = add(position, direction);
        if (free_path(next))
            calculate_min_path(next, path_len + 1);
    }

    cur_path = remove_step(cur_path);
}



















