/***********************************************************************
 * Project4: MergeSort Analysis
 * Authors: OY, MS, AI, NM
 * Date: April 9, 2016
 *********************************************************************/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <semaphore.h>

#define MAX_THREADS 8	// The maximum number of threads spawned
#define MIN_SIZE 4	// The smallest subproblem which we will multithread
#define SHMKEY ((key_t) 2142)   // Shared Mem Key

sem_t mutex;

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


	//Recursive call to left
	mergeSort(arg.array,     )


	//Recursive call to right


	//Combine subsolutions



	//Check if leftovers exist LEFT



	//Check if leftovers exist RIGHT




	mergeSort(arg, )
}



startProgram(int n, int* arr)
{
	//Logic for size checking
	//
	//



	//create 2 threads

	pthread_create(tid[0], ARR, mergeSortDriver, arg);
	pthread_create(tid[1], ARR, mergeSortDriver, arg);
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
	
	// De-Allocate Semaphore
	sem_destroy(&mutex);	
	
	// De-Allocate Shared Memory
	if ((shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)
	{
		perror("shmctl");
		exit(-1);
	}    
	startProgram(n, arr);
}
