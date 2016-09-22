//Compiler builder 50
//Damian Suski
//Daniel Levene
//Fazle Akbar
//Marcy Yi

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Global declarations
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define INPUT_FILE "mcode.pl0"
#define OUTPUT_FILE "trace.txt"

//Instruction struct
typedef struct{
    int op;
    int l;
    int m;
} instruction;

//Function prototypes
void execute(int instructions[][3], int stack[MAX_STACK_HEIGHT], int pc, int bp, int sp);
char* getOpCode(int op, int M);
void translate(int instructions[][3], char* commands, int counter);
instruction fetch(int instructions[][3], int counter);


int main(){

    instruction ir;

    int instructions[MAX_CODE_LENGTH][3];
    int stack[MAX_STACK_HEIGHT];
    int counterA=0, counterB=0, halt=0;
    int sp=0, pc=0, bp=1;
    int i,j;


    FILE* fr = fopen(INPUT_FILE, "r");

    //Input not found
    if(fr==NULL)
        exit(2);


    for(i = 0; i < MAX_CODE_LENGTH; i++){
        for(j=0; j < 3; j++){
            instructions[i][j] = 0;
        }
    }

    for(i = 0; i<MAX_STACK_HEIGHT; i++){
        stack[i] = 0;
    }

    printf("PL/0 code: \n\n");
    while (fscanf(fr, "%d", &instructions[counterA][0]) != EOF){
           fscanf(fr, "%d", &instructions[counterA][1]);
           fscanf(fr, "%d", &instructions[counterA][2]);

        printf("%3d %d %s %d %2d\n", counterA,instructions[counterA][0],getOpCode(instructions[counterA][0],
                               instructions[counterA][2]),instructions[counterA][1],instructions[counterA][2]);
        counterA++;
    }

    fclose(fr);

    //translate(instructions, commands, counter);

    printf("\nExecution: \n\n");
    //execute(instructions, stack, pc, bp, sp);
    printf("pc\t bp\t sp\n", pc, bp, sp);
    printf("%d\t %d\t %d\n\n", pc, bp, sp);

    while(halt != 1)
    {
        ir = fetch(instructions, pc);
        pc = pc+1;
        if(ir.op == 1){
            sp++;
            stack[sp] = ir.m;
        }

        else if(ir.op == 2){

            if(ir.m == 0){
                sp = bp-1;
                pc = stack[sp+4];
                bp = stack[sp+3];
            }
            if(ir.m == 1){
                stack[sp] = -stack[sp];
            }
            if(ir.m == 2){
                sp--;
                stack[sp] = stack[sp]+stack[sp+1];
            }
            if(ir.m == 3){
                sp--;
                stack[sp] = stack[sp]-stack[sp+1];
            }
            if(ir.m == 4){
                sp--;
                stack[sp] = stack[sp]*stack[sp+1];
            }
            if(ir.m == 5){
                sp--;
                stack[sp] = stack[sp]/stack[sp+1];
            }
            if(ir.m == 6){
                stack[sp] = stack[sp]%2;
            }
            if(ir.m == 7){
                sp--;
                stack[sp] = stack[sp]%stack[sp+1];
            }
            if(ir.m == 8){
                sp--;
                stack[sp] = (stack[sp] == stack[sp+1]);
            }
            if(ir.m == 9){
                sp--;
                stack[sp] = (stack[sp] != stack[sp+1]);
            }
            if(ir.m == 10){
                sp--;
                stack[sp] = (stack[sp] < stack[sp+1]);
            }
            if(ir.m == 11){
                sp--;
                stack[sp] = (stack[sp] <= stack[sp+1]);
            }
            if(ir.m == 12){
                sp--;
                stack[sp] = (stack[sp] > stack[sp+1]);
            }
            if(ir.m == 13){
                sp--;
                stack[sp] = (stack[sp] >= stack[sp+1]);
            }

        }

        else if(ir.op == 3){
            sp++;
            stack[sp] = stack[base(ir.l, bp) + ir.m];
        }

        else if(ir.op == 4){
            printf("one");
            stack[base(ir.l, bp) + ir.m] = stack[sp];
            sp = sp--;
            printf("two");
        }

        else if(ir.op == 5){
            stack[sp+1] = 0;
            stack[sp+2] = base(ir.l, bp);
            stack[sp+3] = bp;
            stack[sp+4] = pc;
            bp = sp+1;
            pc = ir.m;
        }

        else if(ir.op == 6){
            sp = sp+ir.m;
        }

        else if(ir.op == 7){
            pc = ir.m;
        }

        else if(ir.op == 8){
            if(stack[sp] == 0)
                pc = ir.m;

            sp--;
        }

        else if(ir.op == 9){
            if(ir.l == 0){
                printf("%d", stack[sp]);
                sp--;
            }
            if(ir.l == 1){
                sp++;
                scanf("%d", &stack[sp]);
            }
            if(ir.l == 2){
                halt = 1;
            }
        }

        printf("%d\t %d\t %d\n", pc, bp, sp);

        counterB++;
    }

    return 0;
}

int base(int l,int base,int *stack){
    int b1=base;
    while(l>0){
        b1=stack[b1+l];
        l--;
    }

    return b1;
}

instruction fetch(int instructions[][3], int counter){
    instruction ir;

    ir.op = instructions[counter][0];
    ir.l  = instructions[counter][1];
    ir.m  = instructions[counter][2];

    return ir;
}


char* getOpCode(int op, int M){
    char* commands[] = {"LIT", "OPR", "LOD", "STO",
                      "CAL", "INC", "JMP", "JPC",
                      "OUT", "INP", "HLT"};

    char* operations[] = {"RET","NEG","ADD","SUB",
                        "MUL","DIV","ODD","MOD","EQL"};

    if(op==2){
        return operations[M];
    }

    if(op==9){
        if(M==0)
            return commands[8];
        else if(M==1)
            return commands[9];
        else
            return commands[10];
    }

    return commands[op-1];
}

void translate(int instructions[][3], char* commands, int counter){
    int i;
    for(i=0; i<counter; i++){
        printf("%d  ", i);
        if(instructions[i][0] == 9){
            if(instructions[i][2] == 0)
                printf("%s      ", commands[8]);
            if(instructions[i][2] == 1)
                printf("%s      ", commands[9]);
            if(instructions[i][2] == 2)
                printf("%s      ", commands[10]);
        }
        if(instructions[i][0] == 4){
            printf("%s      ", commands[4]);
            printf("%d", instructions[i][1]);
        }
        if(instructions[i][0] == 5){
            printf("%s      ", commands[5]);
            printf("%d", instructions[i][1]);
        }


    }

}

/*void execute(int instructions[][3], int stack[MAX_STACK_HEIGHT], int pc, int bp, int sp){
    int counter, halt=0;
    instruction ir;

    while(halt != 1)
    {
        ir = fetch(instructions, counter);

        if(ir.op == 1){
            sp++;
            stack[sp] = ir.m;
        }

        else if(ir.op == 2){

            if(ir.m == 0){
                sp = bp-1;
                pc = stack[sp+4];
                bp = stack[sp+3];
            }
            if(ir.m == 1){
                stack[sp] = -stack[sp];
            }
            if(ir.m == 2){
                sp--;
                stack[sp] = stack[sp]+stack[sp+1];
            }
            if(ir.m == 3){
                sp--;
                stack[sp] = stack[sp]-stack[sp+1];
            }
            if(ir.m == 4){
                sp--;
                stack[sp] = stack[sp]*stack[sp+1];
            }
            if(ir.m == 5){
                sp--;
                stack[sp] = stack[sp]/stack[sp+1];
            }
            if(ir.m == 6){
                stack[sp] = stack[sp]%2;
            }
            if(ir.m == 7){
                sp--;
                stack[sp] = stack[sp]%stack[sp+1];
            }
            if(ir.m == 8){
                sp--;
                stack[sp] = (stack[sp] == stack[sp+1]);
            }
            if(ir.m == 9){
                sp--;
                stack[sp] = (stack[sp] != stack[sp+1]);
            }
            if(ir.m == 10){
                sp--;
                stack[sp] = (stack[sp] < stack[sp+1]);
            }
            if(ir.m == 11){
                sp--;
                stack[sp] = (stack[sp] <= stack[sp+1]);
            }
            if(ir.m == 12){
                sp--;
                stack[sp] = (stack[sp] > stack[sp+1]);
            }
            if(ir.m == 13){
                sp--;
                stack[sp] = (stack[sp] >= stack[sp+1]);
            }

        }

        else if(ir.op == 3){
            sp++;
            stack[sp] = stack[base(ir.l, bp, stack) + ir.m];
        }

        else if(ir.op == 4){
            stack[base(ir.l, bp, stack) + ir.m] = stack[sp];
            sp = sp--;
        }

        else if(ir.op == 5){
            stack[sp+1] = 0;
            stack[sp+2] = base(ir.l, bp, stack);
            stack[sp+3] = bp;
            stack[sp+4] = pc;
            bp = sp+1;
            pc = ir.m;
        }

        else if(ir.op == 6){
            sp = sp+ir.m;
        }

        else if(ir.op == 7){
            pc = ir.m;
        }

        else if(ir.op == 8){
            if(stack[sp] == 0)
                pc = ir.m;

            sp--;
        }

        else if(ir.op == 9){
            if(ir.l == 0){
                printf("%d", stack[sp]);
                sp--;
            }
            if(ir.l == 1){
                sp++;
                scanf("%d", &stack[sp]);
            }
            if(ir.l == 2){
                halt = 1;
            }
        }

        counter++;
    }

}
*/
