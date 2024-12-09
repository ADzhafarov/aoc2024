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
    unsigned long position = 0;

    int start_num;
    int finish_num;

    printf("len = %d, start state = %d, finish_state = %d, finish_id = %d\n", len, start_state, finish_state, finish_id);

    unsigned long long checksum = 0;

    while (start <= finish) {
        if (start_state == FILLED) {
            start_num = *start - '0';
            checksum += (2 * position + start_num - 1) * start_num * start_id / 2;
            start_state = EMPTY;
            position += start_num;
            start++;
            start_id ++;
        } else if (start_state == EMPTY && finish_state == FILLED) {
            start_num = *start - '0';
            finish_num = *finish - '0';
            if (start_num < finish_num) {
                checksum += (2 * position + start_num - 1) * start_num * finish_id / 2;
                finish_num -= start_num;
                *finish = finish_num + '0';
                position += start_num;
                start++;
                start_state = FILLED;
            } else if (start_num > finish_num) {
                checksum += (2 * position + finish_num - 1) * finish_num * finish_id / 2;
                start_num -= finish_num;
                *start = start_num + '0';
                position += finish_num;
                finish--;
                finish_id--;
                finish_state = EMPTY;
            } else {
                checksum += (2 * position + finish_num - 1) * finish_num * finish_id / 2;
                position += finish_num;
                start++;
                finish--;
                finish_id--;
                start_state = FILLED;
                finish_state = EMPTY;
            }
        } else {
            finish--;
            finish_state = FILLED;
        }
    }

    printf("The checksum is: %llu\n", checksum);

    free(input);
}
