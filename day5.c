#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE     200
#define MAXPAGES    100

void add_restriction(char *line);
int check_line(char *line, char *mid);

char restrictions[MAXPAGES][MAXPAGES];

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    char *line;
    size_t maxline = MAXLINE;
    unsigned long correct = 0, corrected = 0;
    char mid;
    int i, j;

    printf("Initializing restrictions array... ");
    for (i = 0; i < MAXPAGES; i++)
        for (j = 0; j < MAXPAGES; j++)
            restrictions[i][j] = 0;
    printf("Success\n");

    printf("Allocating line buffer memory... ");
    line = (char *)malloc(MAXLINE);
    printf("Success\n");

    printf("Scaning restrictions... ");
    while (getline(&line, &maxline, stdin) > 0 && *line != '\n')
        add_restriction(line);
    printf("Success\n");

    printf("Checking page orderings... ");
    while (getline(&line, &maxline, stdin) > 0)
        if (check_line(line, &mid))
            correct += mid;
        else
            corrected += mid;
    printf("Success\n");

    printf("Freeing memory... ");
    free(line);
    printf("Success\n");

    printf("The part 1 answer is\n"
           "*********************** %lu ***********************\n", correct);

    printf("The part 2 answer is\n"
           "*********************** %lu ***********************\n", corrected);

    exit(0);
}

void add_restriction(char *line) {
    char left = atoi(strtok(line, "| \n"));
    char right = atoi(strtok(NULL, "| \n"));

    restrictions[left][right] = 1;
}

#define MAXUPDATE   30

int compare(const void *page1, const void *page2) {
    if (restrictions[*(int *)page1][*(int *)page2])
        return -1;
    else
        return 1;
}

int check_line(char *line, char *mid) {
    int update[MAXUPDATE];
    int i, j, len = 0;
    char *p;

    for (p = strtok(line, ","); p != NULL; p = strtok(NULL, ","))
        update[len++] = atoi(p);

    for (i = 0; i < len; i++)
        printf("%d ", update[i]);
    printf("|");

    for (i = 0; i < len; i++)
        for (j = i + 1; j < len; j++)
            if (restrictions[update[j]][update[i]]) {
                printf("\tSorting... ");
                qsort((void *)update, len, sizeof(int), compare);
                printf("Success\n");
                *mid = update[len / 2];
                return 0;
            }

    *mid = update[len / 2];
    return 1;
}






