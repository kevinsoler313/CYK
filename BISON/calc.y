%{
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
%}

%token A B

%%
S : A B A { printf("Cadena aceptada\n"); }
  ;
%%

int yyerror(const char* s) {
    printf("Cadena no aceptada\n");
    return 0;
}

