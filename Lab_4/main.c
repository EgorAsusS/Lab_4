#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool compress(char* filepath, char* filepath_new);
bool decompress(char* filepath, char* filepath_new);

int main() {
    bool flag = true;
    flag = compress("data.txt", "encrypt.txt");
    if (flag){
        flag = decompress("encrypt.txt", "data_2.txt");
        if (flag){
            printf("Good compress and decompress\n");
        }
        else{
            printf("Error occured while decompressing file\n");
        }
    }
    else{
        printf("Error occured while compressing file\n");
    }
    return 0;
}

bool compress(char* filepath, char* filepath_new) {
    FILE* file = fopen(filepath, "r");
    FILE* file_new = NULL;
    if (file) {
        file_new = fopen(filepath_new, "w");
        char el[8] = { 0 };
        int ix = 0;
        char _el = 0;
        int flag = 0;
        unsigned char mask = 1;
        if (file_new) {
            while (flag != EOF) {
                mask = 1;
                ix = 0;
                _el = 0;
                while (ix < 8 && flag != EOF) {
                    flag = fscanf(file, "%c", &el[ix]);
                    if (flag != EOF) {
                        ix++;
                    }
                }
                if (ix < 8) {
                    for (int i = 0; i < ix; i++) {
                        if(fprintf(file_new, "%c", el[i]) < 0){
                            fclose(file_new);
                            fclose(file);
                            return 0;
                        }
                    }
                }
                else {
                    for (int i = 1, j = 7; i < 8; i++, j--) {
                        _el = ((mask & el[0]) << j) | el[i];
                        mask = mask << 1;
                        if(fprintf(file_new, "%c", _el) < 0){
                            fclose(file_new);
                            fclose(file);
                            return 0;
                        }
                    }
                }
            }
            fclose(file_new);
            fclose(file);
            return 1;
        }
    }
    return 0;
}

bool decompress(char* filepath, char* filepath_new) {
    FILE* file = fopen(filepath, "r");
    FILE* file_new = NULL;
    if (file) {
        file_new = fopen(filepath_new, "w");
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
                _el = 0;
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
                        if(fprintf(file_new, "%c", el[i]) < 0){
                            fclose(file_new);
                            fclose(file);
                            return 0;
                        }
                    }
                }
                else {
                    for (int i = 0, j = 7; i < 7; i++, j--) {
                        _el = ((mask & el[i]) >> j) | _el;
                        el[i] = el[i] & _mask;
                    }
                    if(fprintf(file_new, "%c", _el) < 0){
                        fclose(file_new);
                        fclose(file);
                        return 0;
                    }
                    for (int i = 0; i < 7; i++) {
                        if(fprintf(file_new, "%c", el[i]) < 0){
                            fclose(file_new);
                            fclose(file);
                            return 0;
                        }
                    }
                }
            }
            fclose(file_new);
            fclose(file);
            return 1;

        }
    }
    return 0;
}
