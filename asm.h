#ifndef asm_H
#define asm_H

const int LABELS_MAX_COUNT = 20;
const int LABELS_MAX_NAME = 20;
const int STR_LEN = 20;

struct labels
{
    char value[20] = {'\0'};
    int ip = -1;
};

enum command
{
    PUSH = 1,
    ADD = 2,
    SUB = 3,
    DIV = 4,
    MUL = 5,
    OUT = 6,

    PUSHR = 11,
    POPR = 12,

    JMP = 21,
    JB = 22,
    JBE = 23,
    JA = 24,
    JAE = 25,
    JE = 26,
    JNE = 27,

    HLT = -1,// NOTE -1 in case of error
};

enum registers
{
    rax = 1,
    rbx = 2,
    rcx = 3,
    rdx = 4,
};

#endif
