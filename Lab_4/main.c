#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

FILE* encode(FILE* file, char* file_new_name);
FILE* decode(FILE* file, char* file_new_name);

int main() {
    FILE* file = fopen("data.txt", "r");
    encode(file, "encrypt.txt");
    fclose(file);
    file = fopen("encrypt.txt", "r");
    decode(file, "data.txt");
    fclose(file);
    return 0;
}


FILE* encode(FILE* file, char* filepath) {
    FILE* file_new = NULL;
    if (file) {
        file_new = fopen(filepath, "w");
        char el[8] = { 0 };
        int ix = 0;
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
                    for (int i = 1, j = 7; i < 8; i++, j--) {
                        _el = ((mask & el[0]) << j) | el[i];
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

FILE* decode(FILE* file, char* filepath) {
    FILE* file_new = NULL;
    if (file) {
        file_new = fopen(filepath, "w");
        char el[7] = { 0 };
        int ix = 0;
        char _el = 0;
        bool flag_end = 1;
        int flag = 0;
        unsigned char mask = 0;
        unsigned char _mask = 0;
        if (file_new) {
            while (flag != EOF) {
                mask = 128;
                _mask = ~mask;
                ix = 0;
                flag_end = 1;
                while (ix < 7 && flag != EOF) {
                    flag = fscanf(file, "%c", &el[ix]);
                    if (flag != EOF) {
                        if (mask & el[ix]) {
                            flag_end = 0;
                        }
                        ix++;
                    }
                }
                if (ix < 7 || flag_end) {
                    for (int i = 0; i < ix; i++) {
                        fprintf(file_new, "%c", el[i]);
                    }
                }
                else {
                    for (int i = 0, j = 7; i < 7; i++, j--) {
                        _el = ((mask & el[i]) >> j) | _el;
                        el[i] = el[i] & _mask;
                    }
                    fprintf(file_new, "%c", _el);
                    for (int i = 0; i < 7; i++) {
                        fprintf(file_new, "%c", el[i]);
                    }
                }

            }
            fclose(file_new);
        }
    }
    return file_new;
}