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

//Instruction struct
typedef struct{
    int op;
    int l;
    int m;
} instruction;

//Function prototypes
void execute(int instructions[][4], int stack[MAX_STACK_HEIGHT], int pc, int bp, int sp);
char* getOpCode(int op, int M);
instruction fetch(int instructions[][4], int counter);
int base(int l, int base, int stack[]);
void printStack(int stack[], int sp, int cal);


int main(int argc, char *argv[]){

    int instructions[MAX_CODE_LENGTH][4];
    int stack[MAX_STACK_HEIGHT];
    int counter=0;
    int sp=0, pc=0, bp=1;
    int i,j;
    char* name;


    FILE* fr = fopen(argv[1], "r");

    //Input not found
    if(fr==NULL)
        exit(2);


    for(i = 0; i < MAX_CODE_LENGTH; i++){
        for(j=0; j < 4; j++){
            instructions[i][j] = 0;
        }
    }

    for(i = 0; i<MAX_STACK_HEIGHT; i++){
        stack[i] = 0;
    }

    printf("PL/0 code: \n\n");
    while (fscanf(fr, "%d", &instructions[counter][0]) != EOF){
           fscanf(fr, "%d", &instructions[counter][1]);
           fscanf(fr, "%d", &instructions[counter][2]);
           instructions[counter][3] = counter;

        name = getOpCode(instructions[counter][0], instructions[counter][2]);
        printf("%3d  %s", counter, name);
        if(name == "STO" || name == "CAL" || name == "LOD")
            printf("    %d", instructions[counter][1]);
        if(name != "RET" && name != "HLT" && instructions[counter][0] != 2)
            printf("\t %2d", instructions[counter][2]);
        printf("\n");
        counter = counter+1;
    }

    fclose(fr);

    printf("\nExecution: \n");
    printf("\t\t\t pc\t  bp\t  sp\tstack\n", pc, bp, sp);
    printf("\t\t\t %2d\t  %2d\t  %2d\n", pc, bp, sp);
    execute(instructions, stack, pc, bp, sp);

    return 0;
}

int base(int l, int base, int stack[]){
    int b1=base;
    while(l>0){
        b1=stack[b1+1];
        l--;
    }

    return b1;
}

instruction fetch(int instructions[][4], int counter){
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

void execute(int instructions[][4], int stack[MAX_STACK_HEIGHT], int pc, int bp, int sp){

    int halt = 0;
    int cal = -1;
    char* name;
    instruction ir;

    while(halt != 1)
    {
        ir = fetch(instructions, pc);

        name = getOpCode(instructions[pc][0], instructions[pc][2]);
        printf("%3d  %s\t", instructions[pc][3], name);

        if(name == "STO" || name == "CAL" || name == "LOD")
            printf("    %d", instructions[pc][1]);
        if(name != "RET" && name != "HLT" && instructions[pc][0] != 2)
            printf("\t%3d", instructions[pc][2]);
        else
            printf("\t");

        pc = pc+1;

        //Actually performing the operations:
        if(ir.op == 1){
            sp = sp+1;
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
                sp = sp-1;
                stack[sp] = stack[sp]+stack[sp+1];
            }
            if(ir.m == 3){
                sp = sp-1;
                stack[sp] = stack[sp]-stack[sp+1];
            }
            if(ir.m == 4){
                sp = sp-1;
                stack[sp] = stack[sp]*stack[sp+1];
            }
            if(ir.m == 5){
                sp = sp-1;
                stack[sp] = stack[sp]/stack[sp+1];
            }
            if(ir.m == 6){
                sp = sp-1;
                stack[sp] = stack[sp]%2;
            }
            if(ir.m == 7){
                sp = sp-1;
                stack[sp] = stack[sp]%stack[sp+1];
            }
            if(ir.m == 8){
                sp = sp-1;
                stack[sp] = stack[sp] == stack[sp+1];
            }
            if(ir.m == 9){
                sp = sp-1;
                stack[sp] = stack[sp] != stack[sp+1];
            }
            if(ir.m == 10){
                sp = sp-1;
                stack[sp] = stack[sp] < stack[sp+1];
            }
            if(ir.m == 11){
                sp = sp-1;
                stack[sp] = stack[sp] <= stack[sp+1];
            }
            if(ir.m == 12){
                sp = sp-1;
                stack[sp] = stack[sp] > stack[sp+1];
            }
            if(ir.m == 13){
                sp = sp-1;
                stack[sp] = stack[sp] >= stack[sp+1];
            }

        }

        else if(ir.op == 3){
            sp = sp+1;
            stack[sp] = stack[base(ir.l, bp, stack) + ir.m];
        }

        else if(ir.op == 4){
            stack[base(ir.l, bp, stack) + ir.m] = stack[sp];
            sp = sp-1;
        }

        else if(ir.op == 5){
            stack[sp+1] = 0;
            stack[sp+2] = base(ir.l, bp, stack);
            stack[sp+3] = bp;
            stack[sp+4] = pc;
            bp = sp+1;
            pc = ir.m;
            cal = sp;
        }

        else if(ir.op == 6){
            sp = sp + ir.m;
        }

        else if(ir.op == 7){
            pc = ir.m;
        }

        else if(ir.op == 8){
            if(stack[sp] == 0)
                pc = ir.m;

            sp = sp-1;
        }

        else if(ir.op == 9){
            if(ir.m == 0){
                printf("%d", stack[sp]);
                sp = sp-1;
            }
            if(ir.m == 1){
                sp = sp+1;
                scanf("%d", &stack[sp]);
            }
            if(ir.m == 2){
                halt = 1;
            }
        }

        //Printing out pc, bp, sp, and stack.
        printf("\t%3d\t %3d\t %3d\t", pc, bp, sp);
        if(name != "JMP")
            printStack(stack, sp, cal);
        printf("\n");

    }

}

void printStack(int stack[], int sp, int cal){
    int i;
    for(i=1; i<=sp; i++){
        printf("%d ", stack[i]);
        if(cal != -1)
            if(i==cal && i!=sp)
                printf("| ");
    }
}
