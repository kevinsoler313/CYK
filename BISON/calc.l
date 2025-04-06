%{
#include "calc.tab.h"
%}

%%
a   { return A; }
b   { return B; }
.   { /* Ignorar cualquier otro carácter */ }
\n  { return 0; }
%%

int yywrap() {
    return 1;
}

