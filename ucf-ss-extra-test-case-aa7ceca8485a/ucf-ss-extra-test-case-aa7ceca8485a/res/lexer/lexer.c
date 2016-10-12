#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum token {
  nulsym = 1, identsym, numbersym, plussym, minussym,
  multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
  gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
  periodsym, becomessym, beginsym, endsym, ifsym, thensym,
  whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
  readsym , elsesym
} token_type;

typedef struct Symbol {
    char *identifier;
    int type;
} Symbol;

void printSource(FILE *source, int mirror, int cleanMirror);
void printTokens(FILE *source);
Symbol *nextSymbol(FILE *source);
token_type identifyType(char identifier[100]);

int inComment=0;
char previousC=' ';

int main(int argc, char *argv[]){

    FILE *source = NULL;
    int i=0;
    int mirror=0; //Boolean print out source
    int cleanMirror=0; //Boolean print out cleaned up source

    //No file
    if(argc < 2){
        printf("Error: No file specified");
        return 1;
    }

    source = fopen(argv[1],"r");

    //Read in command line arguments
    while(i<argc){
        if(strcmp(argv[i],"--clean")==0)
            cleanMirror=1;

        if(strcmp(argv[i],"--source")==0)
            mirror=1;

        i++;
    } //End while

    printSource(source,mirror,cleanMirror);
    printTokens(source);

    fclose(source);
    return 0;
}

void printTokens(FILE *source){
    Symbol *symbol = NULL;

    printf("tokens:\n");
    printf("-------\n");

    while(symbol==NULL||symbol->type!=nulsym){
        symbol = nextSymbol(source);
        if(symbol->type == nulsym){
            free(symbol->identifier);
            free(symbol);
            continue;
        }
        printf("%s\t\t%d\n",symbol->identifier,symbol->type);
        free(symbol->identifier);
        free(symbol);
    }
}

Symbol *nextSymbol(FILE *source){
    char *identifier = calloc(100, sizeof(char));
    int i = 0;
    char c;
    Symbol *symbol = calloc(1,sizeof(Symbol));

    //Loop for reading through a symbol
    while((c = fgetc(source))!=EOF){

        if(inComment){
            if(c=='/'&&previousC=='*'){
                inComment = 0;
                previousC=' ';
                i=0;
                continue;
            }
            else{
                previousC=c;
                continue;
            }
        }

        if(isalnum(c)&&!inComment){
            if(!isalnum(previousC)&&previousC!=' '&&previousC!='\t'&&previousC!='\n'){
                ungetc(c,source);
                symbol->identifier = identifier;
                symbol->type = identifyType(identifier);
                previousC = c;
                return symbol;
            }

            else{
                identifier[i] = c;
            }
        }

        else if(c == ' '||c == '\t' || c == '\n'){
            if(previousC==' '||previousC=='\t'||previousC=='\n'){
                previousC = c;
                continue;
                }
            symbol->identifier = identifier;
            symbol->type = identifyType(identifier);
            previousC = c;
            return symbol;
        }

        else{
            if(!inComment){
                if(c=='*'&&previousC=='/'){
                    inComment = 1;
                    free(identifier);
                    identifier = calloc(100,sizeof(char));
                    i=0;
                    c=' ';
                }
                else{
                    if(isalnum(previousC)){
                        ungetc(c,source);
                        symbol->identifier = identifier;
                        symbol->type = identifyType(identifier);
                        previousC = c;
                        return symbol;
                    }
                    else{
                        identifier[i]=c;
                    }
                }
            }
        }

        i++;
        if(i>=12){
            printf("Error: identifier is too long");
            exit(0);
        }

        previousC = c;
    }// End while
    if(i > 0){
        symbol ->identifier = identifier;
        symbol->type = identifyType(identifier);
        return symbol;
    }
    else{
        symbol->identifier="NULL";
        symbol->type=nulsym;
        return symbol;
    }
}

token_type identifyType(char identifier[100]){
    token_type type;
    char tmp[1];

    if(strcmp(identifier, "+") == 0)
        return plussym;
    else if(strcmp(identifier, "-") == 0)
        return minussym;
    else if(strcmp(identifier, "*") == 0)
        return multsym;
    else if(strcmp(identifier, "/") == 0)
        return slashsym;
    else if(strcmp(identifier, "=") == 0)
        return eqsym;
    else if(strcmp(identifier, "<>") == 0)
        return neqsym;
    else if(strcmp(identifier, "<=") == 0)
        return leqsym;
    else if(strcmp(identifier, ">=") == 0)
        return geqsym;
    else if(strcmp(identifier, ">") == 0)
        return lessym;
    else if(strcmp(identifier, "<") == 0)
        return gtrsym;
    else if(strcmp(identifier, "(") == 0)
        return lparentsym;
    else if(strcmp(identifier, ")") == 0)
        return rparentsym;
    else if(strcmp(identifier, ",") == 0)
        return commasym;
    else if(strcmp(identifier, ";") == 0)
        return semicolonsym;
    else if(strcmp(identifier, ".") == 0)
        return periodsym;
    else if(strcmp(identifier, ":=") == 0)
        return becomessym;
    else if(strcmp(identifier, "const") == 0)
        return constsym;
    else if(strcmp(identifier, "var") == 0)
        return varsym;
    else if(strcmp(identifier, "procedure") == 0)
        return procsym;
    else if(strcmp(identifier, "call") == 0)
        return callsym;
    else if(strcmp(identifier, "begin") == 0)
        return beginsym;
    else if(strcmp(identifier, "end") == 0)
        return endsym;
    else if(strcmp(identifier, "if") == 0)
        return ifsym;
    else if(strcmp(identifier, "then") == 0)
        return thensym;
    else if(strcmp(identifier, "else") == 0)
        return elsesym;
    else if(strcmp(identifier, "while") == 0)
        return whilesym;
    else if(strcmp(identifier, "do") == 0)
        return dosym;
    else if(strcmp(identifier, "read") == 0)
        return readsym;
    else if(strcmp(identifier, "write") == 0)
        return writesym;
    else if(strcmp(identifier, "odd") == 0)
        return oddsym;

   //if we get here then the identifier is either a number/identifier/bad token

   if(isalpha(identifier[0]))
        return identsym;
   else{
        if(isalpha(identifier[1])){
           printf("Error: identifier does not start with a letter");
           exit(0);
        }
        else{
            if(atoi(identifier) >= 65536){
                printf("Error: number greater than 2^16-1");
                exit(0);
            }
            else
                return numbersym;
        }
    }

    printf("Error: invalid token");
    exit(0);
}

void printSource(FILE *source, int mirror, int cleanMirror){

    char c;

    if(mirror==1){

        printf("source code:\n");
        printf("------------\n");

        while((c = fgetc(source))!=EOF){
            printf("%c",c);
        }//End while

        printf("\n\n");
    }//End mirror

    rewind(source);

    if(cleanMirror==1){

        char previousC=0;
        int inComment=0;

        printf("source code without comments:\n");
        printf("-----------------------------\n");

        while((c=fgetc(source))!=EOF){

            if(!inComment){
                if(c=='*'&&previousC=='/'){
                    inComment = 1;
                    printf(" ");
                }
                else if(previousC!=0)
                    printf("%c",previousC);
            }

            else
                if(c=='/'&&previousC=='*'){
                    printf(" ");
                    inComment = 0;
                    c = ' ';
                }
                else
                    printf(" ");

            previousC = c;
        }//End while

        printf("%c\n\n",previousC);//Print last character
    }//End clean mirror

    rewind(source);
}
