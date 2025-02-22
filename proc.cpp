#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "proc.h"
#include "asm.h"

typedef int stack_elem;
// TODO сделать typedef для спецификаторов
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

int verificator(struct Stack *Stk)
{
    int error = 0;

    if (Stk == NULL)
        error = STK_NULL_POINTER;

    if (Stk->data == NULL)
        error = STK_OUT_MEMORY;

    if (Stk->size < 0)
        error = STK_BAD_SIZE;

    if (Stk->capacity <= 0)
        error = STK_BAD_CAPACITY;

    if (Stk->size > Stk->capacity)
        error = STK_SIZE_LARGER_CAPACITY;

    if (Stk->data[0] != CANARY)
        error = BAD_CANARY_1;

    if (Stk->data[Stk->capacity + 1] != CANARY)
        error = BAD_CANARY_2;

    return error;
}

const char* decoder(int error) {
    switch(error) {
        case(STK_OUT_MEMORY):
            return "memory allocation error\n";
        case(STK_NULL_POINTER):
            return "Stack pointer is null\n";
        case(STK_BAD_SIZE):
            return "Stack size < 0\n";
        case(STK_BAD_CAPACITY):
            return "Stack capacity <= 0\n";
        case(STK_SIZE_LARGER_CAPACITY):
            return "size > capacity\n";
        case(BAD_CANARY_1):
            return "canary1 was changed\n";
        case(BAD_CANARY_2):
            return "canary2 was changed\n";
        default:
            return "unknown error\n";
    }
}

int stack_dump(struct Stack *Stk) {
    for (int i = 1; i < (Stk->size) + 1; ++i) {
        printf("%d ", Stk->data[i]);
    }
    printf("\n"
           "%d - capacity\n"
           "%d - size\n"
           "%p - pointer on data\n",
           Stk->capacity, Stk->size, Stk->data);
    return 0;
}

int stk_assert(struct Stack *Stk) {
    int error = verificator(Stk);
    if (error) {
        printf("%s", decoder(error));
        stack_dump(Stk);
        assert(0);
    }
    return 0;
}


int stk_null_check(struct Stack *Stk) {
    if (Stk == NULL) {
        printf("Stk pointer is NULL\n");
        assert(0);
    }
    return 0;
}

int put_canary(struct Stack *Stk)
{
    stk_null_check(Stk);
    Stk->data[0] = CANARY;
    Stk->data[Stk->capacity + 1] = CANARY;

    return 0;
}


int stack_destructor(struct Stack* Stk) {
    stk_null_check(Stk);
    for (int i = 0; i < Stk->capacity + 1; ++i)
        Stk->data[i] = POISON;
    free(Stk->data);
    Stk->data = NULL;
    Stk->capacity = 0;
    Stk->size = 0;

    return 0;
}


int stack_constructor(struct Stack * Stk, int capacity) {

    stk_null_check(Stk);

    if (capacity <= 0) {
        printf("capacity is not positive\n");
        assert(0);
    }

    Stk->data = (stack_elem *)calloc(capacity + 2, sizeof(stack_elem));
    if (Stk->data == NULL) {
        printf("memory allocation error\n");
        assert(0);
    }

    Stk->size = 0;
    Stk->capacity = capacity;
    put_canary(Stk);
    stk_assert(Stk);

    return 0;
}

int realloc_up(struct Stack *Stk) {
    stk_assert(Stk);
    if (increase_coefficient <= 0) {
        printf("change increase_coefficient\n");
        assert(0);
    }
    stack_elem *new_ptr = (stack_elem *)realloc(Stk->data, (Stk -> capacity + 2 + increase_coefficient) * sizeof(stack_elem));
    if(new_ptr == NULL) {
        printf("memory reallocation error\n");
        assert(0);
    }
    Stk->data = new_ptr;
    Stk->capacity = Stk->capacity + increase_coefficient;
    Stk->data[Stk->capacity + 1] = CANARY;
    return 0;
}



int stack_push(struct Stack *Stk, stack_elem value) {
    stk_assert(Stk);
    if (Stk->size  == Stk->capacity) {
        realloc_up(Stk);
    }

    Stk->data[Stk->size + 1] = value;
    (Stk->size)++;
    stk_assert(Stk);

    return 0;
}

int stack_pop(struct Stack *Stk, stack_elem *pop_elem) {
    stk_assert(Stk);
    if (Stk->size  == 0) {
        printf("empty Stack\n");
        assert(0);
    }
    Stk->size--;
    *pop_elem = Stk->data[Stk->size + 1];
    Stk->data[Stk->size + 1] = POISON;
    stk_assert(Stk);

    return 0;
}


// ******************************************************************************************** NOTE - PROCESSOR
const int MAX_REGISTER_SIZE = 20;

struct Processor // processor - Processor
{
    int *code = NULL;
    int ip = 0;
    stack_elem register_ax[MAX_REGISTER_SIZE] = {0};
    int register_size = 0;
    int code_size = 0;
};

int code_fill(FILE *f, struct Processor *proc) {

    fscanf(f, "%d", &(proc->code_size));
    proc->code = (stack_elem *)calloc(proc->code_size, sizeof(stack_elem));
    if (proc->code == NULL) {
        printf("memory allocation error");
        assert(0);
    }

    int i = 0;
    int buffer = 0;
    while(1) { // TODO занести в цикл

        if(fscanf(f, "%d", &buffer) == 1) {
            proc->code[i] = buffer;
            i++;
        }
        else
        {
            break;
        }

    }

    return 0;
}

int push_rax(struct Processor *proc, struct Stack *Stk) {
    if (proc->register_size <= 0) {
        printf("bad register size");
        assert(0);
    }
    stack_push(Stk, proc->register_ax[proc->register_size - 1]);

    return 0;
}

int pop_rax(struct Processor *proc, struct Stack *Stk) {
    stack_elem value = 0;
    stack_pop(Stk, &value);
    proc->register_ax[proc->register_size] = value;
    proc->register_size++;

    return 0;
}

int JMP_condition(int command, int temp1, int temp2) {
    switch(command) {
        case JB:
            return (temp2 < temp1);
        case JBE:
            return (temp2 <= temp1);
        case JA:
            return (temp2 > temp1);
        case JAE:
            return (temp2 >= temp1);
        case JE:
            return (temp2 == temp1);
        case JNE:
            return (temp2 != temp1);
        default:
        {
            printf("unknown JMP");
            assert(0);
        }
    }
}


int main ()
{
    struct Processor proc;

    struct Stack Stk = {NULL, 0, 0};

    stack_constructor(&Stk, 10);

    FILE *f2 = fopen("code.txt", "r");
    if (f2 == NULL)
    {
        printf("f2 == null");
        assert(0);
    }

    code_fill(f2, &proc);
    for(int i = 0; i < 24; i++) { //  NOTE - распечатка
        printf("%d ", proc.code[i]);
    }
    printf("\n");

    while(proc.ip < proc.code_size) {
        int command = proc.code[proc.ip];
        switch (command) {

            case PUSH://TODO исправить копипаст
            {
                stack_elem value = proc.code[proc.ip + 1];
                stack_push(&Stk, value);
                proc.ip += 2;
                break;
            }

            case ADD:
            {
                stack_elem temp1 = 0, temp2 = 0, temp3 = 0;
                stack_pop(&Stk, &temp1);
                stack_pop(&Stk, &temp2);
                temp3 = temp1 + temp2;
                stack_push(&Stk, temp3);
                proc.ip++;
                break;
            }

            case SUB:
            {
                stack_elem temp1 = 0, temp2 = 0, temp3 = 0;
                stack_pop(&Stk, &temp1);
                stack_pop(&Stk, &temp2);
                temp3 = temp2 - temp1;
                stack_push(&Stk, temp3);
                proc.ip++;
                break;
            }

            case DIV:
            {
                stack_elem temp1 = 0, temp2 = 0, temp3 = 0;
                stack_pop(&Stk, &temp1);
                stack_pop(&Stk, &temp2);
                if (temp1 == 0) {
                    printf("attempt to divide on zero");
                    assert(0);
                }
                temp3 = temp2 / temp1;
                stack_push(&Stk, temp3);
                proc.ip++;
                break;
            }

            case MUL:
            {
                stack_elem temp1 = 0, temp2 = 0, temp3 = 0;
                stack_pop(&Stk, &temp1);
                stack_pop(&Stk, &temp2);
                temp3 = temp2 * temp1;
                stack_push(&Stk, temp3);
                proc.ip++;
                break;
            }

            case OUT:
            {
                stack_elem result = 0;
                stack_pop(&Stk, &result);
                printf("\nOUT : %d\n", result);
                proc.ip++;
                break;
            }

            case JMP:
            {
                int value = proc.code[proc.ip + 1];
                if ((value < 0) || (value >= proc.code_size)) {
                    printf("Unappropriate value for jmb!");
                    assert(0);
                }
                proc.ip = value;
                break;
            }
            case JB:
            case JBE:
            case JA:
            case JAE:
            case JE:
            case JNE:
            {
                int value = proc.code[proc.ip + 1];
                    if ((value < 0) || (value >= proc.code_size)) {
                    printf("Unappropriate value for jmb!");
                    assert(0);
                }
                stack_elem temp1 = 0, temp2 = 0;
                stack_pop(&Stk, &temp1);
                stack_pop(&Stk, &temp2);
                stack_push(&Stk, temp2);
                stack_push(&Stk, temp1);
                if (JMP_condition(command, temp1, temp2)) {
                    proc.ip = value;
                    break;
                }
                else
                {
                    proc.ip += 2;
                    break;
                }
            }


            case PUSHR:
            {
                switch(proc.code[proc.ip + 1]) {
                    case rax:
                    case rbx:
                    case rcx:
                    case rdx:
                    {
                        push_rax(&proc, &Stk);
                        proc.ip+=2;
                        break;
                    }
                    default:
                        break;
                }

                break;

            }

            case POPR:
            {
                switch(proc.code[proc.ip + 1]) {
                    case rax:
                    case rbx:
                    case rcx:
                    case rdx:
                    {
                        pop_rax(&proc, &Stk);
                        proc.ip+=2;
                        break;
                    }
                    default:
                        break;
                }

                break;

            }
            default:
                break;

        }

        if (command == HLT) {
            fclose(f2);
            break;
        }

    }

    stack_destructor(&Stk);

    return 0;
}
