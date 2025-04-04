#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 100

typedef struct {
    char* elements[MAX_N];
    int size;
} Set;

void add_to_set(Set* set, const char* element) {
    if (set->size < MAX_N && !contains(set, element)) {
        set->elements[set->size] = strdup(element);
        set->size++;
    }
}

int contains(Set* set, const char* element) {
    for (int i = 0; i < set->size; i++) {
        if (strcmp(set->elements[i], element) == 0) {
            return 1;
        }
    }
    return 0;
}

int cyk(const char* comienzo, const char* string) {
    int n = strlen(string);
    if (n == 0 || n % 2 == 0) return 0; // Regla para aceptar solo longitudes impares

    Set table[MAX_N][MAX_N];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            table[i][j].size = 0;

    // Inicialización de la diagonal (símbolos terminales)
    for (int i = 0; i < n; i++) {
        if (string[i] == 'a') {
            add_to_set(&table[i][i], "A");
        } else if (string[i] == 'b') {
            add_to_set(&table[i][i], "B");
        }
    }

    // Algoritmo CYK: llenar la tabla
    for (int length = 2; length <= n; length++) {
        for (int i = 0; i < n - length + 1; i++) {
            int j = i + length - 1;

            for (int k = i; k < j; k++) {
                // Aplicar las reglas correctamente
                if (contains(&table[i][k], "A") && contains(&table[k + 1][j], "B")) {
                    add_to_set(&table[i][j], "S");
                }
                if (contains(&table[i][k], "B") && contains(&table[k + 1][j], "A")) {
                    add_to_set(&table[i][j], "S");
                }
                if (contains(&table[i][k], "A") && contains(&table[k + 1][j], "S")) {
                    add_to_set(&table[i][j], "S");
                }
                if (contains(&table[i][k], "B") && contains(&table[k + 1][j], "S")) {
                    add_to_set(&table[i][j], "S");
                }
                if (contains(&table[i][k], "S") && contains(&table[k + 1][j], "A")) {
                    add_to_set(&table[i][j], "S");
                }
                if (contains(&table[i][k], "S") && contains(&table[k + 1][j], "B")) {
                    add_to_set(&table[i][j], "S");
                }
                if (contains(&table[i][k], "S") && contains(&table[k + 1][j], "S")) {
                    add_to_set(&table[i][j], "S");
                }
            }
        }
    }

    return contains(&table[0][n - 1], comienzo);
}

int main() {
    char* ejemplos[] = { "a", "b", "ab", "ba", "aab", "abb", "aaa", "aba", "bab", "bba" };
    int num_ejemplos = 10;

    for (int i = 0; i < num_ejemplos; i++) {
        int resultado = cyk("S", ejemplos[i]);
        printf("Resultado para '%s': %s\n", ejemplos[i], resultado ? "Sí" : "No");
    }

    return 0;
}
