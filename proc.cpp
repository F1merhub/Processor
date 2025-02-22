#include "proc.h"

int code_fill(FILE *f, struct Processor *proc) {

    fscanf(f, "%d", &(proc->code_size));
    proc->code = (stack_elem *)calloc(proc->code_size, sizeof(stack_elem));
    if (proc->code == NULL) {
        printf("memory allocation error");
        assert(0);
    }

    int i = 0;
    int buffer = 0;
    while(fscanf(f, "%d", &buffer) == 1) {
        proc->code[i] = buffer;
        i++;
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
    Processor proc = {};

    Stack Stk = {NULL, 0, 0};

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

            case PUSH:
            {
                stack_elem value = proc.code[proc.ip + 1];
                stack_push(&Stk, value);
                proc.ip += 2;
                break;
            }

            case ADD:
            {
                stack_elem temp1 = 0, temp2 = 0;
                stack_pop(&Stk, &temp1);
                stack_pop(&Stk, &temp2);
                stack_push(&Stk, temp1 + temp2);
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
                switch(proc.code[proc.ip + 1]) { // FIXME remove switch
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
    fclose(stderr);
    return 0;
}
