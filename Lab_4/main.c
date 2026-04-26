#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

FILE* encode(FILE* file, char* file_new_name);

int main() {
    FILE* file = fopen("data.txt", "r");
    encode(file, "encrypt.txt");
    fclose(file);
    return 0;
}


FILE* encode(FILE* file, char* file_new_name) {
    FILE* file_new = NULL;
    if (file) {
        file_new = fopen(file_new_name, "w");
        char el[8] = { 0 };
        int ix = 0;
        unsigned char ___________________el = 0;
        char _el = 0;
        int flag = 0;
        unsigned char mask = 1;
        if (file_new) {
            while (flag != EOF) {
                mask = 1;
                ix = 0;
                while (ix < 8 && flag != EOF) {
                    flag = fscanf(file, "%c", &el[ix]);
                    if (flag != EOF) {
                        ix++;
                    }
                }
                if (ix < 8) {
                    for (int i = 0; i < ix; i++) {
                        fprintf(file_new, "%c", el[i]);
                    }
                }
                else {
                    for (int i = 1, j = 7; i < 8 && flag != EOF; i++, j--) {
                        _el = ((mask & el[0]) << j) | el[i];
                        ___________________el = _el;
                        mask = mask << 1;
                        fprintf(file_new, "%c", _el);
                    }
                }

            }
            fclose(file_new);
        }
    }
    return file_new;
}