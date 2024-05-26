#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>

int g_num_of_processes;
int *g_max;
int *g_allocated;
int *g_need;
int *g_safe_sequence;
int g_available;
int g_num_of_finished = 0;

pthread_mutex_t lock_resources;
pthread_cond_t condition;

bool get_safe_sequence();
void *process_code(void *arg);
void print_allocation_table();

int main(int argc, char const *argv[])
{
    // Input and Initialize.
    printf("Banker's Algorithm Using Single Resource.\n\n");
    printf("Enter the currently available resource: ");
    scanf("%d", &g_available);
    printf("\n");
    
    printf("Enter the number of processes: ");
    scanf("%d", &g_num_of_processes);

    g_max = (int*) malloc(sizeof(int) * g_num_of_processes);
    g_allocated = (int*) malloc(sizeof(int) * g_num_of_processes);
    g_need = (int*) malloc(sizeof(int) * g_num_of_processes);
    g_safe_sequence = malloc(sizeof(int) * g_num_of_processes);

    printf("Enter the currently allocated resource (P1 P2 ...): ");
    for (int i = 0; i < g_num_of_processes; ++i)
        scanf("%d", &g_allocated[i]);

    printf("Enter the maximum resource required (P1 P2 ...): ");
    for (int i = 0; i < g_num_of_processes; ++i)
        scanf("%d", &g_max[i]);
    printf("\n");

    // Calculate need.
    for (int i = 0; i < g_num_of_processes; ++i)
        g_need[i] = g_max[i] - g_allocated[i];

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
    sleep(1);

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
    free(g_max);
    free(g_allocated);
    free(g_need);
    free(g_safe_sequence);
    return 0;
}

bool get_safe_sequence()
{
    int safe_sequence_index = 0;
    int available = g_available;
    int allocated[g_num_of_processes];
    bool finished[g_num_of_processes];
    
    memcpy(allocated, g_allocated, sizeof(int) * g_num_of_processes);
    memset(finished, false, sizeof(bool) * g_num_of_processes);

    while (safe_sequence_index < g_num_of_processes)
    {
        bool find = false;
        for (int i = 0; i < g_num_of_processes; ++i)
        {
            if (!finished[i])
            {
                if (available >= g_need[i])
                {
                    available += g_need[i];
                    finished[i] = true;
                    find = true;
                    g_safe_sequence[safe_sequence_index++] = i;
                }
            }
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

    while (pid != g_safe_sequence[g_num_of_finished])
        pthread_cond_wait(&condition, &lock_resources);

    printf("[Process %d]\n", pid);
    printf("\tMaximum   : %d\n", g_max[pid]);
    printf("\tAllocated : %d\n", g_allocated[pid]);
    printf("\tNeed      : %d\n", g_need[pid]);
    printf("\tAvailable : %d\n", g_available);
    printf("\t%-2d >= %-2d  : true\n", g_available, g_need[pid]);

    printf("\tResource allocated! %d\n", g_need[pid]);
    sleep(1);
    g_allocated[pid] += g_need[pid];
    g_available -= g_need[pid];

    printf("\tProcess code running...\n");
    sleep(1);
    printf("\tProcess using resources...\n");
    sleep(rand() % 3 + 1);
    printf("\tProcess code completed...\n");

    printf("\tProcess releasing resources...\n");
    sleep(1);
    printf("\tResource released! %d\n", g_allocated[pid]);
    g_available += g_allocated[pid];
    g_allocated[pid] = 0;

    printf("\tNow Available : %d\n\n", g_available);

    ++g_num_of_finished;
    pthread_cond_broadcast(&condition);
    pthread_mutex_unlock(&lock_resources);
    pthread_exit(NULL);
}

void print_allocation_table()
{
    printf("===== Table for Resource Allocation =====\n\n");
    printf("Available resources: %d\n\n", g_available);
    printf("Process   Allocation   Maximum    Need \n");
    for (int i = 0; i < g_num_of_processes; ++i)
    {
        printf("   P%d         %2d         %2d        %2d\n", i + 1, g_allocated[i], g_max[i], g_need[i]);
    }
    printf("\n=========================================\n\n");
}