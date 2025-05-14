#include <stdio.h>
#include <stdlib.h> // Para malloc, free, rand, srand
#include <time.h>   // Para clock, time

// Función para asignar memoria para una matriz cuadrada
int **allocateMatrix(int size) {
    if (size < 0) return NULL; // Evitar tamaño negativo
    if (size == 0) { // Permitir matrices de tamaño 0, pero con cuidado
        int **matrix = (int **)malloc(0 * sizeof(int *)); // Podría ser NULL o un puntero válido
        return matrix; // O manejar como un caso especial devolviendo NULL si no se desea
    }

    int **matrix = (int **)malloc(size * sizeof(int *));
    if (matrix == NULL) {
        perror("Error al asignar memoria para punteros de fila");
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        matrix[i] = (int *)malloc(size * sizeof(int));
        if (matrix[i] == NULL) {
            perror("Error al asignar memoria para una fila");
            // Liberar memoria asignada previamente
            for (int k = 0; k < i; k++) {
                free(matrix[k]);
            }
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

// Función para liberar memoria asignada para una matriz cuadrada
void freeMatrix(int size, int **matrix) {
    if (matrix == NULL || size <= 0) return;
    for (int i = 0; i < size; i++) {
        if (matrix[i] != NULL) {
            free(matrix[i]);
        }
    }
    free(matrix);
}

// Función para llenar una matriz cuadrada con números aleatorios (0-9)
void fillRandomMatrix(int size, int **matrix) {
    if (matrix == NULL || size <= 0) return;
    for (int i = 0; i < size; i++) {
        if (matrix[i] == NULL) continue; // Fila no asignada
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 10; // Números aleatorios entre 0 y 9
        }
    }
}

// Función para imprimir una matriz cuadrada
void printMatrix(int size, int **matrix, const char *name) {
    if (matrix == NULL || size <= 0) {
        printf("Matriz %s no es válida o está vacía.\n", name);
        return;
    }
    printf("Matriz %s (%dx%d):\n", name, size, size);
    for (int i = 0; i < size; i++) {
         if (matrix[i] == NULL) {
            printf("Fila %d no asignada.\n", i);
            continue;
        }
        for (int j = 0; j < size; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Algoritmo Clásico (Naive) para multiplicar dos matrices cuadradas
int **naive_multiply(int size, int **matrixA, int **matrixB) {
    if (size == 0) return allocateMatrix(0);
    if (matrixA == NULL || matrixB == NULL || size < 0) return NULL;

    int **matrixC = allocateMatrix(size);
    if (matrixC == NULL) {
        printf("Error: No se pudo asignar memoria para la matriz resultante C (naive_multiply).\n");
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        if (matrixA[i] == NULL || matrixC[i] == NULL) return NULL; // Chequeo adicional
        for (int j = 0; j < size; j++) {
            matrixC[i][j] = 0;
            for (int k = 0; k < size; k++) {
                if (matrixB[k] == NULL) return NULL; // Chequeo adicional
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    return matrixC;
}

int main() {
    int size;
    clock_t startTime, endTime;
    double cpu_time_used;
    unsigned long long memory_used_bytes = 0;

    // Sembrar el generador de números aleatorios una vez
    srand(time(NULL));

    printf("ALGORITMO NAIVE PARA MULTIPLICACIÓN DE MATRICES\n");
    printf("-----------------------------------------------------------\n");
    printf("Ingrese el tamaño N para las matrices cuadradas (NxN): ");
    if (scanf("%d", &size) != 1) {
        printf("Entrada inválida.\n");
        return 1;
    }


    if (size < 0) { // El caso size == 0 se maneja en las funciones
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
        // freeMatrix ya maneja NULL, así que es seguro llamar
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
    int **matrixC = naive_multiply(size, matrixA, matrixB);
    endTime = clock();

    if (matrixC == NULL && size > 0) { // Solo es error si size > 0 y C es NULL
        printf("La multiplicación de matrices (ingenua) falló.\n");
        freeMatrix(size, matrixA);
        freeMatrix(size, matrixB);
        // matrixC ya es NULL o fue manejado por allocateMatrix(0)
        return 1;
    }

    cpu_time_used = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;

    if (size > 0) {
        size_t size_of_pointers_per_matrix = (size_t)size * sizeof(int *);
        size_t size_of_data_per_matrix = (size_t)size * (size_t)size * sizeof(int);
        memory_used_bytes = 3 * (size_of_pointers_per_matrix + size_of_data_per_matrix);
    }


    printf("Multiplicación (naive) completada.\n");
    if (size > 0 && size <= 10) {
        printMatrix(size, matrixC, "Resultante C (A x B)");
    } else if (size > 10) {
        printf("Matriz Resultante C (%dx%d) calculada. No se imprimirá debido a su tamaño.\n\n", size, size);
    }

    printf("--- Métricas de Rendimiento (Algoritmo Ingenuo) ---\n");
    printf("Tiempo de CPU para la multiplicación: %.6f segundos\n", cpu_time_used);
    printf("Memoria estimada utilizada por las matrices A, B y C: %llu bytes (%.2f KB / %.2f MB)\n",
           memory_used_bytes,
           (double)memory_used_bytes / 1024.0,
           (double)memory_used_bytes / (1024.0 * 1024.0));

    freeMatrix(size, matrixA);
    freeMatrix(size, matrixB);
    freeMatrix(size, matrixC); // Seguro incluso si size es 0 y matrixC es de allocateMatrix(0)

    return 0;
}
