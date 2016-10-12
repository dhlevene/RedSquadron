#include <stdio.h>
#include "tokens.h"
#include "lex.h"

extern LVAL lval;

main() {
  printf("lexer for simple arithmetic expressions\n");
  token_type tok;
  while ((tok=lex()) != nulsym) {
    switch (tok) {
    case -1 :
      printf("illegal token\n");
      continue;
    case idsym : 
      printf("type: %d, value: %s\n", tok, lval.id);
      break;
    case numsym : 
      printf("type: %d, value: %d\n", tok, lval.num);
      break;
    default :
      printf("type: %d\n", tok);
    }
  }
}
