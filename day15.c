#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIDE     50
#define MAXFILENAME 20

int side = 0;

typedef struct _coord {
    int x;
    int y;
} Coordinates;

void perform_movement(Coordinates *robot, Coordinates direction, char *rows[]);
unsigned long calc_gps(char *rows[]);

int main(int argc, char *argv[]) {
    char *rows[MAXSIDE] = {NULL};
    char *p, c;
    int rowlen, i;
    size_t max_rowlen = MAXSIDE + 1;
    Coordinates robot;
    unsigned long result;
    char filename[20];
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
    while (side < MAXSIDE && (rowlen = getline(&rows[side], &max_rowlen, f)) > 1) {
        printf("%s", rows[side]);
        p = strchr(rows[side], '@');
        if (p != NULL) {
            robot.y = side;
            robot.x = p - rows[side];
        }
        side++;
    }
    printf("\tSuccess: grid's size is %dx%d, robot starting point is (%d,%d)\n", side, side, robot.x, robot.y);

    printf("Simulating robot's movement...\n");
    while ((c = getc(f)) != EOF) {
        Coordinates direction;
        if (c == '\n') continue;

        switch (c) {
            case '^':
                direction.x = 0;
                direction.y = -1;
                break;
            case '>':
                direction.x = 1;
                direction.y = 0;
                break;
            case '<':
                direction.x = -1;
                direction.y = 0;
                break;
            case 'v':
                direction.x = 0;
                direction.y = 1;
                break;
        }

        perform_movement(&robot, direction, rows);
        /*printf("===== %c =====\n", c);*/
        /*for (i = 0; i < side; i++)*/
        /*    printf("%s", rows[i]);*/
    }
    printf("\tSuccess\n");

    printf("Aggregating GPS coordinates...\n");
    result = calc_gps(rows);
    printf("\tSuccess\n");

    printf("The resulting sum of GPS coordinates is\n"
           "************ %lu ************\n", result);

    exit(0);

}

unsigned long calc_gps(char *rows[]) {
    int i, j;
    unsigned long result = 0;

    for (i = 0; i < side; i++)
        for (j = 0; j < side; j++)
            if (rows[i][j] == 'O')
                result += 100 * i + j;

    return result;
}

Coordinates add_coords(Coordinates a, Coordinates b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

Coordinates sub_coords(Coordinates a, Coordinates b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}


void perform_movement(Coordinates *robot, Coordinates direction, char *rows[]) {
    Coordinates robot_dest;
    Coordinates next;

    next = add_coords(*robot, direction);
    robot_dest = next;

    while (rows[next.y][next.x] == 'O')
        next = add_coords(next, direction);

    if (rows[next.y][next.x] == '#')
        return;

    rows[robot->y][robot->x] = '.';
    rows[next.y][next.x] = 'O';
    rows[robot_dest.y][robot_dest.x] = '@';

    robot->x = robot_dest.x;
    robot->y = robot_dest.y;

    return;
}















