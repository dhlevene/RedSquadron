#include <stdio.h>
#include <stdlib.h>

typedef struct Symbol {
    char *identifier;
    int type;
} Symbol;

Symbol* testFunc();

int main(int argc, char* argv[]){

    Symbol *symbol = malloc(sizeof(Symbol));
    symbol->identifier = "tuna";
    symbol->type = 6;
    symbol = testFunc();
    printf("%s%d",symbol->identifier,symbol->type);

    return 0;
}

Symbol* testFunc(){
    Symbol* symbol = malloc(sizeof(Symbol));
    symbol->identifier = "salmon";
    symbol->type = 10;

    return symbol;
}
