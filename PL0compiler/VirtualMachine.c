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
#define INPUT_FILE "mcode.pm0"
#define OUTPUT_FILE "trace.txt"

//Instruction struct
typedef struct{
    int op;
    int MAR;
    int load;
    int MDR;

} instruction;

//Function definitions
void execute();
instruction fetch();


int main(){

return 0;
}
