#include <stdio.h>

const int LABELS_MAX_COUNT = 20;
const int LABELS_MAX_NAME = 20;

struct labels
{
    char c[20] = {'2'};  // NOTE как лучше объявить?
    int ip = -2;
};


    int main() {
        struct labels labels_array[LABELS_MAX_COUNT];
        printf("%c", (labels_array[1].c)[0]);
        return 0;
    }

