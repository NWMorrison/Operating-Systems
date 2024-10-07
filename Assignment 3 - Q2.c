/** ****************************************************************************************************************** *
*  Nash Morrison                                                                                                       *
*  10/5/2024                                                                                                           *
*  Dr. Ahuja                                                                                                           *
*  Assignment 3                                                                                                        *
*  Operating Systems                                                                                                   *
*                                                    Question [2]                                                      *
*                                                                                                                      *
*      In a bustling coffee shop, there are five baristas who need access to three shared coffee machines to make      *
*      coffee for customers. Baristas can only make coffee if they have exclusive access to all three coffee machines. *
*      Each barista must follow the rules below to avoid conflicts and ensure efficient coffee-making:                 *
*                                                                                                                      *
*                                                                                                                      *
*       1. A barista must have access to all three machines (left, center, and right) to start making coffee.          *
*       2. Each machine can only be used by one barista at a time.                                                     *
*       3. If any of the three machines are unavailable, the barista must wait until all are free.                     *
*       4. After making coffee, the barista releases the machines for others to use.                                   *
*                                                                                                                      *
*       Your task is to simulate this coffee shop scenario using C programming with mutex locks or semaphores          *
*       to prevent conflicts.                                                                                          *
*       Tasks:                                                                                                         *
*                                                                                                                      *
*       1. Implement a system where each barista is represented by a process.                                          *
*       2. Use appropriate synchronization mechanisms (mutex locks or semaphores) to control access to the shared      *
*           coffee machines.                                                                                           *
*       3. Ensure that no deadlock occurs and baristas work without conflict.                                          *
*       4. When executed, your program should provide a clear output that shows when each barista starts, acquires     *
*           each machine, does work, i.e., making coffee, finishing the work, and releasing the locks.                 *
*       5. Note that when compiled and executed, your solution should work in the Ubuntu environment.                  *
*                                                                                                                      *
*                       Deliverable: Provide your solution in C program as Q2.c                                        *
* ****************************************************************************************************************** **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BARISTAS 5
#define WORKING_MACHINES 4

sem_t machines[WORKING_MACHINES];
sem_t machines_Total;

void* barista(void* arg)
{
    // This will store our Machine ID so we can tell which machines are currently in use.
    int id = *(int*)arg;

    // Letting you know which of the Baristas working is currently waiting for a machine to begin making coffee.
    printf("Barista [%d] is waiting for all of the machines to begin making coffee. \n", ++id);

    // Our semaphore library call which waits for all three machines to become available.
    sem_wait(&machines_Total);

    // Let's use a for loop to acquire each machine.
    for (int i = 1; i < WORKING_MACHINES; ++i)
    {
        sem_wait(&machines[i]);
        printf("Barista [%d] has acquired machine [%d].\n", id, i);
    }

    // Seeing which Barista is making coffee at the given interval.
    printf("Barista [%d] is making coffee!\n", id);
    sleep(rand() % 3 + 1);

    printf("Barista [%d] has finished making their coffee.\n", id);

    // Now we have to release the machines so that any other Baristas may have a turn.
    for (int i = 1; i < WORKING_MACHINES; i++)
    {
        sem_post(&machines[i]);
        printf("Barista [%d] has released machine [%d].\n", id, i);
    }

    // This is our signal to post that all machines are available to make coffee.
    sem_post(&machines_Total);

    // Providing a rest cycle before we can make coffee again.
    sleep(rand() % 2 + 1);

    return NULL;
}

int main()
{
    pthread_t threads[BARISTAS];
    int barista_ID[BARISTAS];

    // Let's start by initializing our semaphores.
    sem_init(&machines_Total, 0, 1);
    for (int i = 0; i < WORKING_MACHINES; i++)
    {
        sem_init(&machines[i], 0, 1);
    }

    // We also need to create our Barista Threads.
    for (int i = 0; i < BARISTAS; i++) {
        barista_ID[i] = i;
        if (pthread_create(&threads[i], NULL, barista, &barista_ID[i]) != 0)
        {
            perror("Could not properly create the thread...");
            return -1;
        }
    }

    // This is where our barista threads will wait.
    for (int i = 0; i < BARISTAS; i++)
    {
        pthread_join(threads[i], NULL);
    }


    // We need to ability to clean up our semaphores.
    sem_destroy(&machines_Total);
    for (int i = 0; i < WORKING_MACHINES; i++)
    {
        sem_destroy(&machines[i]);
    }

    return 0;
}
