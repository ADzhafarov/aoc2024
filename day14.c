#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE     100
#define STARTSKIP   4
#define SKIP        101
#define MAXROBOTS   500
#define GRIDWIDTH   101
#define GRIDHEIGHT  103
#define DEFAULTSEC  100

typedef struct _bot {
    int px;
    int py;
    int vx;
    int vy;
} Robot;

Robot get_robot(char *line);
void print_robot(Robot robot);
Robot calc_step(Robot robot, int skip);
void print_grid(Robot *robots, int robots_num);

int seconds;

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    int i, j;
    char *line, c;
    size_t maxline = MAXLINE;
    int safety_factor;
    Robot robot;
    Robot *robots;
    int robots_num;

    if (argc == 1)
        seconds = DEFAULTSEC;
    else
        seconds = atoi(argv[1]);

    // quadrants
    int quadrants[2][2] = {
        {0, 0},
        {0, 0},
    };

    FILE *f = fopen("input.txt", "r");
    if (f == NULL) {
        printf("Can't open input.txt\n");
        exit(1);
    } else {
        printf("input.txt opened successfully\n");
    }

    printf("Reading input... \n");
    robots = (Robot *)malloc(MAXROBOTS * sizeof(Robot));
    robots_num = 0;
    while (getline(&line, &maxline, f) > 0) {
        robots[robots_num++] = get_robot(line);
        print_robot(robots[robots_num-1]);
    }
    printf("\tSuccess\n");


    for (j = 0; j < robots_num; j++) {
        robots[j] = calc_step(robots[j], STARTSKIP);
    }

    i = STARTSKIP;
    while ((c = getchar()) == '\n') {
        printf("After %d seconds:\n", i);
        print_grid(robots, robots_num);
        for (j = 0; j < robots_num; j++) {
            robots[j] = calc_step(robots[j], SKIP);
        }
        i += SKIP;

        printf("Press Enter to skip 101 seconds or Ctrl+D to finish\n");

    }
        
    free(robots);
    return 0;
}

void print_grid(Robot *robots, int robots_num) {
    int i, j, k;
    int found;

    for (i = 0; i < GRIDHEIGHT; i++) {
        for (j = 0; j < GRIDWIDTH; j++) {
            found = 0;
            for (k = 0; k < robots_num; k++)
                if (robots[k].px == j && robots[k].py == i) {
                    found = 1;
                    break;
                }
            if (found)
                printf("#");
            else
                printf(" ");
        }
        printf("\n");
    }
}

Robot get_robot(char *line) {
    char *p;
    Robot robot;

    p = strtok(line, ",=");
    robot.px = atoi(strtok(NULL, ",="));
    robot.py = atoi(strtok(NULL, ",="));
    robot.vx = atoi(strtok(NULL, ",="));
    robot.vy = atoi(strtok(NULL, ",="));
    
    return robot;
}

Robot calc_step(Robot robot, int skip) {
    robot.px = (robot.px + robot.vx * skip) % GRIDWIDTH;
    if (robot.px < 0) {
        robot.px += GRIDWIDTH;
    }

    robot.py = (robot.py + robot.vy * skip) % GRIDHEIGHT;
    if (robot.py < 0)
        robot.py += GRIDHEIGHT;

    return robot;
}

void print_robot(Robot robot) {
    printf("\t%10s: (%d, %d)\t%10s: (%d, %d)\n", "Position", robot.px, robot.py, "Velocity", robot.vx, robot.vy);
}

