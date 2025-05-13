#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#define STRASSEN_THRESHOLD 64 // Umbral para cambiar a algoritmo ingenuo

using namespace std;
using namespace chrono;

// --- Funciones Auxiliares (Comunes) ---
using Matrix = vector<vector<int>>;

Matrix allocateMatrix(int size) {
    if (size < 0) return {};
    return Matrix(size, vector<int>(size, 0)); // Inicializa una matriz NxN con 0
}

void fillRandomMatrix(int size, Matrix& matrix) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 9); // Números aleatorios entre 0 y 9

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = dis(gen);
        }
    }
}

void printMatrix(int size, const Matrix& matrix, const string& name) {
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

// --- Funciones Específicas de Strassen ---

void addMatrices(int size, const Matrix& A, const Matrix& B, Matrix& C) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void subtractMatrices(int size, const Matrix& A, const Matrix& B, Matrix& C) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

Matrix naive_multiply_strassen_base(int size, const Matrix& A, const Matrix& B) {
    Matrix C(size, vector<int>(size, 0));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

Matrix strassen_multiply_internal(int size, const Matrix& A, const Matrix& B) {
    if (size <= STRASSEN_THRESHOLD) {
        return naive_multiply_strassen_base(size, A, B);
    }

    int newSize = size / 2;

    // Submatrices
    Matrix A11 = allocateMatrix(newSize), A12 = allocateMatrix(newSize);
    Matrix A21 = allocateMatrix(newSize), A22 = allocateMatrix(newSize);
    Matrix B11 = allocateMatrix(newSize), B12 = allocateMatrix(newSize);
    Matrix B21 = allocateMatrix(newSize), B22 = allocateMatrix(newSize);

    // Temporary matrices
    Matrix tempA = allocateMatrix(newSize), tempB = allocateMatrix(newSize);

    // Divide matrices A and B into submatrices
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            A11[i][j] = A[i][j]; A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j]; A22[i][j] = A[i + newSize][j + newSize];
            B11[i][j] = B[i][j]; B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j]; B22[i][j] = B[i + newSize][j + newSize];
        }
    }

    // P1 = (A11 + A22) * (B11 + B22)
    addMatrices(newSize, A11, A22, tempA);
    addMatrices(newSize, B11, B22, tempB);
    Matrix P1 = strassen_multiply_internal(newSize, tempA, tempB);

    // P2 = (A21 + A22) * B11
    addMatrices(newSize, A21, A22, tempA);
    Matrix P2 = strassen_multiply_internal(newSize, tempA, B11);

    // P3 = A11 * (B12 - B22)
    subtractMatrices(newSize, B12, B22, tempB);
    Matrix P3 = strassen_multiply_internal(newSize, A11, tempB);

    // P4 = A22 * (B21 - B11)
    subtractMatrices(newSize, B21, B11, tempB);
    Matrix P4 = strassen_multiply_internal(newSize, A22, tempB);

    // P5 = (A11 + A12) * B22
    addMatrices(newSize, A11, A12, tempA);
    Matrix P5 = strassen_multiply_internal(newSize, tempA, B22);

    // P6 = (A21 - A11) * (B11 + B12)
    subtractMatrices(newSize, A21, A11, tempA);
    addMatrices(newSize, B11, B12, tempB);
    Matrix P6 = strassen_multiply_internal(newSize, tempA, tempB);

    // P7 = (A12 - A22) * (B21 + B22)
    subtractMatrices(newSize, A12, A22, tempA);
    addMatrices(newSize, B21, B22, tempB);
    Matrix P7 = strassen_multiply_internal(newSize, tempA, tempB);

    // C11 = P1 + P4 - P5 + P7
    Matrix C11 = allocateMatrix(newSize);
    addMatrices(newSize, P1, P4, C11);
    subtractMatrices(newSize, C11, P5, C11);
    addMatrices(newSize, C11, P7, C11);

    // C12 = P3 + P5
    Matrix C12 = allocateMatrix(newSize);
    addMatrices(newSize, P3, P5, C12);

    // C21 = P2 + P4
    Matrix C21 = allocateMatrix(newSize);
    addMatrices(newSize, P2, P4, C21);

    // C22 = P1 - P2 + P3 + P6
    Matrix C22 = allocateMatrix(newSize);
    subtractMatrices(newSize, P1, P2, C22);
    addMatrices(newSize, C22, P3, C22);
    addMatrices(newSize, C22, P6, C22);

    // Final Matrix C
    Matrix C(size, vector<int>(size, 0));
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            C[i][j] = C11[i][j];
            C[i][j + newSize] = C12[i][j];
            C[i + newSize][j] = C21[i][j];
            C[i + newSize][j + newSize] = C22[i][j];
        }
    }

    return C;
}

Matrix strassen_multiply(int size, const Matrix& A, const Matrix& B) {
    int new_size = 1;
    while (new_size < size) {
        new_size *= 2;
    }

    Matrix A_padded = allocateMatrix(new_size);
    Matrix B_padded = allocateMatrix(new_size);

    for (int i = 0; i < new_size; i++) {
        for (int j = 0; j < new_size; j++) {
            if (i < size && j < size) {
                A_padded[i][j] = A[i][j];
                B_padded[i][j] = B[i][j];
            } else {
                A_padded[i][j] = 0;
                B_padded[i][j] = 0;
            }
        }
    }

    Matrix C_padded = strassen_multiply_internal(new_size, A_padded, B_padded);

    Matrix C_result = allocateMatrix(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C_result[i][j] = C_padded[i][j];
        }
    }

    return C_result;
}

unsigned long long getMemoryUsage(int size) {
    // Suponiendo que cada valor en la matriz ocupa 4 bytes (int)
    return sizeof(int) * size * size * 3; // Tres matrices: A, B y C
}

int main() {
    int size;
    cout << "ALGORITMO DE STRASSEN PARA MULTIPLICACIÓN DE MATRICES\n";
    cout << "-------------------------------------------------------\n";
    cout << "Ingrese el tamaño N para las matrices cuadradas (NxN): ";
    cin >> size;

    if (size <= 0) {
        cout << "Tamaño no válido.\n";
        return 1;
    }

    Matrix A = allocateMatrix(size);
    Matrix B = allocateMatrix(size);

    fillRandomMatrix(size, A);
    fillRandomMatrix(size, B);

    auto start = high_resolution_clock::now();
    Matrix C = strassen_multiply(size, A, B);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Tiempo de ejecución (Strassen): " << duration.count() << " ms\n";

    // Calcular la memoria utilizada
    unsigned long long memory_used_bytes = getMemoryUsage(size);
    cout << "Memoria utilizada: " << memory_used_bytes << " bytes (" 
         << (double)memory_used_bytes / 1024.0 << " KB / "
         << (double)memory_used_bytes / (1024.0 * 1024.0) << " MB)" << endl;

    return 0;
}
