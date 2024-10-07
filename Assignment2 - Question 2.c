/***********************************************************************************************************************
 *                                                   Question2.c                                                       *
 * Nash William Morrison
 * COP 4610:[3:00-4:15]
 * Prof. Ahuja
 * 9/13/24
 *                                              ---Program Details---
 *   "The Collatz conjecture is one of the most famous unsolved problems in mathematics. The conjecture asks whether
 *        repeating two simple arithmetic operations will eventually transform every positive integer into 1."
 *
 *    My program creates a collatz() function that will take in an argument that will provide each step count, with
 *        the result for each step eventually heading towards 1, showcasing the Collatz conjecture. Parent & Child
 *        process are established, with the directions of the questions laid out within the code. Parent waits, While 
 *        child performs the operations.
 *
 *                                                  Progression Log
 *                                 ---------------------------------------------------
 *                                 ** 9/13: -Completed algorithm for the conjecture **
 *                                 **       -Made sure terminal commands work       **
 *                                   -----------------------------------------------
 *                                 ** 9/14: -Combined Parent/Child Code into my own **
 *                                 **       -Tested #includes w/ online compiler    **
 *                                 **       -Question1.c complete                   **
 *                                 **       -TEST ON LINUX OS                       **
 *                                 **       -Question1.c works on terminal          **
 *                                   -----------------------------------------------
 *                                 ** 9/15: -Started working on Question2.c         **
 *                                 **       -Question2.c completed                  **
 *                                 ***      -TEST ON LINUX OS                       **
 *                                 **       -Question2.c Works on terminal          **
 *                                 **                                               **
 *                                 ---------------------------------------------------
 *                                                Assignment-2 Complete                                                *
 *                                                         -NWM                                                        *
 * ********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#define array_MAX 1000


// Globals
double glo_IQR;
int glo_range;
int glo_count = 0;
int glo_array[array_MAX];
double glo_Standard_Dev_Survey;

// Structure to hold the data for the threads
typedef struct
{
    int* arr;
    int size;
}
        thread_data;

// Comparator function to sort the numbers in ascending order
int compare(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}

// Function to calculate the median
double median(int* arr, int start, int end)
{
    int len = end - start + 1;
    if (len % 2 == 0) {
        return (arr[start + len / 2 - 1] + arr[start + len / 2]) / 2.0;
    } else {
        return arr[start + len / 2];
    }
}

// Worker thread to calculate our range.
void *range(void *arg);

// Worker thread to calculate our Std.Dev
void *standard_Deviation(void *arg);

// Worker thread to calculate the IQR.
void* iqr(void* arg);


int main(int argc, char* argv[])
{
    // If there are not enough arguments passed through the terminal.
    if (argc < 2)
    {
        // Prints the amount of arguments passed through the terminal.
        printf("Usage: %s <numbers>\n", argv[0]);
        return 1;
    }

    // Extracts the numbers from the given command line arguments.
    int size = argc - 1;
    int* glo_Array = (int*)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        glo_Array[i] = atoi(argv[i + 1]);
    }

    // Setting our threads.
    pthread_t thread_Range,
            thread_Std_Dev,
            thread_IQR;

    // Establishing our struct from our data to be used in our threads.
    thread_data data = {glo_Array, size};
    
    // Creating our threads.
    pthread_create(&thread_Range, NULL, range, &data);
    pthread_create(&thread_Std_Dev, NULL, standard_Deviation, &data);
    pthread_create(&thread_IQR, NULL, iqr, &data);

    // Wait for the threads to complete
    pthread_join(thread_Range, NULL);
    pthread_join(thread_Std_Dev, NULL);
    pthread_join(thread_IQR, NULL);

    // Printing our results
    printf("The range value is: [%d]\n", glo_range);
    printf("The Standard Deviation of a Sample is: [%.4f]\n", glo_Standard_Dev_Survey);
    printf("The InterQuartile range value is: [%.2f]\n", glo_IQR);

    // Free allocated memory used for our array.
    free(glo_Array);
    return 0;
}

// Function that sorts the list of numbers into an array and finds the range.
void *range(void *arg)
{
    thread_data* data = (thread_data*)arg;
    int* arr = data->arr;
    int size = data->size;

    // Sort the array to find the minimum and maximum
    qsort(arr, size, sizeof(int), compare);

    // Calculate the range (max - min)
    glo_range = arr[size - 1] - arr[0];
    
    // Exit our thread.
    pthread_exit(NULL);
}

// Function that sorts the list of numbers into an array and finds the InterQuartile Range.
void* iqr(void* arg)
{
    // establishing our struct data to send this data to our array and the size of our array.
    thread_data* data = (thread_data*)arg;
    int* arr = data->arr;
    int size = data->size;

    // Sorting the array
    qsort(arr, size, sizeof(int), compare);

    // Calculate Q1 which is our 25th percentile.
    double Q1 = median(arr, 0, size / 2 - 1);

    // Calculate Q3 which is our 75th percentile.
    double Q3;
    if (size % 2 == 0) {
        Q3 = median(arr, size / 2, size - 1);
    } else {
        Q3 = median(arr, size / 2 + 1, size - 1);
    }

    // Calculate the IQR with Q3 - Q1.
    glo_IQR = Q3 - Q1;

    
    // Exit our thread.
    pthread_exit(NULL);
}

// A function that will calculate the standard deviation from our list of numbers.
void *standard_Deviation(void *arg)
{
    thread_data* data = (thread_data*)arg;
    int* arr = data->arr;
    int size = data->size;

    // Calculate the mean
    double mean = 0.0;
    for (int i = 0; i < size; i++) {
        mean += arr[i];
    }
    mean /= size;

    // Calculate the variance
    double variance = 0.0;
    for (int i = 0; i < size; i++) {
        variance += (arr[i] - mean) * (arr[i] - mean);
    }
    variance /= (size - 1);

    // Calculate the standard deviation
    glo_Standard_Dev_Survey = sqrt(variance);

    // Exit our thead.
    pthread_exit(NULL);

}
