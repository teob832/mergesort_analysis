/***********************************************************************
 * Project4: MergeSort Analysis
 * Authors: OY, MS, AI, NM
 * Date: April 9, 2016
 *********************************************************************/

<<<<<<< HEAD
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_THREADS 8	// The maximum number of threads spawned
#define MIN_SIZE 4	// The smallest subproblem which we will multithread
#define SHMKEY ((key_t) 2142)   // Shared Mem Key

sem_t mutex;

// Struct Used to pass the argument
struct Arg
{
    int n;      //Size of the array
    int* array;
	int mid
	
};

// Struct used for shared memory
typedef struct
{
	int thread_count;
	int turn;
	int flag[2];
} shared_mem;
shared_mem *shared;

mergeSortDriver(void* arg)
{
	//If single element
	if (arg.n == 1)
	return;

	//Find midpoint
	int mid = arg.n / 2;


//MergeSort 
void* mergeSort(void* arg)
{
    pthread_t 		tid[0];           	//Ids for threads
    pthread_attr_t	attr;           	//Attribute
	struct Arg arg;

	arg.n = n;
	arg.array = array;

    // Set Up
    //*************************************************************
    //pthread_mutex_init(&mutex, NULL);    //Initialize mutex
    
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);  



    //Logic for size checking
    //*************************************************************



    // Create the Threads 
    //*************************************************************
    pthread_create(&tid[0], &attr, mergeSort, arg);
    //pthread_create(&tid[0], &attr, mergeSort, arg);






    // Wait for thread to finish 
    //*************************************************************
    pthread_join(tid[0], NULL);
}

// Increases the shared thread count by 1
void incrementThreadCount()
{
	sem_wait(&mutex);
	++shared->thread_count;
	sem_post(&mutex);
}

// Returns true if a new thread should be created
// based on MAX_THREADS and MIN_SIZE
bool createThread(int array_size)
{
	sem_wait(&mutex);
	int num_threads = shared->thread_count;
	sem_post(&mutex);
	
	return num_threads < MAX_THREADS
	&& array_size >= MIN_SIZE;	
}

int main()
{
	int n;
	int* arr;
	int i;
	FILE* fp;
	
	/* Create and connect to a shared memory segment*/
	if ((shmid = shmget(SHMKEY, sizeof(shared_mem), IPC_CREAT | 0666)) < 0)
	{
		perror("shmget");
		exit(1);
	}

	if ((total = (shared_mem*)shmat(shmid, shmadd, 0)) == (shared_mem *)-1)
	{
		perror("shmat");
		exit(0);
	}
	
	// Init thread_count
	shared->thread_count = 0;
	
	// Create Semaphore to protect thread_count
	sem_init(&mutex, 0, 1);
		

	//Call array generator


	//Read the file
	//*************************************************************
	fp = fopen("input.txt", "r");
	fscanf(fp, "%d", &n);
	arr = (int*) malloc(sizeof(int) * n);


	for (i = 0; i < n; ++i)
		fscanf(fp, "%d", &arr[i]);



	// Sort
	//*************************************************************
	mergeSort(n, &arr);

	
	//Clean Up
	//*************************************************************
	fclose(fp);				// Close file
	free(arr);
	sem_destroy(&mutex);			// De-Allocate Semaphore
	
	// De-Allocate Shared Memory
	if ((shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)
	{
		perror("shmctl");
		exit(-1);
	}    
	
	return 0;
}
