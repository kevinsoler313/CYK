%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern int yylex();
extern int yyparse();
extern FILE *yyin;
void yyerror(const char *s);

clock_t start_time, end_time;
double execution_time;
double resultado;

%}

%union {
    double num;
}

%token <num> NUM
%left '+' '-'
%left '*' '/'
%right UMINUS

%type <num> expr

%%

expr:
    expr '+' expr { $$ = $1 + $3; }
  | expr '-' expr { $$ = $1 - $3; }
  | expr '*' expr { $$ = $1 * $3; }
  | expr '/' expr { $$ = $1 / $3; }
  | '-' expr %prec UMINUS { $$ = -$2; }
  | '(' expr ')' { $$ = $2; }
  | NUM { $$ = $1; }
  ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    FILE *file = fopen("bison_tiempos.txt", "w");
    if (!file) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Ejemplos de expresiones matemáticas
    char *ejemplos[] = {"3 + 5", "7 - 2 * 3", "(4 + 5) * 2", "10 / 2 + 3", "8 * (3 + 2) - 4"};
    int num_ejemplos = 5;

    for (int i = 0; i < num_ejemplos; i++) {
        yyin = fmemopen(ejemplos[i], strlen(ejemplos[i]), "r");
        if (!yyin) {
            perror("Error al abrir flujo de memoria");
            continue;
        }

        start_time = clock();
        yyparse();
        end_time = clock();
        
        execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
        fprintf(file, "%d %.4f\n", i + 1, execution_time);
        fclose(yyin);
    }

    fclose(file);

    // Llamar a Gnuplot para graficar los tiempos
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (gnuplot) {
        fprintf(gnuplot, "set title 'Tiempos de Ejecución - Bison'\n");
        fprintf(gnuplot, "set xlabel 'Expresiones'\n");
        fprintf(gnuplot, "set ylabel 'Tiempo (ms)'\n");
        fprintf(gnuplot, "set grid\n");
        fprintf(gnuplot, "plot 'bison_tiempos.txt' using 1:2 with linespoints title 'Bison'\n");
        pclose(gnuplot);
    } else {
        perror("No se pudo abrir Gnuplot");
    }

    return 0;
}
