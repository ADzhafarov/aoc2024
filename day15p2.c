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

Coordinates perform_movement(Coordinates robot, Coordinates direction, char *rows[]);
unsigned long calc_gps(char *rows[]);

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    char *rows[MAXSIDE] = {NULL};
    char *linebuf = NULL;
    char *p, c;
    int new_rowlen;
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
    while (side < MAXSIDE && (rowlen = getline(&linebuf, &max_rowlen, f)) > 1) {
        rows[side] = (char *)malloc((rowlen - 1) * 2);
        new_rowlen = 0;
        for (i = 0; i < rowlen-1; i++) {
            switch (linebuf[i]) {
                case '#':
                    rows[side][new_rowlen++] = '#';
                    rows[side][new_rowlen++] = '#';
                    break;
                case 'O':
                    rows[side][new_rowlen++] = '[';
                    rows[side][new_rowlen++] = ']';
                    break;
                case '.':
                    rows[side][new_rowlen++] = '.';
                    rows[side][new_rowlen++] = '.';
                    break;
                case '@':
                    rows[side][new_rowlen++] = '@';
                    rows[side][new_rowlen++] = '.';
                    break;
            }
            rows[side][new_rowlen] = '\0';
        }
        printf("\t\t%s\n", rows[side]);
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

        robot = perform_movement(robot, direction, rows);
        /*printf("===== %c =====\n", c);*/
        /*for (i = 0; i < side; i++)*/
        /*    printf("%s\n", rows[i]);*/
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
        for (j = 0; j < side * 2; j++)
            if (rows[i][j] == '[')
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

int pushable(Coordinates pusher, Coordinates direction, char *rows[]) {
    Coordinates dest = add_coords(pusher, direction);
    char next = rows[dest.y][dest.x];
    Coordinates pair, pair_dir;

    if (next == '.')
        return 1;

    if (next == '#')
        return 0;

    if (next == '[' || next == ']') {
        if (direction.y == 0)
            return pushable(dest, direction, rows);
        
        pair_dir.x = next == '[' ? 1 : -1;
        pair_dir.y = 0;
        pair = add_coords(dest, pair_dir);

        return pushable(dest, direction, rows) && pushable(pair, direction, rows);
    }

    return 0;
}

void push(Coordinates pusher, Coordinates direction, char *rows[]) {
    Coordinates dest = add_coords(pusher, direction);
    char next = rows[dest.y][dest.x];
    Coordinates pair, pair_dir;

    if (next == '#')
        return;

    if (next == '[' || next == ']') {
        if (direction.y == 0)
            push(dest, direction, rows);
        else {
            pair_dir.x = next == '[' ? 1 : -1;
            pair_dir.y = 0;
            pair = add_coords(dest, pair_dir);

            push(dest, direction, rows);
            push(pair, direction, rows);
        }
    }

    rows[dest.y][dest.x] = rows[pusher.y][pusher.x];
    rows[pusher.y][pusher.x] = '.';
}


Coordinates perform_movement(Coordinates robot, Coordinates direction, char *rows[]) {
    if (pushable(robot, direction, rows)) {
        push(robot, direction, rows) ;
        return add_coords(robot, direction);
    } else
        return robot;
}

