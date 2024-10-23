#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time;
    int finish_time;
    int waiting_time;
    int response_time;
    int turnaround_time;
    bool is_completed;
} Process;

void read_input(char *filename, Process processes[], int *n, int *time_slice);
void fcfs(Process processes[], int n);
void sjf(Process processes[], int n);
void srtf(Process processes[], int n);
void rr(Process processes[], int n, int time_slice);
void calculate_and_print_results(Process processes[], int n);
void reset_processes(Process processes[], int n);
void print_ready_queue(Process processes[], int n, int queue[], int front, int rear, int size);

int main(void) {
    int n, time_slice;
    Process processes[100];
    
    read_input("processes.txt", processes, &n, &time_slice);
    
    printf("\n--- FCFS Scheduling ---\n");
    fcfs(processes, n);
    
    printf("\n--- SJF Scheduling ---\n");
    sjf(processes, n);
    
    printf("\n--- SRTF Scheduling ---\n");
    srtf(processes, n);
    
    printf("\n--- RR Scheduling ---\n");
    rr(processes, n, time_slice);
    
    return 0;
}

// Function to read input from file
void read_input(char *filename, Process processes[], int *n, int *time_slice) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file!\n");
        exit(1);
    }
    
    fscanf(file, "%d", n);
    fscanf(file, "%d", time_slice);

    for (int i = 0; i < *n; i++) {
        processes[i].pid = i + 1;
        fscanf(file, "%d %d", &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].start_time = -1;
        processes[i].is_completed = false;
    }

    fclose(file);
}

// Function to reset process data
void reset_processes(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].start_time = -1;
        processes[i].finish_time = 0;
        processes[i].waiting_time = 0;
        processes[i].response_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].is_completed = false;
    }
}

// Function to print ready queue
void print_ready_queue(Process processes[], int n, int queue[], int front, int rear, int size) {
    printf("Ready Queue: ");
    for (int i = front; i != rear; i = (i + 1) % size) {
        printf("P%d ", processes[queue[i]].pid);
    }
    printf("\n");
}

// Function to calculate and print results
void calculate_and_print_results(Process processes[], int n) {
    int total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0;

    printf("\nPID\tWT\tRT\tTAT\n");
    for (int i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
        total_response_time += processes[i].response_time;
        printf("P%d\t%d\t%d\t%d\n", processes[i].pid, processes[i].waiting_time, processes[i].response_time, processes[i].turnaround_time);
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)total_waiting_time / n);
    printf("Average Response Time: %.2f\n", (float)total_response_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
}

// FCFS Scheduling
void fcfs(Process processes[], int n) {
    int time = 0, completed = 0;
    
    reset_processes(processes, n);
    
    while (completed < n) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && !processes[i].is_completed) {
                idx = i;
                break;
            }
        }

        if (idx != -1) {
            if (processes[idx].start_time == -1) {
                processes[idx].start_time = time;
                processes[idx].response_time = time - processes[idx].arrival_time;
                printf("Process P%d started at time %d\n", processes[idx].pid, time);
            }

            printf("(CPU is executing P%d)\n", processes[idx].pid);
            time += processes[idx].burst_time;
            processes[idx].finish_time = time;
            processes[idx].turnaround_time = processes[idx].finish_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            processes[idx].is_completed = true;
            completed++;
        } else {
            printf("(CPU is idle)\n");
            time++;
        }
    }

    calculate_and_print_results(processes, n);
}

// SJF Scheduling
void sjf(Process processes[], int n) {
    int time = 0, completed = 0;

    reset_processes(processes, n);
    
    while (completed < n) {
        int idx = -1;
        int min_burst_time = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && !processes[i].is_completed && processes[i].burst_time < min_burst_time) {
                min_burst_time = processes[i].burst_time;
                idx = i;
            }
        }

        if (idx != -1) {
            if (processes[idx].start_time == -1) {
                processes[idx].start_time = time;
                processes[idx].response_time = time - processes[idx].arrival_time;
                printf("Process P%d started at time %d\n", processes[idx].pid, time);
            }

            printf("(CPU is executing P%d)\n", processes[idx].pid);
            time += processes[idx].burst_time;
            processes[idx].finish_time = time;
            processes[idx].turnaround_time = processes[idx].finish_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            processes[idx].is_completed = true;
            completed++;
        } else {
            printf("(CPU is idle)\n");
            time++;
        }
    }

    calculate_and_print_results(processes, n);
}

// SRTF Scheduling
void srtf(Process processes[], int n) {
    int time = 0, completed = 0;
    int min_remaining_time, idx;

    reset_processes(processes, n);
    
    while (completed < n) {
        min_remaining_time = INT_MAX;
        idx = -1;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && !processes[i].is_completed && processes[i].remaining_time < min_remaining_time) {
                min_remaining_time = processes[i].remaining_time;
                idx = i;
            }
        }

        if (idx != -1) {
            if (processes[idx].start_time == -1) {
                processes[idx].start_time = time;
                processes[idx].response_time = time - processes[idx].arrival_time;
                printf("Process P%d started at time %d\n", processes[idx].pid, time);
            }

            printf("(CPU is executing P%d)\n", processes[idx].pid);
            time++;
            processes[idx].remaining_time--;

            if (processes[idx].remaining_time == 0) {
                processes[idx].finish_time = time;
                processes[idx].turnaround_time = processes[idx].finish_time - processes[idx].arrival_time;
                processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
                processes[idx].is_completed = true;
                completed++;
            }
        } else {
            printf("(CPU is idle)\n");
            time++;
        }
    }

    calculate_and_print_results(processes, n);
}

// Round Robin Scheduling
void rr(Process processes[], int n, int time_slice) {
    int time = 0, completed = 0;
    int queue[100], front = 0, rear = 0;
    bool in_queue[100] = { false };
    bool process_added;

    reset_processes(processes, n);
    
    // Add processes that arrive at time 0 to the queue
    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time == 0) {
            queue[rear++] = i;
            in_queue[i] = true;
        }
    }

    while (completed < n) {
        process_added = false;

        if (front != rear) {
            int idx = queue[front];
            front = (front + 1) % 100;

            if (processes[idx].start_time == -1) {
                processes[idx].start_time = time;
                processes[idx].response_time = time - processes[idx].arrival_time;
                printf("Process P%d started at time %d\n", processes[idx].pid, time);
            }

            int execution_time = (processes[idx].remaining_time < time_slice) ? processes[idx].remaining_time : time_slice;
            printf("(CPU is executing P%d for %d time units)\n", processes[idx].pid, execution_time);
            time += execution_time;
            processes[idx].remaining_time -= execution_time;

            if (processes[idx].remaining_time == 0) {
                processes[idx].finish_time = time;
                processes[idx].turnaround_time = processes[idx].finish_time - processes[idx].arrival_time;
                processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
                processes[idx].is_completed = true;
                completed++;
            } else {
                queue[rear] = idx;
                rear = (rear + 1) % 100;
            }
        }

        // Add newly arrived processes to the queue
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && !processes[i].is_completed && !in_queue[i]) {
                queue[rear] = i;
                rear = (rear + 1) % 100;
                in_queue[i] = true;
                process_added = true;
                printf("A new job (P%d) has arrived at time %d\n", processes[i].pid, time);
            }
        }

        if (!process_added && front == rear) {
            printf("(CPU is idle)\n");
            time++;
        } else {
            print_ready_queue(processes, n, queue, front, rear, 100);
        }
    }

    calculate_and_print_results(processes, n);
}