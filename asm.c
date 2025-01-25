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
        if (stricmp(str, "Hlt") == 0)
            return HLT;

        return 0;
    }


int main() {

    FILE *f1 = fopen("first.txt", "r"); // TODO rename (input output)
    if (f1 == NULL)
    {
        printf("f1 == null");
        fclose(f1);
        assert(0);
    }

    FILE *f2 = fopen("second.txt", "w");
    if (f2 == NULL)
    {
        printf("f2 == null"); // TODO close f1
        fclose(f2);
        assert(0);
    }

    

     while(1)
    {
        const STR_LEN = 50;
        char str[STR_LEN] = "";
        if (fscanf(f1, "%s", str) != 1)
        {
            printf("%s", str);
            printf("smth went wrong");
            break;
        }

        int command = get_command(str);

        switch (command)
            {
                case PUSH:
                {
                    fprintf(f2, "%d ", command);
                    int value = 0;
                    fscanf(f1, "%d", &value);
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

        if (command == HLT) // FIXME exit on file end
        {
            fprintf(f2, "%d\n", command);
            break;
        }
    }

    fclose(f1);
    fclose(f2);
    return 0;
}
