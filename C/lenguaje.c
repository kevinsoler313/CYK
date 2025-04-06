#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_N 100

typedef struct {
    char* elements[MAX_N];
    int size;
} Set;

int contains(Set* set, const char* element) {
    for (int i = 0; i < set->size; i++) {
        if (strcmp(set->elements[i], element) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_to_set(Set* set, const char* element) {
    if (set->size < MAX_N && !contains(set, element)) {
        set->elements[set->size] = strdup(element);
        set->size++;
    }
}

int cyk(const char* comienzo, const char* string) {
    int n = strlen(string);
    if (n == 0) return 0;

    Set table[MAX_N][MAX_N];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            table[i][j].size = 0;

    for (int i = 0; i < n; i++) {
        if (string[i] == 'a') {
            add_to_set(&table[i][i], "A");
            add_to_set(&table[i][i], "D");
        }
        if (string[i] == 'b') {
            add_to_set(&table[i][i], "C");
        }
    }

    for (int length = 2; length <= n; length++) {
        for (int i = 0; i <= n - length; i++) {
            int j = i + length - 1;
            for (int k = i; k < j; k++) {
                if (contains(&table[i][k], "A") && contains(&table[k+1][j], "B"))
                    add_to_set(&table[i][j], "S");

                if (contains(&table[i][k], "C") && contains(&table[k+1][j], "D"))
                    add_to_set(&table[i][j], "B");
            }
        }
    }

    return contains(&table[0][n-1], comienzo);
}

int main() {
    char* ejemplos[] = { "aba", "a", "b", "ab", "aab", "aba", "aaa", "abc", "bab", "abaa", "ba", "aaab" };
    int num_ejemplos = sizeof(ejemplos) / sizeof(ejemplos[0]);

    FILE* file = fopen("tiempos_cyk.txt", "w");
    if (!file) {
        perror("No se pudo abrir el archivo de salida");
        return 1;
    }

    for (int i = 0; i < num_ejemplos; i++) {
        clock_t start = clock();
        int resultado = cyk("S", ejemplos[i]);
        clock_t end = clock();
        double tiempo = (double)(end - start) / CLOCKS_PER_SEC * 1000;

        printf("Resultado para '%s': %s (%.4f ms)\n", ejemplos[i], resultado ? "Sí" : "No", tiempo);
        fprintf(file, "%d %.4f\n", i + 1, tiempo);
    }

    fclose(file);

    // Automatizar gráfico con Gnuplot
    FILE* gnuplot = popen("gnuplot -persistent", "w");
    if (gnuplot) {
        fprintf(gnuplot, "set title 'Tiempos de ejecución - CYK'\n");
        fprintf(gnuplot, "set xlabel 'Ejemplo'\n");
        fprintf(gnuplot, "set ylabel 'Tiempo (ms)'\n");
        fprintf(gnuplot, "set grid\n");
        fprintf(gnuplot, "plot 'tiempos_cyk.txt' using 1:2 with linespoints title 'CYK'\n");
        pclose(gnuplot);
    }

    return 0;
}

