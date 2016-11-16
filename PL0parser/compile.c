#include "lexer.h"

variableSymbol* symbol_table[MAX_SYMBOL_TABLE_SIZE];
FILE *sourceFile;
Symbol* currentToken;
int numSymbols;

const char *errorMessages[] = {"Use = instead of :=",
"= must be followed by a number",
"Identifier must be follow by ="};

int main(int argc, int* argv[]){

    if(argc<3){
        printf("Error: Not enough arguments");
        exit(1);
    }

    char* input = argv[1];
    char* output = argv[2];

    sourceFile = fopen(input,"r");

    FILE *outputFile = fopen(output,"w");

    fclose(sourceFile);
    fclose(outputFile);

    return 0;
}

int isRelation(int token){
    if(token==lessym||
       token==gtrsym||
       token==geqsym||
       token==eqsym||
       token==neqsym||
       token==leqsym)
       return 1;
    return 0;
}

variableSymbol* addVariableSymbol(){
    int i;

    while(i<numSymbols){
        if(strcmp(symbol_table[i]->name,currentToken->identifier)==0){
            printError(0);//Wrong error
            exit(1);
            return NULL;
        }
    }

    if(i==numSymbols&&numSymbols!=MAX_SYMBOL_TABLE_SIZE){
        variableSymbol* newSymbol = calloc(1,sizeof(variableSymbol));
        strcpy(newSymbol->name,currentToken->identifier);
        symbol_table[i]=newSymbol;
        numSymbols++;
        return newSymbol;
    }

    return NULL;
}

void cycleNextToken(){
    currentToken = nextSymbol(sourceFile);
}

void printError(int errorNum){
    printf("Error number %d, %s",errorNum,errorMessages[errorNum]);
}

void startProgram(){
    cycleNextToken();
    block();
    if(currentToken!=periodsym){
        printError(0);//Should be error 9
    }
    exit(1);
}

void block(){
    if(currentToken==constsym){
        while(currentToken!=commasym){
            cycleNextToken();
            if(currentToken!=identsym)
                printError(0);//Not correct error
            variableSymbol* tempSymbol = addVariableSymbol();
            tempSymbol->kind = 1;
            cycleNextToken();
            if(currentToken!=eqsym)
                printError(0);//Not correct error
            cycleNextToken();
            if(currentToken!=numbersym)
                printError(0);//Not correct error
            tempSymbol->val=atoi(currentToken->identifier);
            cycleNextToken();
        }
        if(currentToken!=semicolonsym)
            printError(0);//Not correct error
        cycleNextToken();
    }
    if(currentToken==varsym){
        while(currentToken!=commasym){
            cycleNextToken();
            if(currentToken!=identsym)
                printError(0);
            variableSymbol* tempSymbol = addVariableSymbol();
            tempSymbol->kind=2;
            cycleNextToken();
        }
        if(currentToken!=semicolonsym)
            printError(0);//Wrong error
        cycleNextToken();
    }
    while(currentToken==procsym){
        cycleNextToken();
        if(currentToken!=identsym)
            printError(0);//Wrong error
        variableSymbol* tempSymbol = addVariableSymbol();
        tempSymbol->kind=3;
        cycleNextToken();
        if(currentToken!=semicolonsym)
            printError(0);
        cycleNextToken();
        block();
        if(currentToken!=semicolonsym)
            printError(0);//Wrong error
        cycleNextToken();
    }
    statement();
}

void statement(){
    if(currentToken==identsym){
        cycleNextToken();
        if(currentToken!=becomessym)
            printError(0);//Wrong error
        cycleNextToken();
        expression();
    }
    else if(currentToken==callsym){
        cycleNextToken();
        if(currentToken!=identsym)
            printError(0);//Wrong error
        cycleNextToken();
    }
    else if(currentToken==beginsym){
        cycleNextToken();
        statement();
        while(currentToken==semicolonsym){
            cycleNextToken();
            statement();
        }
        if(currentToken!=endsym)
            printError(0);//Wrong error
        cycleNextToken();
    }
    else if(currentToken==ifsym){
        cycleNextToken();
        condition();
        if(currentToken!=thensym)
            printError(0);//Wrong error
        cycleNextToken();
        statement();
    }
    else if(currentToken==whilesym){
        cycleNextToken();
        condition();
        if(currentToken!=dosym)
            printError(0);//Wrong error
        cycleNextToken();
        statement();
    }
}

void condition(){
    if(currentToken==oddsym){
        cycleNextToken();
        expression();
    }
    else{
        expression();
        if(isRelation(currentToken)==0)
            printError(0);//Wrong error
        cycleNextToken();
        expression();
    }
}

void expression(){
    if(currentToken==plussym||minussym){
        cycleNextToken();
        term();
        while(currentToken==plussym||currentToken==minussym){
            cycleNextToken();
            term();
        }
    }
}

void term(){
    factor();
    while(currentToken==multsym||currentToken==slashsym){
        cycleNextToken();
        factor();
    }
}

void factor(){
    if(currentToken==identsym){
        cycleNextToken();
    }
    else if(currentToken==numbersym){
        cycleNextToken();
    }
    else if(currentToken==lparentsym){
        cycleNextToken();
        expression();
        if(currentToken!=rparentsym)
            printError(0);//Wrong error
        cycleNextToken();
    }
    else
        printError(0);//Wrong error
}
