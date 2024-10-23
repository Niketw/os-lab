#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fillMatrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 10;  // Random values from 0 to 9
        }
    }
}

void printMatrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void multiplyMatrices(int aRows, int aCols, int bRows, int bCols, int a[aRows][aCols], int b[bRows][bCols], int result[aRows][bCols]) {
    for (int i = 0; i < aRows; i++) {
        for (int j = 0; j < bCols; j++) {
            result[i][j] = 0;
            for (int k = 0; k < aCols; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main() {
    srand(time(NULL));  // Seed for random number generation

    int choice;
    do {
        printf("\nMatrix Multiplication Program\n");
        printf("1. Multiply Matrices\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int aRows, aCols, bRows, bCols;

            printf("Enter dimensions for Matrix A (rows columns): ");
            scanf("%d %d", &aRows, &aCols);
            printf("Enter dimensions for Matrix B (rows columns): ");
            scanf("%d %d", &bRows, &bCols);

            if (aCols != bRows) {
                printf("Matrix multiplication not possible. Columns of A must equal rows of B.\n");
                continue;
            }

            int a[aRows][aCols], b[bRows][bCols], result[aRows][bCols];

            fillMatrix(aRows, aCols, a);
            fillMatrix(bRows, bCols, b);

            printf("\nMatrix A:\n");
            printMatrix(aRows, aCols, a);
            printf("\nMatrix B:\n");
            printMatrix(bRows, bCols, b);

            clock_t start = clock();
            multiplyMatrices(aRows, aCols, bRows, bCols, a, b, result);
            clock_t end = clock();

            printf("\nResultant Matrix:\n");
            printMatrix(aRows, bCols, result);

            double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
            printf("Execution Time: %f seconds\n", time_spent);
        }
    } while (choice != 2);

    return 0;
}
