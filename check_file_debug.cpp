#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "asm.h"


int main() {
    const int STR_LEN = 20;
    char str[STR_LEN] = "";
    char str1[STR_LEN] = "";
    FILE *f1 = fopen("debug.txt", "r");
    if (fscanf(f1, "%s", str) != 1) // NOTE как сделать лучше проверку?
        {
            printf("smth went wrong");
            fclose(f1);
            assert(0);
        }
    fscanf(f1, "%s", str1);
    printf("%s", str);
    printf("%s", str1);
    return 0;
}
