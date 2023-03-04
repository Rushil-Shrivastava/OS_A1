/*
RUSHIL SHRIVASTAVA
AKSHIT ADITYA
MANAS KUMAR KHANDWAL
ARYAN BHARDWAJ
ARJUN AGARWAL
ROHIT ARYAN
*/

#ifndef __CSF372_H__
#define __CSF372_H__

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <stdbool.h>

#define PRINT_INFO(MSG, ...) { \
	printf ( "%s INFO %d:%d %ld %s %s %d : " MSG ";;\n", \
	"TODO_PRINT_TIME", getpid(), getppid(), pthread_self(), __FILE__, __FUNCTION__, \
	__LINE__,  ##__VA_ARGS__); \
	}

#define PRINT_ERROR(MSG, ...) { \
	printf ( "%s ERROR %d:%d %ld %s %s %d : [%d] " MSG ";;\n", \
	"TODO_PRINT_TIME", getpid(), getppid(), pthread_self(), __FILE__, __FUNCTION__, \
	__LINE__,  errno, ##__VA_ARGS__);	\
	}

#define PRINT_ERR_EXIT(MSG, ...) { \
	printf ( "%s ERROR %d:%d %ld %s %s %d : [%d] " MSG ";;\n", \
	"TODO_PRINT_TIME", getpid(), getppid(), pthread_self(), __FILE__, __FUNCTION__, \
	__LINE__,  errno, ##__VA_ARGS__);	\
	_exit(-1); \
	}
	
#endif //__CSF372_H__

// Global variables
int n;
int a;
int b;
int p;

// Function declarations
void retPrime(int x, int size,int *a);
void check(int **arr);
void workerProcess(int **arr);
int createThapx(int *arr);
void *thapx(void *arg);

void handle_sigtstp(int sig) {
    PRINT_INFO("\nStop not allowed\n");
}

int main(int argc, char** argv){
    // Initialize global variable n
    n = atoi(argv[1]);

    // Check if valid input
    if (argc != n*n + 5){
		PRINT_ERROR("\nInvalid args!!!");
		_exit(EINVAL);
	}

    // Initialize global variables
    a = atoi(argv[2]);
    b = atoi(argv[3]);
    p = atoi(argv[4]);

    PRINT_INFO("\nThe value of n = %d, a = %d, b = %d, p = %d\n", n, a , b, p);

    // Make a matrix in which we store values
    int arr[n][n];

    int l = 5;

    // Allocating values in it
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            arr[j][k] = atoi(argv[l]);
            l++;
        }
    }



    // Checking if the array is correct using worker processes
    for (int j = 0; j < n; j++) {
        if (fork() == 0) {
            for (int k = 0; k < n; k++) {
                if (arr[j][k] > b || arr[j][k] < a) {
                    PRINT_ERROR("\nInvalid args in Row %d, Column %d!!!", j + 1, k + 1);
                    _exit(EINVAL);    
                }   
            }   
        } else {
            _exit(EINVAL);
        }
    }

    // Pipe
    int fd[n][2];

    signal(SIGTSTP, &handle_sigtstp);

    // Prints the value of Thapx
    int array[n];
    for (int i = 0; i < n; i++) {
        if(pipe(fd[i]) == -1) {
            PRINT_ERROR("\nPipe not formed");
        } else {
            PRINT_INFO("\nPipe Created\n");
        }

        if (fork() == 0) { // Child created
            PRINT_INFO("\nNew Child Started\n");
            int *row = arr[i];
            int wapx = createThapx(row); // Function to make Thread
            array[i] = wapx;
            for (int j = 0; j < n; j++) {
                if(i != j) {
                    close(fd[j][0]);
                    close(fd[j][1]);
                }
            }
            PRINT_INFO("\nThe Wapx in being written in controller\n");
            close(fd[i][0]);
            write(fd[i][1], &wapx, sizeof(wapx));
            close(fd[i][1]);

            for (int j = 0; j < n; j++) {
                if(i != j) {
                    close(fd[j][0]);
                    close(fd[j][1]);
                }
            }
            close(fd[i][1]);
            read(fd[i][0], &array[i], sizeof(array[i]));
            close(fd[i][0]);

            PRINT_INFO("\nThe Wapx is copied in controller\n");

        } else {
            wait(NULL);            
            _exit(EINVAL);
        }
    }

    // Calculating Fapx
    int fapx;
    for (int i = 0; i < n; i++) {
        fapx += array[i];
    }

    fapx = fapx / n;

    // Printing Fapx
    PRINT_INFO("\nThe value of Fapx is: %d\n\n\n", fapx);
    
    //Exit
	_exit(0);
}

int createThapx(int *arr) {
    int *result;
    int thap[n];


    pthread_t T[n];  // Thread created


    for (int i = 0; i < n; i++) { // Thread doing thread stuff
        
        PRINT_INFO("\nThread %d Created\n", i);
        pthread_create(&T[i], NULL, thapx, &arr[i]);
        pthread_join(T[i], (void *)&result);
        
        thap[i] = *result;
    }

    int sum = 0;

    for (int i = 0; i < n; i++) {
        sum += thap[i];
    }

    int wapx = sum/n;
    PRINT_INFO("\nThe value of Wapx in worker is: %d\n", wapx);
    return wapx;
}

void *thapx(void *val) { // Thread function returning Thapx
    PRINT_INFO("\nThread is running\n");
    int *x = (int *)val;
    int k = *x;
    int array[(2*p) + 1];
    int *result = malloc(sizeof(int));
    int o = 2*p + 1;
    for (int i = 0; i < (2*p) + 1; i++) {
        array[i] = 0;
    }

    retPrime(k, (2*p) + 1, array);
    PRINT_INFO("\nPx array is filled\n");
    for (int i = 2*p; i >= 0; i--) {
        if (array[i] == 0) {
            o--;
        } else {
            break;
        }
    }

    int sum = 0;
    for (int i = 0; i < o; i++) {
        sum += array[i];
    }

    int avg = sum/o;
    *result = avg;
    PRINT_INFO("\nThe value of Thapx is: %d\n", avg);
    return result;                                
}

bool checkPrime(int x){ // Check if the number is prime
    bool f1 = true;
    for (int i = 2; i*i <= x; i++) {
        if (x%i == 0) {
            f1 = false;
            break;
        }
    }
    // PRINT_INFO("\nPrime number found\n");
    return f1;
}

void retPrime(int x,  int size, int *array) { // ye hai func ret prime
    // Initialize few temporary variables
    int ff = x;
    int fa = x;

    // This is index
    int index = 0;
    
    // This is limiting variable for P
    int i = 0;

    // Saves the lower p primes
    while(i < p && ff > 1){
        ff--;
        if (checkPrime(ff)) {
            array[index] = ff;
            index++;
            i++;
        }
    }

    // Saves the current number if prime
    if (checkPrime(x)) {
        array[index] = x;
        index++;
    }

    // Limiting variable for p
    int j = 0;

    // Saves the upper P primes
    while(j < p){
        fa++;
        if(checkPrime(fa)){
            array[index] = fa;
            index++;
            j++;
        }
    }
}