#include <endian.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWIDTH    200
#define MAXHEIGHT   200
#define MAXFNLEN    20

int width;
int height = 0;

typedef struct {
    int x;
    int y;
} Coord;

typedef struct _node {
    Coord coord;
    struct _node *next;
} Node;

void traverse_maze(char *rows[], Coord current_position, Coord direction, Coord finish, long score);
int shortest_path_spots(void);

long min_score[MAXHEIGHT][MAXWIDTH] = {{-1}};

int main(int argc, char *argv[]) {
    char *rows[MAXHEIGHT] = {NULL};
    size_t max_rowlen = MAXWIDTH + 1;
    int rowlen;
    char *p;
    char filename[MAXFNLEN];
    Coord start, end, dir;
    FILE *f;

    printf("Analyzing command-line arguements...\n");
    if (argc == 1)
        sprintf(filename, "input.txt");
    else if (argv[1][0] == 't')
        sprintf(filename, "test-input%c.txt", argv[1][1]);
    else {
        printf("Usage: ./day15 [-t1,2]\n");
        exit(1);
    }
    printf("\tSuccess\n");

    printf("Opening %s file...\n", filename);
    f = fopen(filename, "r");
    if (f == NULL) {
        printf("\tCouldn't open %s\n", filename);
        exit(2);
    }
    printf("\tSuccess\n");

    printf("Reading input...\n");
    while (height < MAXHEIGHT && (rowlen = getline(&rows[height], &max_rowlen, f)) > 1) {
        printf("\t\t%s", rows[height]);

        if (p = strchr(rows[height], 'S'), p != NULL) {
            start.x = p - rows[height];
            start.y = height;
        }

        if (p = strchr(rows[height], 'E'), p != NULL) {
            end.x = p - rows[height];
            end.y = height;
        }

        height++;
    }
    width = strlen(rows[0]) - 1;
    printf("\tSuccess\n"
           "\t\tSize of grid: %dx%d\n"
           "\t\tStart point: (%d,%d)\n"
           "\t\tEnd point: (%d,%d)\n",
           height, width, start.x, start.y, end.x, end.y);
    
    for (int i = 0; i < height; i++) {  
        for (int j = 0; j < width; j++)
             min_score[i][j] = -1;
    }
    
    printf("Recursively traversing the maze... \n");
    dir.x = 1;
    dir.y = 0;
    traverse_maze(rows, start, dir, end, 0);
    printf("\tSuccess\n");

    printf("Freeing memory...\n");
    for (int i = 0; i < height; i++)
        free(rows[i]);
    printf("\tSuccess\n");


    /*for (int i = 0; i < height; i++) {  */
    /*    for (int j = 0; j < width; j++)*/
    /*         printf("%lu ", min_score[i][j]);*/
    /*    printf("\n");*/
    /*}*/

    printf("===== Winner's score: %lu =====\n", min_score[end.y][end.x]);
    printf("===== Good spots: %d =====\n", shortest_path_spots()); 

}


char visited[MAXHEIGHT][MAXWIDTH] = {{0}};
Node *current_path = NULL;

Coord rotate(Coord coord, int clockwise) {
    int tmp = coord.x;
    coord.x = coord.y * clockwise;
    coord.y = -tmp * clockwise;

    return coord;
}

void push(Coord coord) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->coord = coord;
    new_node->next = current_path;
    current_path = new_node;
}

void pop() {
    if (current_path == NULL)
        return;

    Node *old_head = current_path;
    current_path = current_path->next;
    free(old_head);
}

void print_path(Node *path) {
    if (path == NULL)
        return;

    if (path->next == NULL)
        printf("(%d,%d)\n", path->coord.y, path->coord.x);
    else
        printf("(%d,%d)->", path->coord.y, path->coord.x);

    print_path(path->next);
}

Coord sum(Coord a, Coord b) {
    a.x += b.x;
    a.y += b.y;

    return a;
}

#define r(i,j)      rows[i][j]
#define v(i,j)      visited[i][j]

char in_shortest_path[MAXHEIGHT][MAXWIDTH] = {{0}};

int shortest_path_spots(void) {
    int result, i, j;

    result = 0;
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
            if (in_shortest_path[i][j])
                result++;
    return result;
}


void add_to_shortest_path(Node *path) {
    if (path == NULL)
        return;

    in_shortest_path[path->coord.y][path->coord.x] = 1;
    add_to_shortest_path(path->next);
}

void traverse_maze(char *rows[], Coord current_position, Coord direction, Coord finish, long score) {

    push(current_position);
    
    long cur_min_score = min_score[current_position.y][current_position.x];

    if (cur_min_score != -1 && score > cur_min_score + 1000) {
        pop();
        return;
    }

    if (score < cur_min_score || cur_min_score == -1) {
        min_score[current_position.y][current_position.x] = score;
    }

    if (r(current_position.y, current_position.x) == 'E') {
        if (score < cur_min_score || cur_min_score == -1) {
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    in_shortest_path[i][j] = 0;
        }
        if (score == min_score[current_position.y][current_position.x]) {
            add_to_shortest_path(current_path);
            /*printf("\t%ld: ", score);*/
            /*print_path(current_path);*/
        }
        pop();
        return;
    }

    v(current_position.y, current_position.x) = 1;
    Coord next;
    Coord dirs[3] = {
        direction,
        rotate(direction, 1),
        rotate(direction, -1)
    };

    for (int i = 0; i < 3; i++) {
        next = sum(current_position, dirs[i]);
        if (r(next.y, next.x) != '#' && !v(next.y, next.x))
            traverse_maze(rows, next, dirs[i], finish, score + (i == 0 ? 1 : 1001));
    }

    v(current_position.y, current_position.x) = 0;
    pop();
}


