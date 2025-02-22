#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "asm.h"

#define max_length 5

int main() {
    FILE *f = fopen("pupu.txt", "r");
    char s[10] = {'\0'};
    fscanf(f, "%5s", s);
    printf("%s", s);
    return 0;
}
