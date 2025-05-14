#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define STRASSEN_THRESHOLD 64 // Umbral para cambiar a algoritmo ingenuo

// --- Funciones Auxiliares (Comunes) ---
int **allocateMatrix(int size) {
    if (size < 0) return NULL;
    if (size == 0) {
        return (int **)malloc(0 * sizeof(int *)); // Permitir, malloc(0) es válido
    }
    int **matrix = (int **)malloc(size * sizeof(int *));
    if (matrix == NULL) {
        perror("allocateMatrix: Error malloc para punteros de fila");
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        matrix[i] = (int *)malloc(size * sizeof(int));
        if (matrix[i] == NULL) {
            perror("allocateMatrix: Error malloc para fila");
            for (int k = 0; k < i; k++) free(matrix[k]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void freeMatrix(int size, int **matrix) {
    if (matrix == NULL || size <=0) return;
    for (int i = 0; i < size; i++) {
        if (matrix[i] != NULL) free(matrix[i]);
    }
    free(matrix);
}

void fillRandomMatrix(int size, int **matrix) {
    if (matrix == NULL || size <= 0) return;
    for (int i = 0; i < size; i++) {
        if(matrix[i] == NULL) continue;
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void printMatrix(int size, int **matrix, const char *name) {
    if (matrix == NULL || size <= 0) {
        printf("Matriz %s no es válida o está vacía.\n", name);
        return;
    }
    printf("Matriz %s (%dx%d):\n", name, size, size);
    for (int i = 0; i < size; i++) {
        if(matrix[i] == NULL) { printf("Fila %d no asignada.\n", i); continue;}
        for (int j = 0; j < size; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// --- Funciones Específicas de Strassen ---

void addMatrices(int size, int **A, int **B, int **C) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void subtractMatrices(int size, int **A, int **B, int **C) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

int **naive_multiply_strassen_base(int size, int **A, int **B) {
    if (size == 0) return allocateMatrix(0);
    int **C = allocateMatrix(size);
    if (!C) return NULL;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = 0;
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

int **strassen_recursive(int size, int **A, int **B); // Declaración adelantada

int **strassen_multiply_internal(int size, int **A, int **B) {
    if (size <= STRASSEN_THRESHOLD) {
        return naive_multiply_strassen_base(size, A, B);
    }

    int newSize = size / 2;
    // No es necesario crear C aquí, se crea al final a partir de los cuadrantes.

    int **A11 = allocateMatrix(newSize); int **A12 = allocateMatrix(newSize);
    int **A21 = allocateMatrix(newSize); int **A22 = allocateMatrix(newSize);
    int **B11 = allocateMatrix(newSize); int **B12 = allocateMatrix(newSize);
    int **B21 = allocateMatrix(newSize); int **B22 = allocateMatrix(newSize);

    int **P1, **P2, **P3, **P4, **P5, **P6, **P7;
    int **tempA = allocateMatrix(newSize);
    int **tempB = allocateMatrix(newSize);

    // Lista de todas las matrices temporales para facilitar la liberación en caso de error
    int **all_temps[] = {A11, A12, A21, A22, B11, B12, B21, B22, tempA, tempB};
    int num_all_temps = sizeof(all_temps) / sizeof(all_temps[0]);

    for(int i=0; i < num_all_temps; ++i) {
        if (all_temps[i] == NULL) {
            fprintf(stderr, "Error asignando submatrices/temporales en Strassen.\n");
            for(int j=0; j < num_all_temps; ++j) freeMatrix(newSize, all_temps[j]); // Liberar lo que se pudo
            return NULL;
        }
    }

    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            A11[i][j] = A[i][j]; A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j]; A22[i][j] = A[i + newSize][j + newSize];
            B11[i][j] = B[i][j]; B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j]; B22[i][j] = B[i + newSize][j + newSize];
        }
    }

    addMatrices(newSize, A11, A22, tempA); addMatrices(newSize, B11, B22, tempB);
    P1 = strassen_multiply_internal(newSize, tempA, tempB);
    addMatrices(newSize, A21, A22, tempA);
    P2 = strassen_multiply_internal(newSize, tempA, B11);
    subtractMatrices(newSize, B12, B22, tempB);
    P3 = strassen_multiply_internal(newSize, A11, tempB);
    subtractMatrices(newSize, B21, B11, tempB);
    P4 = strassen_multiply_internal(newSize, A22, tempB);
    addMatrices(newSize, A11, A12, tempA);
    P5 = strassen_multiply_internal(newSize, tempA, B22);
    subtractMatrices(newSize, A21, A11, tempA); addMatrices(newSize, B11, B12, tempB);
    P6 = strassen_multiply_internal(newSize, tempA, tempB);
    subtractMatrices(newSize, A12, A22, tempA); addMatrices(newSize, B21, B22, tempB);
    P7 = strassen_multiply_internal(newSize, tempA, tempB);

    int **all_Ps[] = {P1, P2, P3, P4, P5, P6, P7};
    int num_all_Ps = sizeof(all_Ps) / sizeof(all_Ps[0]);
    for(int i=0; i < num_all_Ps; ++i) {
        if (all_Ps[i] == NULL) {
            fprintf(stderr, "Error en una llamada recursiva de Strassen (P%d es NULL).\n", i+1);
            for(int j=0; j < num_all_temps; ++j) freeMatrix(newSize, all_temps[j]);
            for(int k=0; k < num_all_Ps; ++k) freeMatrix(newSize, all_Ps[k]); // Liberar los P que se crearon
            return NULL;
        }
    }
    
    int **C11 = allocateMatrix(newSize); int **C12 = allocateMatrix(newSize);
    int **C21 = allocateMatrix(newSize); int **C22 = allocateMatrix(newSize);
    int **all_Cs_sub[] = {C11, C12, C21, C22};
    int num_all_Cs_sub = sizeof(all_Cs_sub) / sizeof(all_Cs_sub[0]);

    for(int i=0; i < num_all_Cs_sub; ++i) {
        if (all_Cs_sub[i] == NULL) {
            fprintf(stderr, "Error asignando submatrices de C en Strassen.\n");
            for(int j=0; j < num_all_temps; ++j) freeMatrix(newSize, all_temps[j]);
            for(int k=0; k < num_all_Ps; ++k) freeMatrix(newSize, all_Ps[k]);
            for(int l=0; l < num_all_Cs_sub; ++l) freeMatrix(newSize, all_Cs_sub[l]);
            return NULL;
        }
    }

    // C11 = P1 + P4 - P5 + P7
    addMatrices(newSize, P1, P4, tempA);         // tempA = P1+P4
    subtractMatrices(newSize, tempA, P5, tempB); // tempB = P1+P4-P5
    addMatrices(newSize, tempB, P7, C11);        // C11 = P1+P4-P5+P7
    // C12 = P3 + P5
    addMatrices(newSize, P3, P5, C12);
    // C21 = P2 + P4
    addMatrices(newSize, P2, P4, C21);
    // C22 = P1 - P2 + P3 + P6
    subtractMatrices(newSize, P1, P2, tempA);    // tempA = P1-P2
    addMatrices(newSize, tempA, P3, tempB);      // tempB = P1-P2+P3
    addMatrices(newSize, tempB, P6, C22);        // C22 = P1-P2+P3+P6

    int **C_final = allocateMatrix(size);
    if (C_final == NULL) {
         fprintf(stderr, "Error asignando matriz C_final en Strassen.\n");
         // Liberar todo lo anterior
            for(int j=0; j < num_all_temps; ++j) freeMatrix(newSize, all_temps[j]);
            for(int k=0; k < num_all_Ps; ++k) freeMatrix(newSize, all_Ps[k]);
            for(int l=0; l < num_all_Cs_sub; ++l) freeMatrix(newSize, all_Cs_sub[l]);
         return NULL;
    }

    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            C_final[i][j] = C11[i][j];
            C_final[i][j + newSize] = C12[i][j];
            C_final[i + newSize][j] = C21[i][j];
            C_final[i + newSize][j + newSize] = C22[i][j];
        }
    }

    for(int j=0; j < num_all_temps; ++j) freeMatrix(newSize, all_temps[j]);
    for(int k=0; k < num_all_Ps; ++k) freeMatrix(newSize, all_Ps[k]);
    for(int l=0; l < num_all_Cs_sub; ++l) freeMatrix(newSize, all_Cs_sub[l]);
    
    return C_final;
}

int **strassen_multiply(int original_size, int **A_orig, int **B_orig) {
    if (original_size == 0) return allocateMatrix(0);
    if (A_orig == NULL || B_orig == NULL || original_size < 0) return NULL;

    int n = original_size;
    int new_size = 1;
    while(new_size < n) {
        new_size *= 2; // Siguiente potencia de 2
    }

    int **A_padded = A_orig;
    int **B_padded = B_orig;

    if (new_size != n) {
        A_padded = allocateMatrix(new_size);
        B_padded = allocateMatrix(new_size);
        if (!A_padded || !B_padded) {
            fprintf(stderr, "Error asignando matrices para padding.\n");
            freeMatrix(new_size, A_padded); freeMatrix(new_size, B_padded);
            return NULL;
        }
        for (int i = 0; i < new_size; i++) {
            for (int j = 0; j < new_size; j++) {
                if (i < n && j < n) {
                    A_padded[i][j] = A_orig[i][j];
                    B_padded[i][j] = B_orig[i][j];
                } else {
                    A_padded[i][j] = 0; B_padded[i][j] = 0;
                }
            }
        }
    }

    int **C_padded = strassen_multiply_internal(new_size, A_padded, B_padded);

    if (new_size != n) { // Si se hizo padding, liberar las matrices padded A y B
        freeMatrix(new_size, A_padded); // Solo si fueron creadas (new_size != n)
        freeMatrix(new_size, B_padded);
    }
    // No liberar A_orig ni B_orig aquí, se hace en main

    if (!C_padded) return NULL;

    if (new_size != n) {
        int **C_result = allocateMatrix(n);
        if (!C_result) {
            fprintf(stderr, "Error asignando matriz resultado final post-padding.\n");
            freeMatrix(new_size, C_padded); return NULL;
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                C_result[i][j] = C_padded[i][j];
            }
        }
        freeMatrix(new_size, C_padded);
        return C_result;
    } else {
        return C_padded;
    }
}

unsigned long long strassen_extra_memory(int size) {
    if (size <= STRASSEN_THRESHOLD) {
        return 0; // No hay recursión, memoria extra = 0
    }

    int newSize = size / 2;
    // Memoria por llamada recursiva (submatrices + temporales)
    unsigned long long mem_per_call = 10 * (newSize * newSize * sizeof(int)); // Aproximación conservadora
    
    // Memoria de las llamadas recursivas (7 multiplicaciones)
    return mem_per_call + 7 * strassen_extra_memory(newSize);
}

int main() {
    int size;
    clock_t startTime, endTime;
    double cpu_time_used;
    unsigned long long memory_used_bytes = 0;

    srand(time(NULL));

    printf("ALGORITMO DE STRASSEN PARA MULTIPLICACIÓN DE MATRICES\n");
    printf("-------------------------------------------------------\n");
    printf("Ingrese el tamaño N para las matrices cuadradas (NxN): ");
    if (scanf("%d", &size) != 1) {
        printf("Entrada inválida.\n");
        return 1;
    }

    if (size < 0) {
        printf("El tamaño de la matriz no puede ser negativo.\n");
        return 1;
    }
    if (size == 0) {
        printf("Se solicitó un tamaño de matriz de 0. No se realizarán operaciones.\n");
        printf("Tiempo de CPU para la multiplicación: 0.000000 segundos\n");
        printf("Memoria estimada utilizada por las matrices A, B y C: 0 bytes (0.00 KB / 0.00 MB)\n");
        return 0;
    }


    int **matrixA = allocateMatrix(size);
    int **matrixB = allocateMatrix(size);

    if (matrixA == NULL || matrixB == NULL) {
        printf("Error fatal: No se pudo asignar memoria para las matrices A o B.\n");
        freeMatrix(size, matrixA);
        freeMatrix(size, matrixB);
        return 1;
    }

    fillRandomMatrix(size, matrixA);
    fillRandomMatrix(size, matrixB);

    if (size <= 10) {
        printMatrix(size, matrixA, "A");
        printMatrix(size, matrixB, "B");
    } else {
        printf("Matrices A y B generadas (%dx%d). No se imprimirán debido a su tamaño.\n\n", size, size);
    }

    startTime = clock();
    int **matrixC = strassen_multiply(size, matrixA, matrixB);
    endTime = clock();

    if (matrixC == NULL && size > 0) {
        printf("La multiplicación de matrices (Strassen) falló.\n");
        freeMatrix(size, matrixA);
        freeMatrix(size, matrixB);
        return 1;
    }

    cpu_time_used = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
    
    if (size > 0) {
        size_t size_of_pointers_per_matrix = (size_t)size * sizeof(int *);
        size_t size_of_data_per_matrix = (size_t)size * (size_t)size * sizeof(int);
        memory_used_bytes = 3 * (size_of_pointers_per_matrix + size_of_data_per_matrix);
        unsigned long long extra_memory = strassen_extra_memory(size);
        memory_used_bytes += extra_memory;
    }

    printf("Multiplicación (Strassen) completada.\n");
    if (size > 0 && size <= 10) {
        printMatrix(size, matrixC, "Resultante C (A x B)");
    } else if (size > 10){
        printf("Matriz Resultante C (%dx%d) calculada. No se imprimirá debido a su tamaño.\n\n", size, size);
    }

    printf("--- Métricas de Rendimiento (Algoritmo de Strassen) ---\n");
    printf("Tiempo de CPU para la multiplicación: %.6f segundos\n", cpu_time_used);
    printf("Memoria estimada utilizada por las matrices A, B y C (principales): %llu bytes (%.2f KB / %.2f MB)\n",
           memory_used_bytes,
           (double)memory_used_bytes / 1024.0,
           (double)memory_used_bytes / (1024.0 * 1024.0));
   
    freeMatrix(size, matrixA);
    freeMatrix(size, matrixB);
    freeMatrix(size, matrixC);

    return 0;
}