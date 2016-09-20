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

//Function definitions
void execute();
char* getOpCode(int op, int M);
void translate(int** instructions, char* commands, int counter);
instruction fetch();



int main(){

    instruction ir;

    int instructions[MAX_CODE_LENGTH][3];
    int stack[MAX_STACK_HEIGHT];
    int sp=0,pc=0,counter = 0, halt = 0;
    int bp=1;
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

    while (fscanf(fr, "%d", &instructions[counter][0]) != EOF){
        fscanf(fr,"%d",&instructions[counter][1]);
        fscanf(fr,"%d",&instructions[counter][2]);

        printf("%d %s %d %d\n",instructions[counter][0],getOpCode(instructions[counter][0],
                                    instructions[counter][2]),instructions[counter][1],instructions[counter][2]);
    }

    fclose(fr);

    printf("PL/0 code: \n");
    //translate(instructions, commands, counter);

    while(halt!=1){

        ir = fetch(counter,instructions);

        if(ir.op==1){
            sp=sp+1;
            stack[sp]=ir.m;
        }
        else if(ir.op==2){
            if (ir.m == 0){
                sp = bp -1;
                pc = stack[sp + 4];
                bp = stack[sp + 3];
            } else if (ir.m ==1){
                stack[sp] = -stack[sp];
            } else if ( ir.m == 2) {
                sp = sp-1;
                stack[sp] = stack[sp] + stack[sp+1];

            } else if ( ir.m ==3 ) {
                sp = sp-1;
                stack[sp] = stack[sp] - stack[sp+1];
            }else if ( ir.m ==4 ) {
                sp=sp-1;
                stack[sp] = stack[sp] * stack[sp+1];
            }else if ( ir.m ==5 ) {
                sp=sp-1;
                sp = sp-1;
                stack[sp] = stack[sp] / stack[sp + 1];
            }else if ( ir.m ==6 ) {
                sp = sp-1;
                stack[sp] = stack[sp] % 2;
            }else if ( ir.m ==7  ) {
                sp = sp-1;
                stack[sp] = stack[sp] % stack[sp + 1];
            }else if ( ir.m ==8 ) {
                sp = sp-1;
                stack[sp] = stack[sp] == stack[sp + 1];
            }else if ( ir.m ==9 ){
                sp = sp-1;
                stack[sp] = stack[sp] != stack[sp + 1];
            }else if ( ir.m == 10 ) {
                sp = sp-1;
                stack[sp] = stack[sp] <  stack[sp + 1];
            }else if ( ir.m == 11 ) {
            sp = sp-1;
            stack[sp] = stack[sp] <= stack[sp + 1];
            }else if ( ir.m ==12) {
                sp = sp-1;
                stack[sp] = stack[sp] - stack[sp + 1];
            } else if ( ir.m ==13) {
                sp = sp-1;
                stack[sp] = stack[sp] >= stack[sp + 1];
            }
        }

        else if(ir.op==3){
            sp=sp+1;
            stack[sp]=stack[base(ir.l,bp)+ir.m];
        }
        else if(ir.op==4){
            stack[base(ir.l,bp)+ir.m]=stack[sp];
        }
        else if(ir.op==5){
            stack[sp+1]=0;
            stack[sp+2]=base(ir.l,bp);
            stack[sp+3]=bp;
            stack[sp+4]=pc;

            bp=sp+1;
            pc=ir.m;
        }
        else if(ir.op==6){
            sp=sp+ir.m;
        }
        else if(ir.op==7){
            pc=ir.m;
        }
        else if(ir.op==8){
            if(stack[sp]==0)
                pc=ir.m;
            sp=sp-1;
        }
        else if(ir.op==9){
            if(ir.m == 0) {
                    //print(stack(stack[sp]))?
                sp=sp-1;
            } else if (ir.m==1) {
                sp=sp+1;
                // read(Stack[sp]);
            } else {
                halt=1;
            }

        }
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

instruction fetch(int counter, int** instructions){
    instruction ir;
    ir.op=instructions[counter][0];
    ir.l=instructions[counter][1];
    ir.m=instructions[counter][2];

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

void translate(int** instructions, char* commands, int counter){
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
