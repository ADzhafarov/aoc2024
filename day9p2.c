#include <stdio.h>
#include <stdlib.h>

enum state {EMPTY, FILLED};

int main(void) {
    char *input = NULL;
    size_t max_input = 20000;

    int len = getline(&input, &max_input, stdin) - 1;

    int start_state = FILLED;
    int finish_state = len % 2 ? FILLED : EMPTY;

    char *start, *finish;

    start = input;
    finish = input + len - 1;
    int start_id = 0;
    int finish_id = (len - 1) / 2;
    unsigned long position;

    int start_num;
    int finish_num;
    unsigned long *slots_pos = (unsigned long *)malloc((len - 1) * sizeof(unsigned long));
    int i;

    position = 0;
    for (i = 0; i < len - 1; i++) {
        slots_pos[i] = position;
        position += input[i] - '0';
    }

    /*printf("positions: ");*/
    /*for (i = 0; i < (len-1) / 2; i++) {*/
    /*    printf("%d ", slots_pos[i]);*/
    /*}*/
    /*printf("\n");*/


    unsigned long long checksum = 0;

    int empty;
    int value;
    int num;
    int diff;
    int id;
    unsigned long addval;

    for (value = len - 1; value >= 0; value -= 2) {
        id = value / 2;
        diff = -1;
        num = input[value] - '0';
        printf("Id %d ", id);
        for (empty = 1; empty < value; empty += 2) {
            diff = (input[empty] - '0') - (input[value] - '0');
            if (diff >= 0) {
                printf("moves to %d-%d\n", slots_pos[empty], slots_pos[empty] + num - 1);
                addval = (2 * slots_pos[empty] + num - 1) * num * id / 2;
                checksum += addval;
                slots_pos[empty] += num;
                input[empty] -= num;
                break;
            }
        }
        if (diff < 0) {
            addval = (2 * slots_pos[value] + num - 1) * num * id / 2;
            checksum += addval;
            printf("stays at %d-%d\n", slots_pos[value], slots_pos[value] + num - 1);
        }
        if (addval < 0)
            printf("Overflow");
    }

    printf("The checksum is: %llu\n", checksum);

    free(slots_pos);
    free(input);
}
