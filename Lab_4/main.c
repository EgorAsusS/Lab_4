#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int compress(char* filepath, char* filepath_new);
int decompress(char* filepath, char* filepath_new);

int main() {
    //FILE* f = fopen("data.txt", "w");
    //for (size_t j = 0; j < 10; j++) {
    //    for (size_t i = 0; i < 8; i++) {
    //        fprintf(f, "%c", i);
    //    }
    //}
    //for (size_t i = 1; i < 8; i++) {
    //    fprintf(f, "%c", i);
    //}
    //fclose(f);
    //f = NULL;
    
    //FILE* f = fopen("data.txt", "w");
    //for (size_t i = 0; i < 128; i++) {
    //for (size_t i = 1; i < 128; i++) {
    //    if (i == 26) {
    //        continue;
    //    }
    //    fprintf(f, "%c", i);
    //}
    //fclose(f);
    //f = NULL;

    bool flag = true;
    flag = compress("data.txt", "compress.txt");
    if (flag == 1){
        flag = decompress("compress.txt", "decompress.txt");
        if (flag == 1){
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

int compress(char* filepath, char* filepath_new) {
    if (filepath && filepath_new) {
        FILE* file = fopen(filepath, "r");
        FILE* file_new = NULL;
        if (file) {
            file_new = fopen(filepath_new, "w");
            char el[8] = { 0 };
            int ix = 0;
            char _el = 0;
            int flag = 0;
            int status = 1;
            char flag_tail = '0';
            unsigned char mask = 1;
            if (file_new) {
                if (fprintf(file_new, "%c", flag_tail) < 0) {
                    flag = -2;
                }
                while (flag >= 0) {
                    mask = 1;
                    ix = 0;
                    _el = 0;
                    while (ix < 8 && flag >= 0) {
                        flag = fscanf(file, "%c", &el[ix]);
                        if (flag != EOF) {
                            if ((unsigned char)el[ix] > 127) {
                                flag = -2;
                                continue;
                            }
                            ix++;
                        }
                    }
                    if (ix < 8) {
                        flag_tail = '1';
                        for (int i = 0; i < ix; i++) {
                            if (fprintf(file_new, "%c", el[i]) < 0) {
                                flag = -2;
                                i = ix;
                            }
                        }
                    }
                    else {
                        for (int i = 1, j = 7; i < 8; i++, j--) {
                            _el = ((mask & el[0]) << j) | el[i];
                            mask = mask << 1;
                            if (fprintf(file_new, "%c", _el) < 0) {
                                flag = -2;
                                i = ix;
                            }
                        }
                    }
                }
                if (flag_tail == '1') {
                    if (fseek(file_new, 0, SEEK_SET)) {
                        flag = -2;
                    }
                    else if (fprintf(file_new, "%c", flag_tail) < 0) {
                        flag = -2;
                    }
                }
                if (fclose(file_new)) {
                    status = -1;
                }
                if (flag == -2) {
                    if (remove(filepath_new)) {
                        return -2;
                    }
                    return 0;
                }
                if (fclose(file)) {
                    status = -1;
                }
                return status;
            }
        }
    }
    return 0;
}

int decompress(char* filepath, char* filepath_new) {
    if (filepath && filepath_new) {
        FILE* file = fopen(filepath, "r");
        FILE* file_new = NULL;
        if (file) {
            file_new = fopen(filepath_new, "w");
            char el[7] = { 0 };
            int ix = 0;
            char _el = 0;
            char flag_end = 1;
            char flag_tail = '0';
            int flag = 0;
            int status = 1;
            unsigned char mask = 0;
            unsigned char _mask = 0;
            if (file_new) {
                flag = fscanf(file, "%c", &flag_tail);
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
                    if (flag != EOF || ix < 7) {
                        if (flag_end && flag_tail == '1') {
                            flag = fscanf(file, "%c", &_el);
                            if (fseek(file, -1, SEEK_CUR)) {
                                flag = -2;
                            }
                            _el = 0;
                            if (flag == EOF) {
                                for (int i = 0; i < ix; i++) {
                                    if (fprintf(file_new, "%c", el[i]) < 0) {
                                        flag = -2;
                                        i = ix;
                                    }
                                }
                                continue;
                            }
                        }
                        for (int i = 0, j = 7; i < 7; i++, j--) {
                            _el = ((mask & el[i]) >> j) | _el;
                            el[i] = el[i] & _mask;
                        }
                        if (fprintf(file_new, "%c", _el) < 0) {
                            flag = -2;
                        }
                        if (flag != -2) {
                            for (int i = 0; i < 7; i++) {
                                if (fprintf(file_new, "%c", el[i]) < 0) {
                                    flag = -2;
                                    i = 7;
                                }
                            }
                        }
                    }
                }
                if (fclose(file_new)) {
                    status = -1;
                }
                if (flag == -2) {
                    if (remove(filepath_new)) {
                        return -2;
                    }
                    return 0;
                }
                if (fclose(file)) {
                    status = -1;
                }
                return status;
            }
        }
    }
    return 0;
}