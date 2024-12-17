#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long Register;
typedef char Octal;

Register get_register(char *line, FILE *f);
int scan_program(char *prog, char *line, FILE *f);
void execute_program(char *prog, int len);

#define MAX_PROGRAM     20

size_t max_line = 50;
Register A, A_tmp, B, C;

Octal result[20];
int res_len = 0;
int len;

int verbose = 0;

int main(int argc, char **argv) {
    char filename[20], *line;
    Octal program[MAX_PROGRAM];
    FILE *f;
    int i;

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
    printf("Registers: %ld %ld %ld\n", A, B, C);

    getline(&line, &max_line, f);

    len = scan_program(program, line, f);

    printf("Program: ");
    for (i = 0; i < len; i++)
        printf("%d ", program[i]);
    printf("(%d)\n", len);
    

    int idx_neq;
    int cntr = 0;
    for (A_tmp = (long) pow(8, len-1);;) {
        A = A_tmp;
        res_len = 0;

        execute_program(program, len);

        idx_neq = len - 1;
        while (idx_neq >= 0 && result[idx_neq] == program[idx_neq])
            idx_neq--;

        if (idx_neq == -1) {
            printf("The answer is %lu\n", A_tmp);
            return 0;
        }

        A_tmp += (long) pow(8, idx_neq);
    }
}


void execute_command(Octal opcode, Octal operand, int *ip, Octal *prog) {
    int combo_operand;
    int out;

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
            A /= (int) pow(2, combo_operand);
            break;
        case 1:
            B ^= operand;
            break;
        case 2:
            B = combo_operand & 7;
            break;
        case 3:
            if (A)
                *ip = operand;
            else
                *ip += 2;
            break;
        case 4:
            B ^= C;
            break;
        case 5:
            out = combo_operand & 7;
            result[res_len++] = out;
            break;
        case 6:
            B = A / (int) pow(2, combo_operand);
            break;
        case 7:
            C = A / (int) pow(2, combo_operand);
            break;
    }

    if (opcode != 3)
        *ip += 2;
}

void execute_program(char *prog, int len) {
    int ip = 0;
    char c;

    while (ip < len) {
        execute_command(prog[ip], prog[ip+1], &ip, prog);
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






















