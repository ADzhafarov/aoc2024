#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int Register;
typedef char Octal;

Register get_register(char *line, FILE *f);
int scan_program(char *prog, char *line, FILE *f);
void execute_program(char *prog, int len);

#define MAX_PROGRAM     20

size_t max_line = 50;
Register A, B, C;

int main(int argc, char **argv) {
    char filename[20], *line;
    Octal program[MAX_PROGRAM];
    FILE *f;
    int i, len;

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

    line = (char *)malloc(max_line);

    A = get_register(line, f);
    B = get_register(line, f);
    C = get_register(line, f);
    printf("Registers: %d %d %d\n", A, B, C);

    getline(&line, &max_line, f);

    len = scan_program(program, line, f);

    printf("Program: ");
    for (i = 0; i < len; i++)
        printf("%d ", program[i]);
    printf("(%d)\n", len);
    
    execute_program(program, len);

}

int first = 1;
int verbose = 1;
Octal result[20];
int res_len = 0;

void execute_command(Octal opcode, Octal operand, int *ip) {
    int combo_operand;

    switch (operand) {
        case 0: case 1: case 2: case 3:
            combo_operand = operand;
            break;
        case 4:
            combo_operand = A;
            break;
        case 5:
            combo_operand = B;
            break;
        case 6:
            combo_operand = C;
            break;
    }
    

    switch (opcode) {
        case 0:
            if (verbose)
                printf("Executing adv with A=%d, operand=%d", A, combo_operand);
            A /= (int) pow(2, combo_operand);
            if (verbose)
                printf("\tResult: %d\n", A);
            break;
        case 1:
            if (verbose)
                printf("Executing bxl with B=%d, operand=%d", B, operand);
            B ^= operand;
            if (verbose)
                printf("\tResult: %d\n", B);
            break;
        case 2:
            if (verbose)
                printf("Executing bst with B=%d, operand=%d", B, combo_operand);
            B = combo_operand & 7;
            if (verbose)
                printf("\tResult: %d\n", B);
            break;
        case 3:
            if (verbose)
                printf("Executing jzn with operand=%d (A=%d)\n", operand, A);
            if (A)
                *ip = operand;
            else
                *ip += 2;
            break;
        case 4:
            if (verbose)
                printf("Executing bxc with B=%d, C=%d", B, C);
            B ^= C;
            if (verbose)
                printf("\tResult: %d\n", B);
            break;
        case 5:
            if (verbose)
                printf("Executing out with operand=%d\n", combo_operand);
            result[res_len++] = combo_operand & 7;
            if (verbose)
                printf("\tResult: \t\t\t\t\t\t%d\n", combo_operand & 7);
            first = 0;
            break;
        case 6:
            if (verbose)
                printf("Executing bdv with A=%d, operand=%d", A, combo_operand);
            B = A / (int) pow(2, combo_operand);
            if (verbose)
                printf("\tResult: %d\n", B);
            break;
        case 7:
            if (verbose)
                printf("Executing cdv with A=%d, operand=%d", A, combo_operand);
            C = A / (int) pow(2, combo_operand);
            if (verbose)
                printf("\tResult: %d\n", C);
            break;
    }


    if (verbose) {
        printf("\t\t\tA: %d\n", A);
        printf("\t\t\tB: %d\n", B);
        printf("\t\t\tC: %d\n", C);
    }

    if (opcode != 3)
        *ip += 2;
}

void execute_program(char *prog, int len) {
    int ip = 0;
    char c;

    while (ip < len) {
        execute_command(prog[ip], prog[ip+1], &ip);
    }
    printf("Program result: \n");
    for (int i = 0; i < res_len; i++){
        printf("%d%c", result[i], i == res_len - 1 ? '\n': ',');
    }
}

int scan_program(char *prog, char *line, FILE *f) {
    int len = 0;
    char c;

    while (c = getc(f), c != EOF)
        if ('0' <= c && c <= '7' && len < MAX_PROGRAM)
            prog[len++] = c-'0';

    return len;

}

Register get_register(char *line, FILE *f) {
    char *p;

    if (getline(&line, &max_line, f) <= 0) {
        printf("get_register: error collecting a line\n");
        exit(2);
    }

    p = strtok(line, ":");
    p = strtok(NULL, ":");

    if (p == NULL) {
        printf("get_register: error initializing register\n");
        exit(3);
    }

    return atoi(p);
}






















