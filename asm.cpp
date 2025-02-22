#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "asm.h"

struct Command_Code
{
    const char *command;
    const int code;
};


int get_command(const char *str) {
    struct Command_Code command_array[] = {
        {"push",    PUSH},
        {"add",     ADD},
        {"sub",     SUB},
        {"out",     OUT},
        {"div",     DIV},
        {"mul",     MUL},
        {"pushr",   PUSHR},
        {"popr",    POPR},
        {"jmp",     JMP},
        {"jb",      JB},
        {"jbe",     JBE},
        {"ja",      JA},
        {"jae",     JAE},
        {"je",      JE},
        {"jne",     JNE},
        {"hlt",     HLT},
    };

    int command_count = sizeof(command_array) / sizeof(command_array[0]);
    for (int i = 0; i < command_count; i++) {
        if (strcmp(str, command_array[i].command) == 0)
            return command_array[i].code;
    }

    return -1;
}

int get_register_name(const char* str) {
    struct Command_Code reg_array[] = {
        {"rax", rax},
        {"rbx", rbx},
        {"rcx", rcx},
        {"rdx", rdx},
    };

    int reg_count = sizeof(reg_array) / sizeof(reg_array[0]);
    for (int i = 0; i < reg_count; i++) {
        if (strcmp(str, reg_array[i].command) == 0)
            return reg_array[i].code;
    }

    return 0;

}

int put_label(char *str, labels labels_array[], int code_size, int * put_label_error) {
        str[strlen(str) - 1] = '\0';
        int i = 0;
        while ((labels_array[i].ip) != -1) {
                i++;
                if (i >= LABELS_MAX_COUNT) {
                    fprintf(stderr, "labels_array is overflow");
                    *put_label_error = -1;
            }
        }

        strcpy(labels_array[i].value, str);
        labels_array[i].ip = code_size;

    return 0;
}

int put_code_size(FILE *f1, FILE *f2, labels labels_array[], int * put_label_error) {

    int code_size = 0;

    while(1) {

        char str[STR_LEN] = "";
        if (fscanf(f1, "%20s ", str) != 1) {
            fprintf(stderr, "string was not read");
            return -1;
        }

        if ((strchr(str, ':') - str) == strlen(str) - 1)
            put_label(str, labels_array, code_size, put_label_error);

            int command = get_command(str);

            switch(command) {
                case JMP:
                case JB:
                case JBE:
                case JA:
                case JAE:
                case JE:
                case JNE:
                case PUSH:
                case PUSHR:
                case POPR:
                {
                    code_size += 2;
                    break;
                }

                case ADD:
                case MUL:
                case DIV:
                case SUB:
                case OUT:
                {
                    code_size += 1;
                    break;
                }

                default:
                    break;
            }

            if (command == HLT) {
                code_size += 1;
                break;
            }

    }

    fseek(f1, 0, SEEK_SET);
    fprintf(f2, "%d\n", code_size);
    return code_size;
}


int main() {

    freopen("error.log", "w", stderr);

    FILE *f1 = fopen("input.txt", "r");
    if (f1 == NULL)
    {
        fprintf(stderr, "f1 == null");
        assert(0);
    }

    FILE *f2 = fopen("code.txt", "w");
    if (f2 == NULL)
    {
        fprintf(stderr, "f2 == null");
        assert(0);
    }

    struct labels labels_array[LABELS_MAX_COUNT];

    int put_label_error = 0;
    int put_code_size_error = put_code_size(f1, f2, labels_array, &put_label_error);
    if (put_code_size_error == -1)
        return -1;
    if (put_label_error == -1)
        return -1;
        for (int i = 0; i < LABELS_MAX_COUNT; i++ ) { // NOTE распечатка
            printf("%d ", labels_array[i].ip);
            printf("%s ", labels_array[i].value);
            printf("\n");
        }

    while(1)
    {
        const int STR_LEN = 20;
        char str[STR_LEN] = "";
        if (fscanf(f1, "%20s", str) != 1)
        {
            fprintf(stderr, "string was not read");
            fclose(f1);
            fclose(f2);
            return -1;
        }

        int command = get_command(str);

        switch (command)
            {
                case JMP:
                case JB:
                case JBE:
                case JA:
                case JAE:
                case JE:
                case JNE:
                {

                    fprintf(f2, "%d ", command);
                    char jmp_buffer[20] = "";
                    if(fscanf(f1, "%20s", jmp_buffer) != 1) {
                        fprintf(stderr, "string was not read");
                        fclose(f1);
                        fclose(f2);
                        return -1;
                    }

                    if (strchr(jmp_buffer, ':') == jmp_buffer) {
                        char label_value[20] = {'\0'};
                        strncpy(label_value, jmp_buffer + 1, strlen(jmp_buffer) - 1);

                        int i = 0;
                        while (strcmp(labels_array[i].value, label_value)) {
                            i++;
                            if (i >= LABELS_MAX_COUNT) {
                                fprintf(stderr, "labels_array is overflow");
                                fclose(f1);
                                fclose(f2);
                                return -1;
                            }
                        }
                        int value = labels_array[i].ip;
                        fprintf(f2, "%d\n", value);
                        break;
                    }
                    else
                    {
                        int value = 0;
                        if((value = strtol(jmp_buffer, NULL, 10)) == 0) {
                            fprintf(stderr, "JMP arguement should be a number or label");
                            fclose(f1);
                            fclose(f2);
                            return -1;
                        }

                        fprintf(f2, "%d\n", value);
                        break;
                    }
                }

                case PUSH:
                {

                    fprintf(f2, "%d ", command);
                    int value = 0;
                    if (fscanf(f1, "%d", &value) != 1) //
                    {
                        fprintf(stderr, "push arguement was not read");
                        fclose(f1);
                        fclose(f2);
                        return -1;
                    }

                    fprintf(f2, "%d\n", value);
                    break;

                }

                case ADD:
                case SUB:
                case OUT:
                case DIV:
                case MUL:
                {
                    fprintf(f2, "%d\n", command);
                    break;
                }

                case PUSHR:
                case POPR:
                {
                    fprintf(f2, "%d ", command);
                    char temp_str[STR_LEN] = "";
                    if (fscanf(f1, "%20s", temp_str) != 1)
                    {
                        fprintf(stderr, "register was not read");
                        fclose(f1);
                        fclose(f2);
                        return -1;
                    }
                    int reg = get_register_name(temp_str);
                    switch(reg) {
                        case 0:
                        {
                            fprintf(stderr, "unknown register");
                            fclose(f1);
                            fclose(f2);
                            return -1;
                        }
                        case rax:
                        case rbx:
                        case rcx:
                        case rdx:
                            {
                                fprintf(f2, "%d\n", reg);
                                break;
                            }
                    }
                }

                default:
                    break;
            }

        if (command == HLT)
        {
            fprintf(f2, "%d\n", command);
            break;
        }

    }

    fclose(stderr);
    fclose(f1);
    fclose(f2);
    return 0;
}
