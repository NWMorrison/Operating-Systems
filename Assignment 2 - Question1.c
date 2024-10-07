/***********************************************************************************************************************
 *                                                   Question1.c                                                       *
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
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Global Variable For our Collatz Algorithm.
long long int result = 0;

// Initialized collatz function that accepts our terminal command.
void collatz(long long int x);

int main(int argc, char* argv[])
{
    // Setting our given argument from terminal to be used for our algorithm.
    // This handles error handling in a way where it will still do the calculations if you provide multiple numbers, by only
    // taking the second indexed number. So I am going to leave it as is.
	char* m = argv[1];
	argc = atoi(m);
	long long int n = argc;
	
	// Establishing our Parent and Child processes.
	pid_t pid;

	// Forking a child process
	pid = fork();


	// Error occurred from our pid.
	if (pid < 0)
	{

		fprintf(stderr, "Fork Failed\n");
		return 1;
	}

	// This is our child process, so cute
	else if (pid == 0)
	{

		printf("I am the child %d\n", pid);
		
		// My homemade Collatz Conjecture Function (This was very fun to code)
		collatz(n);
		printf("Child Complete...\n");
	}

	// This is our Parent Process, waits for child to complete. So nice of the parent.
	else
	{
	    // Waiting......
		wait(NULL);
		
		// All done!
		printf("Child is complete, I Am The Parent %d\n", pid);
		
	}
	return 0;
}

void collatz(long long int x)
{

	result = x;

	// Since we want to look at large integer values, it's best to set our step and our result to long-long.
	long long int step = 0;


	printf("Your Initial Input is [%lld]...\n", result);
	printf("Let's calculate your value into the 'Collatz Conjecture'\n\n");


	// While loop as long as our result is not equal to (1).
	while(result != 1)
	{

		// This handles errors for an invalid user input of negative values or '0'.
		if(result == 0 || result < 0)
		{
			printf("--ERROR-- Invalid Input of [%lld]\n "
			       "Integer cannot be 0, "
			       "or a negative number...\n", result);
			exit(0);
		}

		// If our result with our modulo operator is equal to (0), we have an even number.
		if (result % 2 == 0)
		{
			// Collatz Conjecture states to take every even integer found and divide it by (2).
			// We save this number to our global variable 'result' and pre-increment our step count.
			result /= 2;
			++step;
		}

		// Also, If our result with our modulo operator is equal to (1), we have an odd number.
		else if (result % 2 == 1)
		{
			// Collatz Conjecture states to take every odd integer found, multiply by (3) and add (1).
			// We save this number to our global variable 'result' and pre-increment our step count.
			result *= 3;
			
			// This would be considered our "+ 1".
			++result;
			++step;
		}

		// I decided to print each iteration of the loop to screen instead of an end result to showcase each step.
		printf("[Step:(%lld)] - Your result is: %lld\n", step, result);
	}

	// We print a final step count to the user. This also is a way of confirming that our algorithm properly works.
	printf("\nYour final step count not including our initial value is: %lld\n\n", step);
}
