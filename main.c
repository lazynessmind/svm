#include <stdio.h>

#define STACK_SIZE 1000

enum InstType {
    NOP = 0,
    PUSH,
    DUP,
    PLUS,
    MINUS,
    EQ,
    JMP, //Maybe next time?
};

struct Instruction {
    enum InstType type;
    int op;
};

struct SVM {
    int stack[STACK_SIZE];
    int ip;
};

//#define EQUAL {.type = EQ, .op = 0}
#define PLUSI {.type = PLUS, .op = 0}
#define MINUSI {.type = MINUS, .op = 0}
#define PUSHI(operand) {.type = PUSH, .op = operand}

struct Instruction program[100] = {
    PUSHI(35),
    PUSHI(34),
    PLUSI,
    PUSHI(35),
    MINUS,
    PUSHI(34),
    {.type = EQ, .op = 0},
};

void DumpSVM(struct SVM* svm) {
    printf("Current Stack:\n");
    for(int i = 0; i < svm->ip; i++){
        printf("  IP:%d, OP:%d\n", i, svm->stack[i]);
    }
}

int main(void){

    struct SVM svm = {0};


    for(int i = 0; i < 100; i++){
        if(program[i].type == PUSH){
            svm.stack[svm.ip++] = program[i].op;
            DumpSVM(&svm);
        } else if(program[i].type == DUP){
            int top = svm.stack[svm.ip - 1];
            svm.stack[svm.ip++] = top;
            DumpSVM(&svm);
        } else if(program[i].type == PLUS){
            int op1 = svm.stack[svm.ip-2];
            int op2 = svm.stack[svm.ip-1];

            svm.ip -= 2;

            svm.stack[svm.ip++] = op1 + op2;
            DumpSVM(&svm);
        } else if(program[i].type == MINUS){

            if(svm.ip < 2) {
                printf("STACK_UNDERFLOW\n");
                return 1;
            }

            int op1 = svm.stack[svm.ip-2];
            int op2 = svm.stack[svm.ip-1];

            svm.ip -= 2;
            
            svm.stack[svm.ip++] = op1 - op2;
            DumpSVM(&svm);
        } else if(program[i].type == EQ){
            if(svm.ip < 2) {
                printf("STACK_UNDERFLOW\n");
                return 1;
            }

            int op1 = svm.stack[svm.ip-2];
            int op2 = svm.stack[svm.ip-1];

            svm.ip -= 2;
            
            svm.stack[svm.ip++] = op1 == op2 ? 1 : 0;
            DumpSVM(&svm);
            
        }

    }

    DumpSVM(&svm);

    return 0;
}
