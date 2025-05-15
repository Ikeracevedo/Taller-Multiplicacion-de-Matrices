import java.util.Random;

public class ArquiEjercicio {
    final static int m = 128;
    static int[][] A = new int[m][m];
    static int[][] B = new int[m][m];
    static int[][] C = new int[m][m];

    // Rellenar matrices con números aleatorios entre 0 y 9
    public static void fillMatrices() {
        Random rand = new Random();
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                A[i][j] = rand.nextInt(10); // entre 0 y 9
                B[i][j] = rand.nextInt(10);
            }
        }
    }


    public static int[][] blockedMultiply(int[][] A, int[][] B, int blockSize) {
        int m = A.length;
        int p = A[0].length;
        int n = B[0].length;

        if (B.length != p) {
            throw new IllegalArgumentException("Incompatibilidad: A es m×p y B debe ser p×n");
        }

        int[][] C = new int[m][n];

        for (int ii = 0; ii < m; ii += blockSize) {
            for (int jj = 0; jj < n; jj += blockSize) {
                for (int kk = 0; kk < p; kk += blockSize) {
                    int iMax = Math.min(ii + blockSize, m);
                    int jMax = Math.min(jj + blockSize, n);
                    int kMax = Math.min(kk + blockSize, p);

                    for (int i = ii; i < iMax; i++) {
                        for (int j = jj; j < jMax; j++) {
                            int sum = C[i][j];
                            for (int k = kk; k < kMax; k++) {
                                sum += A[i][k] * B[k][j];
                            }
                            C[i][j] = sum;
                        }
                    }
                }
            }
        }

        return C;
    }

    public static void measurePerformance(Runnable task) {
        System.gc();
        Runtime runtime = Runtime.getRuntime();
        long memoryBefore = runtime.totalMemory() - runtime.freeMemory();

        long startTime = System.nanoTime();

        task.run();

        long endTime = System.nanoTime();
        long elapsedTime = endTime - startTime;

        long memoryAfter = runtime.totalMemory() - runtime.freeMemory();
        long memoryUsed = memoryAfter - memoryBefore;

        System.out.println("Tiempo de CPU: " + (elapsedTime / 1_000_000.0) + " ms");
        System.out.println("Memoria usada: " + (memoryUsed / (1024.0 * 1024.0)) + " MB");
    }

    public static int[][] strassenMultiply(int[][] A, int[][] B) {
        int n = A.length;
        if (n == 1) {
            int[][] result = new int[1][1];
            result[0][0] = A[0][0] * B[0][0];
            return result;
        }

        int newSize = n / 2;
        int[][] A11 = new int[newSize][newSize];
        int[][] A12 = new int[newSize][newSize];
        int[][] A21 = new int[newSize][newSize];
        int[][] A22 = new int[newSize][newSize];
        int[][] B11 = new int[newSize][newSize];
        int[][] B12 = new int[newSize][newSize];
        int[][] B21 = new int[newSize][newSize];
        int[][] B22 = new int[newSize][newSize];

        // Dividir matrices A y B en 4 submatrices
        splitMatrix(A, A11, 0, 0);
        splitMatrix(A, A12, 0, newSize);
        splitMatrix(A, A21, newSize, 0);
        splitMatrix(A, A22, newSize, newSize);
        splitMatrix(B, B11, 0, 0);
        splitMatrix(B, B12, 0, newSize);
        splitMatrix(B, B21, newSize, 0);
        splitMatrix(B, B22, newSize, newSize);

        // Calcular los 7 productos de Strassen
        int[][] M1 = strassenMultiply(add(A11, A22), add(B11, B22));
        int[][] M2 = strassenMultiply(add(A21, A22), B11);
        int[][] M3 = strassenMultiply(A11, subtract(B12, B22));
        int[][] M4 = strassenMultiply(A22, subtract(B21, B11));
        int[][] M5 = strassenMultiply(add(A11, A12), B22);
        int[][] M6 = strassenMultiply(subtract(A21, A11), add(B11, B12));
        int[][] M7 = strassenMultiply(subtract(A12, A22), add(B21, B22));

        // Calcular los cuadrantes del resultado
        int[][] C11 = add(subtract(add(M1, M4), M5), M7);
        int[][] C12 = add(M3, M5);
        int[][] C21 = add(M2, M4);
        int[][] C22 = add(subtract(add(M1, M3), M2), M6);

        // Unir los cuadrantes
        int[][] C = new int[n][n];
        joinMatrix(C11, C, 0, 0);
        joinMatrix(C12, C, 0, newSize);
        joinMatrix(C21, C, newSize, 0);
        joinMatrix(C22, C, newSize, newSize);

        return C;
    }

    private static void splitMatrix(int[][] parent, int[][] child, int rowOffset, int colOffset) {
        for (int i = 0; i < child.length; i++) {
            for (int j = 0; j < child.length; j++) {
                child[i][j] = parent[i + rowOffset][j + colOffset];
            }
        }
    }

    private static void joinMatrix(int[][] child, int[][] parent, int rowOffset, int colOffset) {
        for (int i = 0; i < child.length; i++) {
            for (int j = 0; j < child.length; j++) {
                parent[i + rowOffset][j + colOffset] = child[i][j];
            }
        }
    }

    private static int[][] add(int[][] A, int[][] B) {
        int n = A.length;
        int[][] result = new int[n][n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                result[i][j] = A[i][j] + B[i][j];
        return result;
    }

    private static int[][] subtract(int[][] A, int[][] B) {
        int n = A.length;
        int[][] result = new int[n][n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                result[i][j] = A[i][j] - B[i][j];
        return result;
    }

    public static void main(String[] args) {
        // Rellenar matrices
        fillMatrices();

        // Elegir tamaño de bloque
        int blockSize = 16;

        // Medir el rendimiento de la multiplicación en bloques
        measurePerformance(() -> {
            C = blockedMultiply(A, B, blockSize);
        });

        measurePerformance(() -> {
            C = strassenMultiply(A, B);
        });
    }
}
