#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "proc.h"
#include "asm.h"

int main() {
    FILE *f2 = fopen("second.txt", "r");
    if (f2 == NULL)
    {
        printf("f1 == null");
        assert(0);
    }
    int command = 0;
    fscanf(f2, "%d", &command);
    printf("%d", command);

    return 0;
}
