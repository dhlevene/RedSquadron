#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "lex.h"
#include "parser.h"

extern LVAL lval;

token_type tok;

void advance() { tok=lex(); }

void S(void) {
  message("S  -> E $");
  E();
  if (tok==nulsym) {
    ;
  }
  else {
    error("$ expected\n");
    exit(1);
  }
}

// E -> T E'
void E(void) {
  message("E  -> T E'");
  T();
  Eprime();
}

// E' -> + T E' 
// E' -> epsilon
void Eprime(void) {
  while (tok==plussym) {
    message("E' -> + T E'");
    advance();
    T();
  }
  message("E' -> epsilon");
}

// T -> F T'
void T(void) {
  message("T  -> F T'");
  F();
  Tprime();
}

// T' -> * F T' 
// T' -> epsilon
void Tprime(void) {
  while (tok==multsym) {
    message("T' -> * F T'");
    advance();
    F();
  }
  message("T' -> epsilon");
}

// F -> ( E ) 
// F -> id 
// F -> num
void F(void) {
  switch (tok) {
  case lparensym :
    message("F  -> ( E )");
    advance();
    E();
    if (tok==rparensym) {
      advance();
    } 
    else {
      error(") expected\n");
      exit(1);
    }
    break;
  case idsym :
    //message("F  -> id");
    printf("F  -> %s\n", lval.id);
    advance();
    break;
  case numsym :
    //message("F  -> num");
    printf("F  -> %d\n", lval.num);
    advance();
    break;
  default :
    error("(, identifier or number expected\n");
    exit(1);
  } 
}

main() {
  printf("recursive descent parser based on syntax graph\n");
  advance();
  S();
}
