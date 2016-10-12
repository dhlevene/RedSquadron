#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "lex.h"
#include "parser.h"

extern LVAL lval;

token_type tok;

void advance() { 
  tok=lex(); 
}

void eat(token_type t) { 
  if (tok==t) {
    advance();
  } else {
    printf("error\n");
    exit(1);
  }
}

void S(void) {
  switch (tok) {
  case idsym :
  case numsym :
  case lparensym :
    printf("S  -> E $\n");
    E();
    eat(nulsym);
    break;
  default :
    printf("error\n");
    exit(1);
  }
}

// E -> T E'
void E(void) {
  switch (tok) {
  case idsym :
  case numsym :
  case lparensym :
    printf("E  -> T E'\n");
    T();
    Eprime();
    break;
  default :
    printf("error\n");
    exit(1);
  }  
}

// E' -> + T E' | epsilon
void Eprime(void) {
  switch (tok) {
  case plussym :
    printf("E' -> + T E'\n");
    eat(plussym);
    T();
    Eprime();
    break;
  case rparensym :
  case nulsym :
    printf("E  -> epsilon\n");
    break;
  default :
    printf("error\n");
    exit(1);
  }
}

// T -> F T'
void T(void) {
  switch (tok) {
  case idsym :
  case numsym :
  case lparensym : 
    printf("T  -> F T'\n");
    F();
    Tprime();
    break;
  default : 
    printf("error\n");
    exit(1);
  }
}

// T' -> * F T' | epsilon
void Tprime(void) {
  switch (tok) {
  case plussym: 
    printf("T' -> epsilon\n");
    break;
  case multsym:
    printf("T' -> * F T'\n");
    eat(multsym);
    F();
    Tprime();
    break;
  case rparensym : 
  case nulsym : 
    printf("T' -> epsilon\n");
    break;
  default :
    printf("error\n");
    exit(1);
  }
}

// F -> ( E ) | id | num
void F(void) {
  switch (tok) {
  case idsym :
    //printf("F  -> id\n");
    printf("F  -> %s\n", lval.id);
    eat(idsym);
    break;
  case numsym :
    //printf("F  -> num\n");
    printf("F  -> %d\n", lval.num);
    eat(numsym);
    break;
  case lparensym :
    printf("F  -> ( E )\n");
    eat(lparensym);
    E();
    eat(rparensym);
    break;
  default :
    printf("error\n");
    exit(1);
  } // switch
}

main() {
  printf("recursive descent parser based on predictive table\n");
  advance();
  S();
}
