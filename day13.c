#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _2vect {
    long x;
    long y;
} Vect;

int read_machine(char *linebuf, Vect *a, Vect *b, Vect *prize);
long calc_cost(Vect a, Vect b, Vect prize);

#define MAXLINE     30

int part;

int main(int argc, char *argv[]) {
    Vect a, b, prize;
    int next = 1;
    long cost = 0;
    char *linebuf = (char *)malloc(MAXLINE);

    if (argc == 1)
        part = 1;
    else
        part = atoi(argv[1]);

    if (part != 1 && part != 2) {
        printf("Usage: ./day13 [1-2]\n");
        exit(1);
    }

    while (next) {
        next = read_machine(linebuf, &a, &b, &prize);
        printf("A=(%ld,%ld), B=(%ld,%ld), prize=(%ld,%ld)\n", a.x, a.y, b.x, b.y, prize.x, prize.y);
        cost += calc_cost(a, b, prize);
    }

    free(linebuf);

    printf("Total cost: %ld\n", cost);
}

size_t maxline = MAXLINE;

typedef struct _2mat {
    long m11;
    long m12;
    long m21;
    long m22;
} Mat;

Mat get_matrix(Vect a, Vect b);
Mat reverse_matrix_without_division(Mat matrix);

#define det(m)      ((m).m11 * (m).m22 - (m).m12 * (m).m21)

Vect MxV(Mat m, Vect v);

#define     ACOST   3
#define     BCOST   1

long calc_cost(Vect a, Vect b, Vect prize) {
    Mat matrix = get_matrix(a, b);
    Vect result_amplified;
    int determinant = det(matrix);
    if (determinant == 0) {
        printf("calc_cost: collinear A and B");
        return 0; // Not a correct answer, just hope this doesn't occur
    }
    matrix = reverse_matrix_without_division(matrix);
    result_amplified = MxV(matrix, prize);
    if (result_amplified.x % determinant || result_amplified.y % determinant)
        return 0;

    return result_amplified.x * ACOST / determinant + result_amplified.y * BCOST / determinant;
}

Vect MxV(Mat m, Vect v) {
    Vect new_vect;
    new_vect.x = m.m11 * v.x + m.m12 * v.y;
    new_vect.y = m.m21 * v.x + m.m22 * v.y;
    return new_vect;
}

Mat reverse_matrix_without_division(Mat matrix) {
    Mat reverse;

    reverse.m11 = matrix.m22;
    reverse.m12 = -matrix.m21;
    reverse.m21 = -matrix.m12;
    reverse.m22 = matrix.m11;

    return reverse;
}

Mat get_matrix(Vect a, Vect b) {
    Mat matrix;
    matrix.m11 = a.x;
    matrix.m12 = a.y;
    matrix.m21 = b.x;
    matrix.m22 = b.y;
    return matrix;
}

void read_coords(const char *name, const char *delimiters, Vect *coord, char *linebuf, long add);

int read_machine(char *linebuf, Vect *a, Vect *b, Vect *prize) {

    read_coords("button A", "+,", a, linebuf, 0);
    read_coords("button B", "+,", b, linebuf, 0);
    read_coords("Prize", "=,", prize, linebuf, part == 2 ? 10000000000000L : 0);

    if (getline(&linebuf, &maxline, stdin) > 0)
        return 1;

    return 0;
}

void read_coords(const char *name, const char *delimiters, Vect *coord, char *linebuf, long add) {
    char *x, *y;
    if (getline(&linebuf, &maxline, stdin) > 0) {
        strtok(linebuf, delimiters);
        x = strtok(NULL, delimiters);
        strtok(NULL, delimiters);
        y = strtok(NULL, delimiters);
        coord->x = atoi(x) + add;
        coord->y = atoi(y) + add;
    } else
        printf("read_coords: error reading %s", name);
}

