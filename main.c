#include <stdio.h>

#define STACK_SIZE 1000

enum InstType {
    INST_NOP = 0,
    INST_HALT,
    INST_PUSH,
    INST_DUP,
    INST_PLUS,
    INST_MINUS,
    INST_EQ,
    INST_JMP, //Maybe next time?
};

struct Instruction {
    enum InstType type;
    int op;
};

struct SVM {
    int stack[STACK_SIZE];
    int ip;
};

#define EQUAL {.type = INST_EQ, .op = 0}
#define PLUSI {.type = INST_PLUS, .op = 0}
#define MINUSI {.type = INST_MINUS, .op = 0}
#define PUSHI(operand) {.type = INST_PUSH, .op = operand}
struct Instruction program[100] = {
    PUSHI(35),
    PUSHI(34),
    PLUSI, //69
    PUSHI(35), //35
    MINUSI, //34
    PUSHI(34), //34
    EQUAL // 1
};

const char* InstToStr(int inst){
    switch(inst){
        case INST_NOP: return "NOP";
        case INST_HALT: return "HALT";
        case INST_PUSH: return "PUSH";
        case INST_DUP: return "DUP";
        case INST_PLUS: return "PLUS";
        case INST_MINUS: return "MINUS";
        case INST_EQ: return "EQUAL";
        case INST_JMP: return "JUMP";
        default:
            return "Unreachable";
    }
}

void DumpSVM(struct SVM* svm, int inst) {
    printf("Current Stack: %s\n", InstToStr(inst));
    for(int i = 0; i < svm->ip; i++){
        printf("  IP:%d, OP:%d\n", i, svm->stack[i]);
    }
}

int main(void){

    struct SVM svm = {0};


    for(int i = 0; i < 100; i++){
        if(program[i].type == INST_PUSH){
            svm.stack[svm.ip++] = program[i].op;
            DumpSVM(&svm, INST_PUSH);
        } else if(program[i].type == INST_DUP){
            int top = svm.stack[svm.ip - 1];
            svm.stack[svm.ip++] = top;
            DumpSVM(&svm, INST_DUP);
        } else if(program[i].type == INST_PLUS){
            int op1 = svm.stack[svm.ip-2];
            int op2 = svm.stack[svm.ip-1];

            svm.ip -= 2;

            svm.stack[svm.ip++] = op1 + op2;
            DumpSVM(&svm, INST_PLUS);
        } else if(program[i].type == INST_MINUS){

            if(svm.ip < 2) {
                printf("STACK_UNDERFLOW\n");
                return 1;
            }

            int op1 = svm.stack[svm.ip-2];
            int op2 = svm.stack[svm.ip-1];

            svm.ip -= 2;

            svm.stack[svm.ip++] = op1 - op2;
            DumpSVM(&svm, INST_MINUS);
        } else if(program[i].type == INST_EQ){
            if(svm.ip < 2) {
                printf("STACK_UNDERFLOW\n");
                return 1;
            }

            int op1 = svm.stack[svm.ip-2];
            int op2 = svm.stack[svm.ip-1];

            svm.ip -= 2;

            svm.stack[svm.ip++] = op1 == op2 ? 1 : 0;
            DumpSVM(&svm, INST_EQ);

        }

    }

    printf("Exit stack:\n");
    DumpSVM(&svm, INST_NOP);

    return 0;
}
