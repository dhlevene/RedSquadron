#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "tokens.h"

extern YYSTYPE yylval;
extern FILE *yyin;

int main(int argc, char *argv[]) {

  int tok;
  
  if (argc != 2) {
    fprintf(stderr, "usage: ./lextest filename\n"); 
    exit(1);
  }

  yyin = fopen(argv[1], "r");
  if (!yyin) {
    printf("cannot file open %s", argv[1]); 
    exit(1);
  }

  for(;;) {
    tok = yylex();
    if (tok == 0) break;
    printf("(%2d,%2d) %-12s %d", yylval.line, yylval.col, yylval.text, tok);
    switch (tok) {
    case identsym :
      printf("    %s\n", yylval.val.ident);
      break;
    case numbersym :
      printf("    %d\n", yylval.val.number);
      break;
    default :
      printf("\n");
    }
  }
  return 0;
}


