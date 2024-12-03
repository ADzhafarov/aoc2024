#include <regex.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXTOK      100
#define MUL_OP      "mul"
#define DO_OP       "do"
#define DONT_OP     "don't"

int get_tok(char *token);
enum token_type {WORD=-3, LEFTP='(', RIGHTP=')', COMMA=',', NUM=-2}; // Explicit negative values to not collide with printable symbols
enum state {W4_MUL, W4_LEFTPAR, W4_FIRSTOP, W4_COMMA, W4_SECONDOP, W4_RIGHTPAR}; // State shows what are we waiting for;
enum operators {MUL = 'm', DO = 'd', DONT = 'n'};

int main(void) {
    int tok_type, state, tok_len, first_op, second_op, count = 0;
    unsigned long res1 = 0;
    unsigned long res2 = 0;
    char token[MAXTOK];
    int turn_on = 1;
    int operator;

    state = W4_MUL;
    while((tok_type = get_tok(token)) != EOF) {
        tok_len = strlen(token);
        if (tok_type == WORD) {
            if (tok_len >= 2 && strcmp(token + tok_len - 2, DO_OP) == 0) {
                state = W4_LEFTPAR;
                operator = DO;
            } else if (tok_len >= 3 && strcmp(token + tok_len - 3, MUL_OP) == 0) {
                state = W4_LEFTPAR;
                operator = MUL;
            } else if (tok_len >= 5 && strcmp(token + tok_len - 5, DONT_OP) == 0) {
                state = W4_LEFTPAR;
                operator = DONT;
            }
            else
                state = W4_MUL;
        } else if (tok_type == LEFTP) {
            if (state == W4_LEFTPAR) {
                if (operator == MUL)
                    state = W4_FIRSTOP;
                else if (operator == DO || operator == DONT)
                    state = W4_RIGHTPAR;
                else
                    state = W4_MUL;
            } else
                state = W4_MUL;
        } else if (tok_type == NUM) {
            if (tok_len > 3)
                state = W4_MUL;

            if (state == W4_FIRSTOP) {
                first_op = atoi(token);
                state = W4_COMMA;
            } else if (state == W4_SECONDOP) {
                second_op = atoi(token);
                state = W4_RIGHTPAR;
            } else {
                state = W4_MUL;
            }

        } else if (tok_type == COMMA) {
            if (state == W4_COMMA)
                state = W4_SECONDOP;
            else
                state = W4_MUL;
        } else if (tok_type == RIGHTP) {
            if (state == W4_RIGHTPAR) {
                if (operator == MUL) {
                    count++;
                    res1 += first_op * second_op;
                    res2 += first_op * second_op * turn_on;
                } else if (operator == DO) {
                    turn_on = 1;
                } else if (operator == DONT) {
                    turn_on = 0;
                }
            }
            state = W4_MUL;
        } else {
            state = W4_MUL;
        }
    }

    printf("Found %d valid pairs, the result is: %lu for the first task and %lu for the second task", count, res1, res2);
    exit(0);
}

int get_tok(char *token) {
    *token = getchar();

    if (!isalnum(*token)) {
        *(token + 1) = '\0';
        return *token;
    }

    if (isalpha(*token)) {
        while (isalpha(*++token = getchar()) || *token == '\'')
            ;
        ungetc(*token, stdin);
        *token = '\0';
        return WORD;
    }

    while (isdigit(*++token = getchar()))
        ;
    ungetc(*token, stdin);
    *token = '\0';
    return NUM;
    
}

