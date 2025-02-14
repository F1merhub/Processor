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

    HLT = -1,
};

#endif
