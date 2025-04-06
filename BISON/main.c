#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern int yyparse();
extern void yy_scan_string(const char*);

int main() {
    char* cadenas[] = { "aba", "a", "b", "ab", "aab", "aba", "aaa", "abc", "bab" };
    int num = sizeof(cadenas) / sizeof(cadenas[0]);

    FILE* file = fopen("tiempos_bison.txt", "w");
    if (!file) {
        perror("No se pudo abrir el archivo");
        return 1;
    }

    for (int i = 0; i < num; i++) {
        clock_t start = clock();
        yy_scan_string(cadenas[i]);
        printf("Cadena '%s': ", cadenas[i]);
        yyparse();
        clock_t end = clock();

        double tiempo = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
        fprintf(file, "%d %.4f\n", i + 1, tiempo);
    }

    fclose(file);
    return 0;
}

