#include <iostream>
#include <vector>
#include <cstdlib> // Para rand, srand
#include <ctime>   // Para clock, time
#include <chrono>  // Para medir el tiempo en C++

using namespace std;

// Función para asignar memoria para una matriz cuadrada usando std::vector
vector<vector<int>> allocateMatrix(int size) {
    if (size < 0) return {}; // Evitar tamaño negativo
    return vector<vector<int>>(size, vector<int>(size, 0)); // Inicializa una matriz NxN con 0
}

// Función para llenar una matriz cuadrada con números aleatorios (0-9)
void fillRandomMatrix(int size, vector<vector<int>>& matrix) {
    if (size <= 0) return;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 10; // Números aleatorios entre 0 y 9
        }
    }
}

// Función para imprimir una matriz cuadrada
void printMatrix(int size, const vector<vector<int>>& matrix, const string& name) {
    if (size <= 0) {
        cout << "Matriz " << name << " no es válida o está vacía.\n";
        return;
    }
    cout << "Matriz " << name << " (" << size << "x" << size << "):\n";
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

// Algoritmo Clásico (Naive) para multiplicar dos matrices cuadradas
vector<vector<int>> naive_multiply(int size, const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB) {
    if (size == 0) return allocateMatrix(0);
    vector<vector<int>> matrixC = allocateMatrix(size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrixC[i][j] = 0;
            for (int k = 0; k < size; k++) {
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    return matrixC;
}

int main() {
    int size;
    srand(time(NULL)); // Sembrar el generador de números aleatorios

    cout << "ALGORITMO NAIVE PARA MULTIPLICACIÓN DE MATRICES\n";
    cout << "-----------------------------------------------------------\n";
    cout << "Ingrese el tamaño N para las matrices cuadradas (NxN): ";
    if (!(cin >> size)) {
        cout << "Entrada inválida.\n";
        return 1;
    }

    if (size < 0) { // El caso size == 0 se maneja en las funciones
        cout << "El tamaño de la matriz no puede ser negativo.\n";
        return 1;
    }

    if (size == 0) {
        cout << "Se solicitó un tamaño de matriz de 0. No se realizarán operaciones.\n";
        cout << "Tiempo de CPU para la multiplicación: 0.000000 segundos\n";
        cout << "Memoria estimada utilizada por las matrices A, B y C: 0 bytes (0.00 KB / 0.00 MB)\n";
        return 0;
    }

    // Asignar matrices A y B
    vector<vector<int>> matrixA = allocateMatrix(size);
    vector<vector<int>> matrixB = allocateMatrix(size);

    // Llenar las matrices con valores aleatorios
    fillRandomMatrix(size, matrixA);
    fillRandomMatrix(size, matrixB);

    // Mostrar las matrices si el tamaño es menor o igual a 10
    if (size <= 10) {
        printMatrix(size, matrixA, "A");
        printMatrix(size, matrixB, "B");
    } else {
        cout << "Matrices A y B generadas (" << size << "x" << size << "). No se imprimirán debido a su tamaño.\n\n";
    }

    // Medir el tiempo de ejecución
    auto startTime = chrono::high_resolution_clock::now();
    vector<vector<int>> matrixC = naive_multiply(size, matrixA, matrixB);
    auto endTime = chrono::high_resolution_clock::now();

    // Calcular el tiempo de ejecución
    chrono::duration<double> cpu_time_used = endTime - startTime;

    // Calcular la memoria utilizada (en bytes)
    size_t memory_used_bytes = 3 * (size * sizeof(int*) + size * size * sizeof(int));

    // Mostrar el resultado
    cout << "Multiplicación (naive) completada.\n";
    if (size <= 10) {
        printMatrix(size, matrixC, "Resultante C (A x B)");
    } else {
        cout << "Matriz Resultante C (" << size << "x" << size << ") calculada. No se imprimirá debido a su tamaño.\n\n";
    }

    cout << "--- Métricas de Rendimiento (Algoritmo Ingenuo) ---\n";
    cout << "Tiempo de CPU para la multiplicación: " << cpu_time_used.count() << " segundos\n";
    cout << "Memoria estimada utilizada por las matrices A, B y C: " << memory_used_bytes << " bytes ("
         << (double)memory_used_bytes / 1024.0 << " KB / " << (double)memory_used_bytes / (1024.0 * 1024.0) << " MB)\n";

    return 0;
}
