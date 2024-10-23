#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX 100

// Structure to hold data for each thread
typedef struct {
    int row;
    int aCols;
    int bCols;
    int (*a)[MAX];
    int (*b)[MAX];
    int (*result)[MAX];
} ThreadData;

// Function to fill a matrix with random values
void fillMatrix(int rows, int cols, int matrix[MAX][MAX]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 10; // Random values from 0 to 9
        }
    }
}

// Function to print a matrix
void printMatrix(int rows, int cols, int matrix[MAX][MAX]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function for each thread to compute a single row of the result matrix
void* multiplyRow(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int row = data->row;

    for (int j = 0; j < data->bCols; j++) {
        data->result[row][j] = 0;
        for (int k = 0; k < data->aCols; k++) {
            data->result[row][j] += data->a[row][k] * data->b[k][j];
        }
    }
    return NULL;
}

int main() {
    srand(time(NULL)); // Seed for random number generation

    int choice;
    do {
        printf("\nMatrix Multiplication with POSIX Threads\n");
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

            int a[MAX][MAX], b[MAX][MAX], result[MAX][MAX];
            fillMatrix(aRows, aCols, a);
            fillMatrix(bRows, bCols, b);

            printf("\nMatrix A:\n");
            printMatrix(aRows, aCols, a);
            printf("\nMatrix B:\n");
            printMatrix(bRows, bCols, b);

            pthread_t threads[aRows];
            ThreadData threadData[aRows];

            clock_t start = clock();
            for (int i = 0; i < aRows; i++) {
                threadData[i].row = i;
                threadData[i].aCols = aCols;
                threadData[i].bCols = bCols;
                threadData[i].a = a;
                threadData[i].b = b;
                threadData[i].result = result;

                pthread_create(&threads[i], NULL, multiplyRow, &threadData[i]);
            }

            for (int i = 0; i < aRows; i++) {
                pthread_join(threads[i], NULL);
            }
            clock_t end = clock();

            printf("\nResultant Matrix:\n");
            printMatrix(aRows, bCols, result);

            double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
            printf("Execution Time: %f seconds\n", time_spent);
        }
    } while (choice != 2);

    return 0;
}
