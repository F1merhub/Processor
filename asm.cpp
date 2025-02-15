// NOTE НЕ ЗАБЫТЬ СДЕЛАТЬ ПРОВЕРКИ НА АРГУМЕНТ У ФУНКЦИЙ и в проце тоже


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "asm.h"

int get_command(const char* str) {

        if (stricmp(str, "Push") == 0)
            return PUSH;
        if (stricmp(str, "Add") == 0)
            return ADD;
        if (stricmp(str, "Sub") == 0)
            return SUB;
        if (stricmp(str, "Out") == 0)
            return OUT;
        if (stricmp(str, "Div") == 0)
            return DIV;
        if (stricmp(str, "Mul") == 0)
            return MUL;

        if (stricmp(str, "Pushr") == 0)
            return PUSHR;
        if (stricmp(str, "Popr") == 0)
            return POPR;

        if (stricmp(str, "Jmp") == 0)
            return JMP;
        if (stricmp(str, "Jb") == 0)
            return JB;
        if (stricmp(str, "Jbe") == 0)
            return JBE;
        if (stricmp(str, "Ja") == 0)
            return JA;
        if (stricmp(str, "Jae") == 0)
            return JAE;
        if (stricmp(str, "Je") == 0)
            return JE;
        if (stricmp(str, "Jne") == 0)
            return JNE;

        if (stricmp(str, "Hlt") == 0)
            return HLT;

        return 0;
    }

int get_register_name(const char* str) {
    if (stricmp(str, "rax") == 0)
        return rax;
    if (stricmp(str, "rbx") == 0)
        return rbx;
    if (stricmp(str, "rcx") == 0)
        return rcx;
    if (stricmp(str, "rdx") == 0)
        return rdx;

    return 0;
}

const int LABELS_MAX_COUNT = 20;
const int LABELS_MAX_NAME = 20;

struct labels
{
    char value[20] = {'\0'};  // NOTE как лучше объявить?
    int ip = -1;
};

// int put_label(char *str, labels labels_array[], int code_size) { // TODO сделать изменения в случае переполнения массива структур
//         str[strlen(str) - 1] = '\0';// TODO проверить нормально ли записались имена меток
//         int i = 0;
//         while ((labels_array[i].ip) != -1) {
//                 i++;
//                 if (i >= LABELS_MAX_COUNT) {
//                     printf("labels_array is overflow");
//                     assert(0);
//             }
//         }
//
//         strcpy(labels_array[i].value, str);
//         labels_array[i].ip = code_size;
//
//     return 0;
// }

int put_code_size(FILE *f1, FILE *f2) {

    int code_size = 0;

    while(1) {

        const int STR_LEN = 20;

        char str[STR_LEN] = "";

        if (fscanf(f1, "%s ", str) != 1) {
            printf("smth went wrong");
            fclose(f1);
            fclose(f2);
            assert(0);
        }

        // if (strchr(str, ':'))
        //     put_label(str, labels_array, code_size);


            int command = get_command(str);

            switch(command) { // TODO Когда проходимся, надо записать метки
                case 0: // TODO поменять на unknown
                    break;
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

    FILE *f1 = fopen("input.txt", "r");
    if (f1 == NULL)
    {
        printf("f1 == null");
        fclose(f1);
        assert(0);
    }

    FILE *f2 = fopen("code.txt", "w");
    if (f2 == NULL)
    {
        printf("f2 == null");
        fclose(f1);
        fclose(f2);
        assert(0);
    }

//     struct labels labels_array[LABELS_MAX_COUNT];
//
//     put_code_size(f1, f2, labels_array);
//         for (int i = 0; i < LABELS_MAX_COUNT; i++ ) {
//             printf("%d ", labels_array[i].ip);
//             printf("%s ", labels_array[i].value);
//         }

    put_code_size(f1, f2);

    while(1)
    {
        const int STR_LEN = 10;
        char str[STR_LEN] = "";
        if (fscanf(f1, "%s", str) != 1) // NOTE как сделать лучше проверку?
        {
            printf("smth went wrong");
            fclose(f1);
            fclose(f2);
            assert(0);
            break;
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

                    fprintf(f2, "%d ", command);  // TODO проверки на остальные команды
                    int value = 0;
                    if (fscanf(f1, "%d", &value) != 1) // NOTE как сделать лучше проверку?
                    {
                        printf("argument should be a number!");
                        fclose(f1);
                        fclose(f2);
                        assert(0);
                    }

                    fprintf(f2, "%d\n", value);
                    break;

                }
//                 {
//
//                     fprintf(f2, "%d ", command);  // TODO проверки на остальные команды
//                     char jmp_buffer[20] = "";
//                     if(fscanf(f1, "%s", jmp_buffer) != 1) {
//                         printf("smth went wrong");
//                         fclose(f1);
//                         fclose(f2);
//                         assert(0);
//                     }
//
//                     if (strchr(jmp_buffer, ':')) { // переходим в массив по имени
//                         jmp_buffer[strlen(jmp_buffer) - 1] = '\0';
//                         int i = 0;
//                         while (strcmp(labels_array[i].value, jmp_buffer)) {
//                             i++;
//                             if (i >= LABELS_MAX_COUNT) {
//                                 printf("labels_array is overflow");
//                                 assert(0);
//                             }
//                         }
//                         int value = labels_array[i].ip;
//                         fprintf(f2, "%d\n", value);
//                         break;
//                     }
//                     else
//                     {
//                         int value = 0;
//                         if((value = strtol(jmp_buffer, NULL, 10)) == 0) {
//                             printf("JMP arguement should be a number or label");
//                             fclose(f1);
//                             fclose(f2);
//                             assert(0);
//                         }
//
//                         fprintf(f2, "%d\n", value);
//                         break;
//                     }
//                 }

                case PUSH:
                {

                    fprintf(f2, "%d ", command);  // TODO проверки на остальные команды
                    int value = 0;
                    if (fscanf(f1, "%d", &value) != 1) // NOTE как сделать лучше проверку?
                    {
                        printf("argument should be a number!");
                        fclose(f1);
                        fclose(f2);
                        assert(0);
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
                    if (fscanf(f1, "%s", temp_str) != 1)
                    {
                        printf("smth went wrong");
                        fclose(f1);
                        fclose(f2);
                        assert(0);
                    }
                    int reg = get_register_name(temp_str);
                    switch(reg) {
                        case 0:
                        {
                            printf("unknown register");
                            fclose(f1);
                            fclose(f2);
                            assert(0);
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

    fclose(f1);
    fclose(f2);
    return 0;
}
