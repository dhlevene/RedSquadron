#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum token {
  nulsym = 1, identsym, numbersym, plussym, minussym,
  multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
  gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
  periodsym, becomessym, beginsym, endsym, ifsym, thensym,
  whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
  readsym , elsesym
} token_type;

#define MAX_SYMBOL_TABLE_SIZE 100

typedef struct variableSymbol{
    int kind;       //const=1 var=2, proc=3
    char name[12];  //Up to 11 chars
    int val;        //number ascii value
    int level;      //L level
    int addr;       //M address
} variableSymbol;

typedef struct Symbol {
    char *identifier;
    int type;
} Symbol;


void printSource(FILE *source, int mirror, int cleanMirror);
void printTokens(FILE *source);
Symbol *nextSymbol(FILE *source);
token_type identifyType(char identifier[100]);
