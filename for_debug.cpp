#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "asm.h"

int main() {
    char label_value[20] = {'\0'};
    char jmp_buffer[20] = "abc";
    strncpy(label_value, jmp_buffer + 1, strlen(jmp_buffer) - 1);
    printf("%s", label_value);
    return 0;
}
