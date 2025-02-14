//


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "asm.h"

int get_command(const char* str) { // TODO Добавить pop pushr.. jmp

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
        if (stricmp(str, "Hlt") == 0)
            return HLT;

        return 0;
    }

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
            break;
        }

        int command = get_command(str);

        switch(command) { // TODO Когда проходимся, надо записать метки
            case 0:
                break;
            case PUSH:    // TODO придумвать, что делать с условными переходами
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

    put_code_size(f1, f2);

     while(1)
    {
        const int STR_LEN = 20;
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
                case PUSH:
                {

                    fprintf(f2, "%d ", command);  // TODO проверки на команды
                    int value = 0;

                    if (fscanf(f1, "%d", &value) != 1) // NOTE как сделать лучше проверку?
                    {
                    printf("smth went wrong");
                    fclose(f1);
                    fclose(f2);
                    assert(0);
                    break;
                    }

                    fprintf(f2, "%d\n", value);
                    break;

                }

                case ADD:
                {
                    fprintf(f2, "%d\n", command);
                    break;
                }

                case SUB:
                {
                    fprintf(f2, "%d\n", command);
                    break;
                }

                case OUT:
                {
                    fprintf(f2, "%d\n", command);
                    break;
                }

                case DIV:
                {
                    fprintf(f2, "%d\n", command);
                    break;
                }

                case MUL:
                {
                    fprintf(f2, "%d\n", command);
                    break;
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
