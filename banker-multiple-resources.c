#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>

int g_num_of_processes;
int g_num_of_resources;
int **g_max;
int **g_allocated;
int **g_need;
int *g_safe_sequence;
int *g_available;
int g_num_of_finished = 0;

pthread_mutex_t lock_resources;
pthread_cond_t condition;

bool get_safe_sequence();
void *process_code(void *arg);
void print_allocation_table();
void print_vector(int *vector, int n);

int main(int argc, char const *argv[])
{
    // Input and Initialize.
    printf("Banker's Algorithm Using Multiple Resources.\n\n");
    printf("Enter the number of resources: ");
    scanf("%d", &g_num_of_resources);

    g_available = (int*) malloc(sizeof(int) * g_num_of_resources);

    printf("Enter the currently available resources (R1 R2 ...): ");
    for (int i = 0; i < g_num_of_resources; ++i)
        scanf("%d", &g_available[i]);
    printf("\n");
    
    printf("Enter the number of processes: ");
    scanf("%d", &g_num_of_processes);

    g_max = (int**) malloc(sizeof(int*) * g_num_of_processes);
    g_allocated = (int**) malloc(sizeof(int*) * g_num_of_processes);
    g_need = (int**) malloc(sizeof(int*) * g_num_of_processes);
    g_safe_sequence = (int*) malloc(sizeof(int) * g_num_of_processes);
    for (int i = 0; i < g_num_of_processes; ++i)
    {
        g_allocated[i] = (int*) malloc(sizeof(int) * g_num_of_resources);
        g_max[i] = (int*) malloc(sizeof(int) * g_num_of_resources);
        g_need[i] = (int*) malloc(sizeof(int) * g_num_of_resources);
    }

    for (int i = 0; i < g_num_of_processes; ++i)
    {
        printf("Enter the currently allocated resource to Process %d (R1 R2 ...): ", i);

        for (int j = 0; j < g_num_of_resources; ++j)
            scanf("%d", &g_allocated[i][j]);
    }
    printf("\n");
    
    for (int i = 0; i < g_num_of_processes; ++i)
    {
        printf("Enter the maximum resource required by Process %d (R1 R2 ...): ", i);

        for (int j = 0; j < g_num_of_resources; ++j)
            scanf("%d", &g_max[i][j]);
    }
    printf("\n");

    // Calculate need.
    for (int i = 0; i < g_num_of_processes; ++i)
    {   
        for (int j = 0; j < g_num_of_resources; ++j) 
            g_need[i][j] = g_max[i][j] - g_allocated[i][j];
    }

    print_allocation_table();

    // Find safe sequence.
    bool safe = get_safe_sequence();
    if (!safe)
    {
        printf("[ERROR] The system is unsafe!\n");
        return 0;
    }
    printf("Safe sequence Found: ");
    for (int i = 0; i < g_num_of_processes; ++i)
    {
        printf("%d", g_safe_sequence[i]);
        if (i < g_num_of_processes - 1)
            printf(" -> ");
    }
    printf("\n");

    // Execute Process.
    printf("Executing Processes...\n\n");
    // sleep(1);

    pthread_mutex_init(&lock_resources, NULL);
    pthread_t processes[g_num_of_processes];
    pthread_attr_t attribuite;
    pthread_attr_init(&attribuite);
    pthread_cond_init(&condition, NULL);

    int process_id[g_num_of_processes];
    for (int i = 0; i < g_num_of_processes; ++i)
        process_id[i] = i;

    for (int i = 0; i < g_num_of_processes; ++i)
        pthread_create(&processes[i], &attribuite, process_code, &process_id[i]);

    for (int i = 0; i < g_num_of_processes; ++i)
        pthread_join(processes[i], NULL);

    printf("All processes finished.\n");
    return 0;
}

bool get_safe_sequence()
{
    int safe_sequence_index = 0;
    int available[g_num_of_resources];
    int allocated[g_num_of_processes][g_num_of_resources];
    bool finished[g_num_of_processes];
    
    memcpy(available, g_available, sizeof(int) * g_num_of_resources);
    for (int i = 0; i < g_num_of_processes; ++i) 
        memcpy(allocated[i], g_allocated[i], sizeof(int) * g_num_of_resources);
    memset(finished, false, sizeof(bool) * g_num_of_processes);

    while (safe_sequence_index < g_num_of_processes)
    {
        bool find = false;
        for (int i = 0; i < g_num_of_processes; ++i)
        {   
            if (finished[i])
                continue;

            bool assignable = true;
            for (int j = 0; j < g_num_of_resources; ++j) 
            {
                if (available[j] < g_need[i][j])
                {
                    assignable = false;
                    break;
                }
            }

            if (!assignable)
                continue;
            
            for (int j = 0; j < g_num_of_resources; ++j) 
                available[j] += g_need[i][j];
            finished[i] = true;
            find = true;
            g_safe_sequence[safe_sequence_index++] = i;
        }

        if (!find)
        {
            memset(g_safe_sequence, -1, sizeof(int) * g_num_of_processes);
            return false;
        }
    }
    return true;
}

void *process_code(void *arg)
{   
    int pid = *((int*) arg);

    pthread_mutex_lock(&lock_resources);
    
    while (pid != g_safe_sequence[g_num_of_finished]) {
        pthread_cond_wait(&condition, &lock_resources);
    }

    printf("[Process %d]\n", pid);
    printf("\tMaximum   : ");
    print_vector(g_max[pid], g_num_of_resources);
    printf("\n");
    printf("\tAllocated : ");
    print_vector(g_allocated[pid], g_num_of_resources);
    printf("\n");
    printf("\tNeed      : ");
    print_vector(g_need[pid], g_num_of_resources);
    printf("\n");
    printf("\tAvailable : ");
    print_vector(g_available, g_num_of_resources);
    printf("\n");
    printf("\t");
    print_vector(g_available, g_num_of_resources);
    printf(" >= ");
    print_vector(g_need[pid], g_num_of_resources);
    printf(" : true\n");

    printf("\tResource allocated! ");
    print_vector(g_need[pid], g_num_of_resources);
    printf("\n");
    sleep(1);
    for (int i = 0; i < g_num_of_resources; ++i)
    {
        g_allocated[pid][i] += g_need[pid][i];
        g_available[i] -= g_need[pid][i];
    }

    printf("\tProcess code running...\n");
    sleep(1);
    printf("\tProcess using resources...\n");
    sleep(rand() % 3 + 1);
    printf("\tProcess code completed...\n");

    printf("\tProcess releasing resources...\n");
    sleep(1);
    printf("\tResource released! ");
    print_vector(g_allocated[pid], g_num_of_resources);
    printf("\n");
    for (int i = 0; i < g_num_of_resources; ++i)
    {
        g_available[i] += g_allocated[pid][i];
        g_allocated[pid][i] = 0;
    }

    printf("\tNow Available : ");
    print_vector(g_available, g_num_of_resources);
    printf("\n\n");

    ++g_num_of_finished;
    pthread_cond_broadcast(&condition);
    pthread_mutex_unlock(&lock_resources);
    pthread_exit(NULL);
}

void print_allocation_table()
{
    printf("===== Table for Resource Allocation =====\n\n");
    printf("Available resources: ");
    print_vector(g_available, g_num_of_resources);
    printf("\n\n");
    printf("Process   Allocation   Maximum    Need \n");
    for (int i = 0; i < g_num_of_processes; ++i)
    {
        printf("   P%d        ", i);
        print_vector(g_allocated[i], g_num_of_resources);
        printf("      ");
        print_vector(g_max[i], g_num_of_resources);
        printf("     ");
        print_vector(g_need[i], g_num_of_resources);
        printf("\n");
    }
    printf("\n=========================================\n\n");
}

void print_vector(int *vector, int n)
{
    for (int i = 0; i < n; ++i)
    {
        printf("%d", vector[i]);
        if (i < n - 1)
            printf(" ");
    }
}