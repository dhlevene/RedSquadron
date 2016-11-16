#include "lexer.h";

variableSymbol* symbol_table[MAX_SYMBOL_TABLE_SIZE];
FILE *sourceFile;
Symbol* currentToken;
int numSymbols=0;
int PC = 0;
FILE *outputFile;

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

    outputFile = fopen(output,"w");

    fclose(sourceFile);
    fclose(outputFile);

    return 0;
}

void emit(int op, int l, int m){
    if(PC > 500)
        printError(0);

    fprintf("%d %d %d", op, l, m);
    PC++;
}

int isRelation(int token){
    if(token==lessym||
       token==gtrsym||
       token==geqsym||
       token==eqsym ||
       token==neqsym||
       token==leqsym)
       return 1;
    return 0;
}

int searchSymbol(char* name){
    int i;
    for(i=0;i<numSymbols;i++){
        if(strcmp(symbol_table[i]->name,name)==0){
            return i;
        }
    }
    return NULL;
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
    int symPosition;

    if(currentToken==identsym){
        symPosition = searchSymbol(currentToken->identifier);
        cycleNextToken();
        if(currentToken!=becomessym)
            printError(0);//Wrong error
        cycleNextToken();
        expression();
        emit(4,0,symbol_table[symPosition]->addr);
    }
    else if(currentToken==callsym){
        cycleNextToken();
        if(currentToken!=identsym)
            printError(0);//Wrong error
        symPosition = searchSymbol(currentToken->identifier);
        cycleNextToken();
        emit(5,0,symbol_table[symPosition]->addr);
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
        emit(8,0,0);
        statement();
        emit(7,0,numSymbols+1);
    }
    else if(currentToken==whilesym){
        cycleNextToken();
        condition();
        emit(8,0,0);
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
        emit(2, 0, 6);
    }
    else{
        expression();
        if(isRelation(currentToken)==0)
            printError(0);//Wrong error
        int relOP = currentToken;
        cycleNextToken();
        expression();
        if(relOP == eqsym)
            emit(2, 0, 8);
        if(relOP == neqsym)
            emit(2, 0, 9);
        if(relOP == lessym)
            emit(2, 0, 10);
        if(relOP == leqsym)
            emit(2, 0, 11);
        if(relOP == gtrsym)
            emit(2, 0, 12);
        if(relOP == geqsym)
            emit(2, 0, 13);
    }
}

void expression(){
    int addop;
    if(currentToken==plussym||minussym){
        addop = currentToken;
        cycleNextToken();
        term();
        if(addop == minussym)
            emit(2, 0, 1);
    }
    else
        term();

    while(currentToken==plussym||currentToken==minussym){
        addop = currentToken;
        cycleNextToken();
        term();
        if(addop == plussym)
            emit(2, 0, 2);
        else
            emit(2, 0, 3);
    }

}

void term(){
    int mulop;
    factor();
    while(currentToken==multsym||currentToken==slashsym){
        mulop = currentToken;
        cycleNextToken();
        factor();
        if(mulop == multsym)
            emit(2, 0, 4);
        else
            emit(2, 0, 5);
    }
}

void factor(){
    int symPosition;
    if(currentToken==identsym){
        symPosition = searchSymbol(currentToken->identifier);

        if((symbol_table[symPosition]->kind) == 1)
            emit(1,0,symbol_table[symPosition]->val);
        else
            emit(3,0,symbol_table[symPosition]->addr);
        cycleNextToken();
    }
    else if(currentToken==numbersym){
        emit(1,0,atoi(currentToken->identifier));
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
