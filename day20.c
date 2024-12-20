#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Coord;

char filename[20]; 
FILE *f;
char *line_buf;
size_t max_line = 150;
int line_len;
int side = 0;
int **grid;
Coord start;
int cheats[101];

void simulate_run_without_cheats(Coord position, int picosecs);
void simulate_run_with_cheats(void);

int main(int argc, char **argv) {
    int i, j;

    line_buf = (char *)malloc(max_line);
    for (i = 0; i < 101; i++) {
        cheats[i] = 0;
    }

    printf("Analyzing command-line arguements...\n");
    if (argc == 1) {
        sprintf(filename, "input.txt");
    }
    else if (argv[1][0] == '-' && argv[1][1] == 't') {
        sprintf(filename, "test-input.txt");
    } else {
        printf("Usage: ./day15 [-t]\n");
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


    printf("Scanning input...\n");
    i = 0;
    while (line_len = getline(&line_buf, &max_line, f), line_len > 0) {
        if (side == 0) {
            side = line_len - 1;
            grid = (int **)malloc(side * sizeof(int *));
            for (j = 0; j < side; j++) {
                grid[j] = (int *)calloc(side, sizeof(int));
            }
        }

        for (j = 0; j < side; j++) {
            if (line_buf[j] == '#')
                grid[i][j] = -1;
            if (line_buf[j] == 'S') {
                start.x = j;
                start.y = i;
            }
        }

        i++;
    }
    printf("\tSuccess\n");

    printf("Simulating run without cheats\n");
    simulate_run_without_cheats(start, 0);
    printf("\tSuccess\n");

    simulate_run_with_cheats();

    for (int i = 0; i < 100; i++) {
        if (cheats[i] != 0) {
            printf("Saved %d picoseconds: %d\n", i, cheats[i]);
        }
    }
    if (cheats[100] != 0)
        printf("Saved 100+ picoseconds: %d\n", cheats[100]);

    printf("Freeing memory...\n");
    for (i = 0; i < side; i++) {
        free(grid[i]);
    }
    free(grid);
    free(line_buf);
    fclose(f);
    printf("\tSuccess\n");

}

Coord add(Coord a, Coord b);
Coord rotate(Coord dir);

#define within(a)       ((0 <= a.x) && (a.x < side) && (0 <= a.y) && (a.y < side))

void simulate_run_without_cheats(Coord position, int picosecs) {
    Coord next, dir;

    grid[position.y][position.x] = picosecs;
    
    dir.x = 1;
    dir.y = 0;

    for (int i = 0; i < 4; i++) {
        dir = rotate(dir);
        next = add(position, dir);

        if (within(next) && grid[next.y][next.x] == 0 && (next.y != start.y || next.x != start.x))
            simulate_run_without_cheats(next, picosecs + 1);
    }
}

#define MAXCHEATS   20 // for part 1 set it to 2

void check_cheats(Coord position) {
    Coord next, dir;
    int time_saved;
    int i, j, diff_y, diff_x;

    dir.x = 2;
    dir.y = 0;

    for (i = position.y - MAXCHEATS; i <= position.y + MAXCHEATS; i++)
        if (0 <= i && i < side) {
            diff_y = abs(i - position.y);
            for (j = position.x - (MAXCHEATS - diff_y); j <= position.x + (MAXCHEATS - diff_y); j++)
                if (0 <= j && j < side) {
                    diff_x = abs(j - position.x);
                    next.x = j;
                    next.y = i;
                    if (grid[next.y][next.x] > grid[position.y][position.x] + diff_x + diff_y) {
                        time_saved = grid[next.y][next.x] - grid[position.y][position.x] - diff_x - diff_y;
                        cheats[time_saved >= 100 ? 100 : time_saved]++;
                    }
                }

        }
}

void simulate_run_with_cheats(void) {
    Coord position;
    for (int i = 0; i < side; i++)
        for (int j = 0; j < side; j++)
            if (grid[i][j] != -1) {
                position.x = j;
                position.y = i;
                check_cheats(position);
            }
}

Coord add(Coord a, Coord b) {
    a.x += b.x;
    a.y += b.y;

    return a;
}
Coord rotate(Coord dir) {
    int tmp = dir.x;
    dir.x = dir.y;
    dir.y = -tmp;

    return dir;
}

