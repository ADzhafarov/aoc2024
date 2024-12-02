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
    char *token;
    int buf[10];
    int direction;
    int prev, cur;
    int len, i;
    int violations[2];
    int violations_num = 0;
    int option1, option2;

    prev = -1;
    len = 0;
    for (token = strtok(line, " \t\n"); token != NULL; token = strtok(NULL, " \t\n")) {
        if (isdigit(*token)) {
            cur = atoi(token);
            if (prev != -1) {
                buf[len++] = cur - prev;
            }
            prev = cur;
        }
    }

    /*printf("\t\tDifs: ");*/
    /*for (i = 0; i < len; i++) */
    /*    printf("%d, ", buf[i]);*/
    /*printf("\n");*/


    for (direction = -1; direction <= 1; direction += 2) {
        violations_num = 0;
        for (i = 0; i < len; i++) {
            if (buf[i] * direction <= 0 || abs(buf[i]) > 3) {
                if (violations_num == 2) {
                    violations_num++;
                    break;
                }
                violations[violations_num++] = i;
            }
        }

        if (violations_num == 0)
            return 1;

        if (violations_num == 1) {
            if (violations[0] == 0 || violations[0] == len-1)
                return 1;
            else {
                option1 = buf[violations[0]] + buf[violations[0] + 1];
                option2 = buf[violations[0]] + buf[violations[0] - 1];

                if (option1 * direction > 0 && abs(option1) <= 3 ||
                    option2 * direction > 0 && abs(option2) <= 3 )
                    return 1;
            }
        } else if (violations_num == 2) {
            if (abs(violations[0] - violations[1]) == 1) {
                option1 = buf[violations[0]] + buf[violations[1]];
                if (option1 * direction > 0 && abs(option1) <= 3)
                    return 1;
            }
        }
    }

    return 0;
}











