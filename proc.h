#ifndef proc_H
#define proc_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "proc.h"
#include "asm.h"
#include "stack.h"

const int MAX_REGISTER_SIZE = 20;

struct Processor
{
    int *code = NULL;
    int ip = 0;
    stack_elem register_ax[MAX_REGISTER_SIZE] = {0};
    int register_size = 0;
    int code_size = 0;
};

#endif
