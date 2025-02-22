#ifndef stack_H
#define stack_H

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "asm.h"
#include <stdlib.h>

typedef int stack_elem;

struct Stack
{
    stack_elem* data;
    int size;
    int capacity;
};

const int increase_coefficient = 20;

const stack_elem CANARY = (stack_elem)0xBADC0FFEE;

const stack_elem POISON = (stack_elem)0xBAD1ABA;

enum errorcode
{
    STK_OK =                        0,  // все оки
    STK_OUT_MEMORY =                1,  // calloc не дал память
    STK_NULL_POINTER =              2,  // date = 0
    STK_BAD_SIZE =                  3,  // size < 0
    STK_BAD_CAPACITY =              4,  // capacity <= 0
    STK_SIZE_LARGER_CAPACITY =      5,  // вышли за размер стэка
    BAD_CANARY_1 =                  7,  // левая канарейка
    BAD_CANARY_2 =                  8,  // правая канарейка
};

int verificator(struct Stack *Stk);

const char* decoder(int error);

int stack_dump(struct Stack *Stk);

int stk_assert(struct Stack *Stk);

int stk_null_check(struct Stack *Stk);

int put_canary(struct Stack *Stk);

int stack_destructor(struct Stack* Stk);

int stack_constructor(struct Stack * Stk, int capacity);

int realloc_up(struct Stack *Stk);

int stack_push(struct Stack *Stk, stack_elem value);

int stack_pop(struct Stack *Stk, stack_elem *pop_elem);

#endif
