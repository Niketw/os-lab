#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_NUM 20

// Structure to pass arguments to threads
typedef struct {
    int start;
    int end;
    int *result;
} ThreadData;

// Function to calculate sum in a thread
void* calculate_sum(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sum = 0;
    for (int i = data->start; i <= data->end; i++) {
        sum += i;
    }
    *(data->result) = sum;
    return NULL;
}

// Function to sum using threads
void sum_with_threads(int num_threads) {
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    int results[num_threads];
    int range = MAX_NUM / num_threads;
    
    // Start time
    clock_t start_time = clock();

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].start = i * range + 1;
        thread_data[i].end = (i == num_threads - 1) ? MAX_NUM : (i + 1) * range;
        thread_data[i].result = &results[i];
        pthread_create(&threads[i], NULL, calculate_sum, &thread_data[i]);
    }

    // Join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calculate total sum
    int total_sum = 0;
    for (int i = 0; i < num_threads; i++) {
        total_sum += results[i];
    }

    // End time
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Sum using %d threads: %d\n", num_threads, total_sum);
    printf("Execution time with %d threads: %f seconds\n", num_threads, execution_time);
}

// Single-threaded sum
void sum_single_thread() {
    clock_t start_time = clock();
    int sum = 0;

    for (int i = 1; i <= MAX_NUM; i++) {
        sum += i;
    }

    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Sum using single thread: %d\n", sum);
    printf("Execution time with single thread: %f seconds\n", execution_time);
}

int main() {
    sum_single_thread(); // Calculate sum using a single thread

    sum_with_threads(2); // Calculate sum using 2 threads
    sum_with_threads(4); // Calculate sum using 4 threads
    sum_with_threads(10); // Calculate sum using 10 threads

    return 0;
}
