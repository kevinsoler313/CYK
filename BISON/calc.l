%{
#include "calc.tab.h"
#include <stdlib.h>
%}

%%

[0-9]+(\.[0-9]+)?  { yylval.num = atof(yytext); return NUM; }
[+\-*/()]          { return yytext[0]; }
\n                 { return '\n'; }
[ \t]              { /* Ignorar espacios en blanco */ }
.                  { printf("Caracter no reconocido: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}
