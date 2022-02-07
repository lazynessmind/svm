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
    INST_LT,
    INST_JMP,
    INST_JNEQ,
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
#define LT { .type = INST_LT, .op = 0 }
#define PLUS {.type = INST_PLUS, .op = 0}
#define MINUS {.type = INST_MINUS, .op = 0}
#define PUSH(operand) {.type = INST_PUSH, .op = operand}
#define HALT {.type = INST_HALT, .op = 0}
#define JMP(addr) {.type = INST_JMP, .op = addr}
#define JNEQ(addr) {.type = INST_JNEQ, .op = addr }
#define DUP {.type = INST_DUP, .op = 0}

struct Instruction program[100] = {
    PUSH(0), //0
    PUSH(1), //0, 1
    DUP, // 0, 1, 1
    PUSH(10), //0, 1, 1, 10
    LT, // 0, 1, [1] == true
    JNEQ(9), //0, 1
    DUP,
    PUSH(1), //0, 1, 1, 1
    PLUS, // 0, 1, 2
    JMP(2),
    HALT
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
        case INST_JNEQ: return "JNEQ";
        case INST_LT: return "LT";
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

    int cnt = 0;
    for(int i = 0; i < 100; i++){
        cnt++;
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

        } else if(program[i].type == INST_LT){
            if(svm.ip < 2) {
                printf("STACK_UNDERFLOW\n");
                return 1;
            }

            int op1 = svm.stack[svm.ip-2];
            int op2 = svm.stack[svm.ip-1];

            svm.ip -= 2;

            svm.stack[svm.ip++] = op1 < op2 ? 1 : 0;
            DumpSVM(&svm, INST_LT);

        } else if(program[i].type == INST_JMP){
            i = program[i].op - 1;
            DumpSVM(&svm, INST_JMP);

        } else if(program[i].type == INST_JNEQ){
            int top = svm.stack[svm.ip - 1];
            svm.ip--;
            if(top != 1)
                i = program[i].op;
            DumpSVM(&svm, INST_JNEQ);
        } else if(program[i].type == INST_HALT) {
            DumpSVM(&svm, INST_HALT);
            break;
        }

    }

    printf("Exit stack:\n");
    DumpSVM(&svm, INST_NOP);

    return 0;
}
