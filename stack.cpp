#include "stack.h"

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
