#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *identifier;
    int type;
} Symbol;

typedef enum token {
  nulsym = 1, identsym, numbersym, plussym, minussym,
  multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
  gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
  periodsym, becomessym, beginsym, endsym, ifsym, thensym,
  whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
  readsym , elsesym
} token_type;

Symbol* testFunc();

int main(int argc, char* argv[]){

    char* simples = "3hell3o";
    if(isalpha(simples[5]))
        printf("yey");

    printf("\n\n\n");

    Symbol *symbol = malloc(sizeof(Symbol));
    symbol->identifier = "tuna";
    symbol->type = 6;
    symbol = testFunc();
    printf("%s%d",symbol->identifier,symbol->type);

    Symbol* testSymbol = malloc(sizeof(Symbol));
    testSymbol->type = varsym;
    printf("\n%d", testSymbol->type);

    return 0;
}

Symbol* testFunc(){
    Symbol* symbol = malloc(sizeof(Symbol));
    symbol->identifier = "salmon";
    symbol->type = 10;

    return symbol;
}
