#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXTOWELS       3000
#define MAXTOWEL        10


size_t max_line = 3000;
char **towels;
char *line_buf;
int towels_num = 0;
FILE *f;

void get_towels(void);
int is_design_possible(char *design);

int main(int argc, char *argv[]) {
    towels = (char **)malloc(sizeof(char *) * MAXTOWELS);
    int i;
    line_buf = (char *)malloc(max_line);

    for (i = 0; i < MAXTOWELS; i++) {
        towels[i] = (char *)malloc(MAXTOWEL);
    }

    printf("Opening the file...\n");
    if (argc == 1)
        f = fopen("input.txt", "r");
    else if (argv[1][0] == '-' && argv[1][1] == 't')
        f = fopen("test-input.txt", "r");
    else {
        printf("Usage: ./day19 [-t]\n");
        exit(1);
    }
    if (f == NULL) {
        printf("\tCouldn't open the file\n");
        exit(2);
    }
    printf("\tSuccess\n");

    printf("Getting towels\n");
    get_towels();
    for (int i = 0; i < towels_num; i++) 
        printf("%s\n", towels[i]);
    printf("\tSuccessfully scanned %d types of towels\n", towels_num);


    printf("Analysing patterns...\n");
    int result = 0;
    int len;
    while (len = getline(&line_buf, &max_line, f), len > 0) {
        line_buf[len-1] = '\0';
        result += is_design_possible(line_buf);
    }
    printf("\tSuccess\n");


    printf("The result is %d\n", result);
    
    printf("Freeing memory...\n");
    free(towels);
    free(line_buf);
    printf("\tSuccess\n");
}

int starts_with(char **string, char *prefix);

int is_design_possible(char *design) {
    char *p;

    if (*design == '\0') {
        return 1;
    }

    for (int i = 0; i < towels_num; i++) {
        p = design;

        if (starts_with(&p, towels[i]) && is_design_possible(p))
            return 1;
    }

    return 0;

}

void get_towels(void) {
    int len;
    char *p;

    len = getline(&line_buf, &max_line, f);

    line_buf[len-1] = '\0';

    p = strtok(line_buf, ", ");

    while (p != NULL) {
        strncpy(towels[towels_num++], p, MAXTOWEL);
        p =  strtok(NULL, ", ");
    }

    getline(&line_buf, &max_line, f);

}

int starts_with(char **string, char *prefix) {
    if (**string == '\n')
        return 1;

    while (**string != '\0' && **string == *prefix) {
        prefix++;
        (*string)++;
    }

    return *prefix == '\0';
}
