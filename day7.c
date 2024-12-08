#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE         250
#define MAXOPERANDS     40

int valid_calibration(int *operands, int operands_num, long target);

int main(void) {
    char *line = (char *)malloc(MAXLINE);
    char *p;
    size_t maxline = MAXLINE;
    long target;
    int operands[MAXOPERANDS];
    int operands_num = 0;
    int i;
    unsigned long long result = 0;
    int line_num = 1;

    printf("Lines: ");
    while (getline(&line, &maxline, stdin) > 0) {
        operands_num = 0;
        target = atol(strtok(line, ":"));
        while ((p = strtok(NULL, " ")) != NULL) {
            operands[operands_num++] = atoi(p);
        }

        if (valid_calibration(operands, operands_num, target)) {
            result += target;
            printf("%d ", line_num);
        }

        line_num++;
    }
    printf("\n");

    printf("The result is\n"
           "******************** %llu ********************\n", result);
    
    free(line);
}

int valid_calibration(int *operands, int operands_num, long target) {
    int last;

    if (operands_num == 0)
        return target == 0;

    if (target < 0)
        return 0;

    if (target == 0)
        return operands_num == 0;

    last = operands[operands_num - 1];
    
    if (operands_num == 1)
        return last == target;

    // Checking for * option
    if (last != 0 && target % last == 0 && valid_calibration(operands, operands_num - 1, target / last))
        return 1;

    // Checking for + option
    if (valid_calibration(operands, operands_num - 1, target - last))
        return 1;

    // Checking for || option as out last resort
    int order = 1;
    while (order <= last)
        order *= 10;
    if (((target - last) % order == 0) && valid_calibration(operands, operands_num - 1, target / order))
        return 1;

    return 0;
}

