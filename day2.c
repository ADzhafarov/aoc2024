#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define FILENAME    "input.txt"
#define MAXLINE     40

int is_safe(char *line);

int main(int argc, char *argv[]) {
    FILE *fp;
    int len, safe_count, is_line_safe;
    char *filename;
    char *line;

    if (argc == 1) {
        filename = FILENAME;
        printf("Default filename is used: %s\n", filename);
    } else {
        filename = argv[1];
        printf("Filename derived from command-line arguements: %s\n", filename);
    }

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("main: can't open file %s\n", filename);
        exit(1);
    } else
        printf("File %s opened successfully\n", filename);
    

    printf("Reading columns from input...\n");
    printf("\tAllocating line buffer... ");
    line = (char *)malloc(MAXLINE);
    if (line == NULL) {
        printf("main: can't allocate memory for line buffer\n");
        exit(2);
    } else {
        printf("Success\n");
    }

    printf("\tAnalyzing input lines...\n");
    printf("\t\tSafe lines: ");
    while (fgets(line, MAXLINE, fp)) {
        is_line_safe = is_safe(line);
        safe_count += is_line_safe;
        len++;
        if (is_line_safe)
            printf("%d ", len);
    }
    printf("\n");
    
    free(line);

    printf("\tRead %d lines\n", len);
    printf("The amount of safe lines is\n"
           "**************************** %d ****************************\n", safe_count);

    exit(0);
}

int is_safe(char *line) {
    int direction = 0;
    int previous = -1, current;

    char *token;
    for (token = strtok(line, " \t\n"); token != NULL; token = strtok(NULL, " \t\n")) {
        if (isdigit(*token)) {
            current = atoi(token);

            // if previous is -1 then it's the first element and we just skip to the next
            if (previous == -1) {
                previous = current;
                continue;
            }

            // if previous is not -1 but direction is 0 then it's the second element
            // define direction, check it's value and move on
            if (direction == 0) {
                direction = current - previous;
                if (0 == direction || abs(direction) > 3)
                    return 0;
                previous = current;
                continue;
            }

            // if this is == 0 then current == previous and we return 0
            // if this is < 0 then the sign of direction differs from the sign of cur - prev
            // and sequence is not monotonous
            if ((current - previous) * direction <= 0 || abs(current - previous) > 3)
                return 0;

            previous = current;
        }
    }
    return 1;
}











