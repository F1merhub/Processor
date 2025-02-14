#include <stdio.h>

    struct processor
{
    int *code = NULL;
    int ip = 4;
    int register_ax[6] = {0};
    int code_size = 0;
};
    int main() {
    struct processor proc;
    printf("%d", proc.ip);

    return 0;
}
