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
    token_type type;
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
        printf("No file specified");
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
        if(i>=100){
            printf("Error: Token too long");
        }

        previousC = c;
    }// End while
    symbol->identifier="NULL";
    symbol->type=nulsym;
    return symbol;
}

token_type identifyType(char identifier[100]){
    return 1;
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
