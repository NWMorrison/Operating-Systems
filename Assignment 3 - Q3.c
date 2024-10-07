/** ****************************************************************************************************************** *
*  Nash Morrison     *                                                                                                 *
*  10/5/2024         *                                                                                                 *
*  Dr. Ahuja         *                                                                                                 *
*  Assignment 3      *                                                                                                 *
*  Operating Systems *                                                                                                 *
*  * * * * * * * * *                              Question [3]                                                         *
*                                                                                                                      *
*       In FGCU's parking lot there are limited spaces. Cars (drivers) arrive throughout the day trying to find a        *
*       spot. Once a car parks, it stays for a certain amount of time before leaving, freeing up a spot for the next   *
*       car.                                                                                                           *
*                                       The Parking Lot Follows These Rules:                                           *
*              1. The parking lot has a fixed number of spaces.                                                        *
*              2. Cars arrive one by one to park.                                                                      *
*              3. If a parking space is available, the car parks. Otherwise, it waits until a spot becomes free.       *
*              4. When a car leaves, it frees up a space for waiting cars.                                             *
*                                                                                                                      *
*        The task is to implement a model using POSIX threads and semaphores in C to simulate this parking lot:        *
*                                                                                                                      *
*              1. Create a simulation where there are n cars arriving randomly.                                        *
*              2. The parking lot has a limited number of parking spots (e.g., 5).                                     *
*              3. Cars will wait if there are no parking spots available and will park as soon                         *
*                 as a spot is freed up.                                                                               *
*                                                                                                                      *
*                                           Output The Following:                                                      *
*              1. When a car parks.                                                                                    *
*              2. When a car leaves the parking lot.                                                                   *
*              3. The current number of available parking spaces.                                                      *
*                                                                                                                      *
*           Note that when compiled and executed, your solution should work in the Ubuntu environment.                 *
*                                                                                                                      *
*                           Deliverable: Provide your solution in C program as Q3.c                                    *
* ****************************************************************************************************************** **/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>           // For our sleep function to simulate a rotating car lot.

#define PARKING_SPOTS 10       // Number of parking spots in the lot
#define CARS 15               // Number of cars arriving randomly

sem_t parking_Spots;          // Semaphore to track available parking spots
pthread_mutex_t print_lock;   // Mutex to protect print statements

void* zoom_Zoom_Zoom(void* car_id) {
    int id = *((int*)car_id);

    // Lets the user know which car has arrived in the parking lot.
    pthread_mutex_lock(&print_lock);
    printf("Car [%d] has arrived.\n", id);
    pthread_mutex_unlock(&print_lock);

    // Trying to park our car. If there are no spots available then we will wait.
    sem_wait(&parking_Spots); // Decrease the semaphore value (occupy a spot)

    // This code is where we are parking our car, and letting the user now how many spots are available for use.
    pthread_mutex_lock(&print_lock);
    int spots_available;
    sem_getvalue(&parking_Spots, &spots_available);
    printf("Car [%d] is parking. Spots available: [%d]\n", id, spots_available);
    pthread_mutex_unlock(&print_lock);

    // Simulates a car parked for only a short duration of random time [1-8 seconds].
    sleep(rand() % 8 + 1);

    // Car leaves, free up a parking spot
    pthread_mutex_lock(&print_lock);

    // This call increases our semaphore count (Sooo we free up a spot for use)
    sem_post(&parking_Spots);
    sem_getvalue(&parking_Spots, &spots_available);
    printf("Car [%d] is leaving. Spots available: [%d]\n", id, spots_available);
    pthread_mutex_unlock(&print_lock);

    // Frees up our car id.
    free(car_id);
    return NULL;
}

int main()
{
    pthread_t car_threads[CARS];

    // Initialize the semaphore with the number of parking spots
    sem_init(&parking_Spots, 0, PARKING_SPOTS);

    // Initialize the mutex
    pthread_mutex_init(&print_lock, NULL);

    // Create threads representing cars arriving at random intervals
    for (int i = 0; i < CARS; i++)
    {
        // Allocating memory to use for our car id so that we have no issues with memory down the road.
        int* car_id = malloc(sizeof(int));
        *car_id = i + 1;

        // Create a thread for each car
        pthread_create(&car_threads[i], NULL, zoom_Zoom_Zoom, car_id);

        // Simulates the arrival of cars at different time intervals (I used between 1 and 3 seconds)
        sleep(rand() % 3 + 1);
    }

    // Wait for all of our car threads to finish
    for (int i = 0; i < CARS; i++)
    {
        pthread_join(car_threads[i], NULL);
    }

    // Destroys the semaphore and mutex locks
    sem_destroy(&parking_Spots);
    pthread_mutex_destroy(&print_lock);

    return 0;
}