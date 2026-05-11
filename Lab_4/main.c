#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int compress(char* filepath, char* filepath_new);
int decompress(char* filepath, char* filepath_new);

int main() {
    // Тесты на нули
    //FILE* f = fopen("data.txt", "w");
    //for (size_t j = 0; j < 1; j++) {
    //    for (size_t i = 0; i < 8; i++) {
    //        fprintf(f, "%c", i);
    //    }
    //}
    //for (size_t i = 0; i < 7; i++) {
    //    fprintf(f, "%c", i);
    //}
    //fclose(f);
    //f = NULL;
    
    // Тесты на ASCII
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

    // Тест на неправильные данные в 1 символе
    //int flag = 0;
    //flag = decompress("compress.txt", "decompress.txt");
    //if (flag == 0) {
    //    printf("BAD\n");
    //}

    int flag = 0;
    flag = compress("data.txt", "compress.txt");
    if (flag == 1){
        flag = decompress("compress.txt", "decompress.txt");
        if (flag == 1){
            printf("Good compress and decompress\n");
        }
        else if (flag == -1) {
            printf("Error occured while closing file\n");
        }
        else if (flag == -2) {
            printf("Error occured while removing file\n");
        }
        else if (flag == -3) {
            printf("Error occured because input data is invalid\n");
        }
        else {
            printf("Error occured while decompressing file\n");
        }
        
    }
    else if (flag == -1) {
        printf("Error occured while closing file\n");
    }
    else if (flag == -2) {
        printf("Error occured while removing file\n");
    }
    else if (flag == -3) {
        printf("Error occured because input data is invalid\n");
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
            int flag = 1;
            int status = 1;
            char flag_tail = '0';
            unsigned char mask = 1;
            if (file_new) {
                if (fprintf(file_new, "%c", flag_tail) < 0) {
                    flag = -2;
                }
                while (flag > 0) {
                    mask = 1;
                    ix = 0;
                    _el = 0;
                    while (ix < 8 && flag > 0) { // Считываем символы по 8 штук (или меньше)
                        flag = fscanf(file, "%c", &el[ix]);
                        if (flag > 0) {
                            if ((unsigned char)el[ix] > 127) {
                                flag = -3;
                                continue;
                            }
                            ix++;
                        }
                    }
                    if (ix && flag) { // Сжимаем и записываем или просто записываем (если есть хвост (количество символов в конце в файла меньшее 8))
                        if (ix < 8) {
                            flag_tail = '1';
                            for (int i = 0; i < ix; i++) {
                                if (fprintf(file_new, "%c", el[i]) < 1) {
                                    flag = -2;
                                    i = ix;
                                }
                            }
                        }
                        else {
                            for (int i = 1, j = 7; i < 8; i++, j--) {
                                _el = ((mask & el[0]) << j) | el[i];
                                mask = mask << 1;
                                if (fprintf(file_new, "%c", _el) < 1) {
                                    flag = -2;
                                    i = ix;
                                }
                            }
                        }
                    }
                }
                if (flag_tail == '1' && flag >= -1) { // Записывем в файл flag_tail
                    if (fseek(file_new, 0, SEEK_SET)) {
                        flag = -2;
                    }
                    else if (fprintf(file_new, "%c", flag_tail) < 1) {
                        flag = -2;
                    }
                }
                if (fclose(file_new) || fclose(file)) {
                    status = -1;
                }
                if (status != -1) {
                    if (flag == -3) {
                        status = -3;
                    }
                }
                if (flag == -2 || flag == -3 || flag == 0) {
                    if (remove(filepath_new)) {
                        if (status == -3) {
                            return status;
                        }
                        else {
                            return -2;
                        }
                    }
                    return 0;
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
            int flag = 1;
            int status = 1;
            unsigned char mask = 0;
            unsigned char _mask = 0;
            if (file_new) {
                flag = fscanf(file, "%c", &flag_tail);
                if (flag_tail != '1' && flag_tail != '0') {
                    flag = -3;
                }
                while (flag > 0) {
                    mask = 128;
                    _mask = ~mask;
                    ix = 0;
                    flag_end = 1;
                    _el = 0;
                    while (ix < 7 && flag > 0) {
                        flag = fscanf(file, "%c", &el[ix]);
                        if (flag != EOF) {
                            if (mask & el[ix]) {
                                flag_end = 0;
                            }
                            ix++;
                        }
                    }
                    if (ix && flag) {
                        if ((flag_end && flag_tail == '1') || ix < 7) { // Претензия на конец
                            if (ix < 7 && flag_end == 0) {
                                flag = -3;
                                continue;
                            }
                            flag = fscanf(file, "%c", &_el);
                            if (fseek(file, -1, SEEK_CUR)) {
                                flag = -2;
                            }
                            _el = 0;
                            if (flag == EOF) { // Если конец
                                for (int i = 0; i < ix; i++) {
                                    if (fprintf(file_new, "%c", el[i]) < 1) {
                                        flag = -2;
                                        i = ix;
                                    }
                                }
                                continue;
                            }
                        }
                        for (int i = 0, j = 7; i < 7; i++, j--) { // Декодируем
                            _el = ((mask & el[i]) >> j) | _el;
                            el[i] = el[i] & _mask;
                        }
                        if (fprintf(file_new, "%c", _el) < 1) { // Записываем декодированный символ
                            flag = -2;
                        }
                        if (flag != -2) {
                            for (int i = 0; i < 7; i++) {
                                if (fprintf(file_new, "%c", el[i]) < 1) { // Записываем остальные декодированне символы
                                    flag = -2;
                                    i = 7;
                                }
                            }
                        }
                    }
                }
                if (fclose(file_new) || fclose(file)) {
                    status = -1;
                }
                if (status != -1) {
                    if (flag == -3) {
                        status = -3;
                    }
                }
                if (flag == -2 || flag == -3 || flag == 0) {
                    if (remove(filepath_new)) {
                        if (status == -3) {
                            return status;
                        }
                        else {
                            return -2;
                        }
                    }
                    return 0;
                }
                return status;
            }
        }
    }
    return 0;
}