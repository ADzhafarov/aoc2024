#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE     100
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
Robot calc_pos(Robot robot);

int seconds;

int main(int argc, char *argv[]) {
    char *line;
    size_t maxline = MAXLINE;
    int safety_factor;
    Robot robot;
    int robots;

    if (argc == 1)
        seconds = DEFAULTSEC;
    else
        seconds = atoi(argv[1]);

    // quadrants
    int quadrants[2][2] = {
        {0, 0},
        {0, 0},
    };

    robots = 0;
    while (getline(&line, &maxline, stdin) > 0) {
        robot = get_robot(line);
        robot = calc_pos(robot);
        if (robot.px == GRIDWIDTH / 2 || robot.py == GRIDHEIGHT / 2)
            continue;
        quadrants[robot.px > GRIDWIDTH / 2][robot.py > GRIDHEIGHT / 2]++;
        robots++;
    }

    printf("Total amount of robots: %d\n", robots);
    
    printf("Quadrants:\n");
    printf("\t---------\n");
    printf("\t|%3d|%3d|\n", quadrants[0][0], quadrants[1][0]);
    printf("\t---------\n");
    printf("\t|%3d|%3d|\n", quadrants[0][1], quadrants[1][1]);
    printf("\t---------\n");

    safety_factor = quadrants[0][0] * quadrants[0][1] * quadrants[1][0] * quadrants[1][1];
    printf("The safety factor is\n"
           "******* %d *******\n", safety_factor);
        
    return 0;
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

Robot calc_pos(Robot robot) {
    robot.px = (robot.px + robot.vx * seconds) % GRIDWIDTH;
    if (robot.px < 0) {
        robot.px += GRIDWIDTH;
    }

    robot.py = (robot.py + robot.vy * seconds) % GRIDHEIGHT;
    if (robot.py < 0)
        robot.py += GRIDHEIGHT;

    return robot;
}

void print_robot(Robot robot) {
    printf("\t%10s: (%d, %d)\t%10s: (%d, %d)\n", "Position", robot.px, robot.py, "Velocity", robot.vx, robot.vy);
}

