// Algorytm-benchmark do liczenia złożoności

#include <cstdio>
#include <cstdlib>
#include <ctime>

// Mnożenie macierzy dwuwymiarowych
void matrixMultiply(int n, int **mat1, int **mat2, int **result) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = 0;
            for (int k = 0; k < n; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

// Liczba fibonacciego; duże n żeby długo liczyło
unsigned long fibonacci(int n) {
    if (n <= 1) return n;
    unsigned long a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        unsigned long c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main() {
    int n = 2000; //Wielkość macierzy; tyle żeby działanie było długie

    // Tworzenie macierzy
    int **mat1 = static_cast<int **>(malloc(n * sizeof(int *)));
    int **mat2 = static_cast<int **>(malloc(n * sizeof(int *)));
    int **result = static_cast<int **>(malloc(n * sizeof(int *)));
    for (int i = 0; i < n; i++) {
        mat1[i] = static_cast<int *>(malloc(n * sizeof(int)));
        mat2[i] = static_cast<int *>(malloc(n * sizeof(int)));
        result[i] = static_cast<int *>(malloc(n * sizeof(int)));
    }

    // Zapełnianie macierzy
    srand(time(nullptr));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat1[i][j] = rand() % 100;
            mat2[i][j] = rand() % 100;
        }
    }

    // Start pomiaru czasu
    // clock_t start = clock();
    matrixMultiply(n, mat1, mat2, result);

    unsigned long fibResult = fibonacci(45);

    // Koniec pomiaru czasu
    // clock_t end = clock();
    // double timeSpent = static_cast<double>(end - start) / CLOCKS_PER_SEC;


    free(mat1);
    free(mat2);
    free(result);

    return 0;
}
