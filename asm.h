#ifndef asm_H
#define asm_H

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

    HLT = -1,
};

enum registers
{
    rax = 1,
    rbx = 2,
    rcx = 3,
    rdx = 4,
};

#endif
