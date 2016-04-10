/***********************************************************************
 * Project4: MergeSort Analysis
 * Authors: OY, MS, AI, NM
 * Date: April 9, 2016
 *********************************************************************/

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
#define SHMKEY ((key_t) 9999)   // Shared Mem Key

sem_t mutex;

int n;

// Struct Used to pass the argument
struct Arg
{
    int n;      //Size of the array
    int* array;
    int* temp;
	
};

// Struct used for shared memory
typedef struct
{
	int thread_count;
} shared_mem;
shared_mem *shared;


// Increases the shared thread count by 1
void incrementThreadCount()
{
	sem_wait(&mutex);
	++(shared->thread_count);
	sem_post(&mutex);
}

//MergeSort 
void* mergeSort(void* arg_in)
{
	int fc = 0;

	
	
	int l = 0, r = 0, s = 0, i = 0, j = 0;
	int size = ((struct Arg*)arg_in)->n;

	if (size <= 1) return;	

	int mid = (size + 1) / 2;
	incrementThreadCount();
	pthread_t 		tid[2];           	//Ids for threads
	pthread_attr_t	attr;           	//Attribute
	
	struct Arg* arg_left = (struct Arg*) malloc(sizeof(struct Arg));
	struct Arg* arg_right = (struct Arg*) malloc(sizeof(struct Arg));	
 
	arg_left->n = mid;
	arg_right->n = size - mid;
	arg_left->array = ((struct Arg*)arg_in)->array;
	arg_right->array = ((struct Arg*)arg_in)->array + mid;
	arg_left->temp = ((struct Arg*)arg_in)->temp;
	arg_right->temp = ((struct Arg*)arg_in)->temp + mid;

    // Set Up
    //*************************************************************
    	pthread_attr_init(&attr);
    	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);  



    // Create the Threads 
    //*************************************************************
	if(shouldCreateThread(arg_left-> n)) 
		pthread_create(&tid[0], &attr, mergeSort, (void*)arg_left);
	else
		mergeSort((void*)arg_left);
	
	if(shouldCreateThread(arg_right-> n)) 
		pthread_create(&tid[1], &attr, mergeSort, (void*)arg_right);
	else	
		mergeSort((void*)arg_right);

	// Wait for threads to finish 
    //*************************************************************
	if(shouldCreateThread(arg_left->n)) 
		pthread_join(tid[0], NULL);
	if(shouldCreateThread(arg_right->n))
		pthread_join(tid[1], NULL);

	 while(l < mid && r < size-mid){			
            if(arg_left->array[l] < arg_right->array[r]){			
                ((struct Arg*)arg_in)->temp[s] = arg_left->array[l];			
                l++;					
            }
            else{
                ((struct Arg*)arg_in)->temp[s] = arg_right->array[r];
                r++;
            }
            s++;					
        }
	for (i = l, j = s; i <= mid-1, j <= s+mid-l-1; i++, j++)
	    ((struct Arg*)arg_in)->temp[j] = arg_left->array[i];
        
        s += (mid - l);

        for (i = r, j = s; i <= size - mid - 1, j <= s + size - mid - 1 - r; i++, j++)
            ((struct Arg*)arg_in)->temp[j] = arg_right->array[i];
        
        for (i = 0; i <= size - 1; i++)
            ((struct Arg*)arg_in)->array[i] = ((struct Arg*)arg_in)->temp[i];							

	free(arg_left);
	free(arg_right);

    
}




// Returns true if a new thread should be created
// based on MAX_THREADS and MIN_SIZE
int shouldCreateThread(int array_size)
{
	sem_wait(&mutex);
	int num_threads = shared->thread_count;
	sem_post(&mutex);
	
	if(num_threads < MAX_THREADS
	&& array_size >= MIN_SIZE)	
		return 1;
	return 0;
}

int main()
{
	int shmid;
	char *shmadd;
	shmadd = (char *)0;

	
	int j = 9001;
	int* arr;
	int i;
	FILE* fp;
	
	struct Arg* input_struct;
	input_struct = (struct Arg*) malloc(sizeof(struct Arg));
	
	/* Create and connect to a shared memory segment*/
	if ((shmid = shmget(SHMKEY, sizeof(shared_mem), IPC_CREAT | 0666)) < 0)
	{
		perror("shmget");
		exit(1);
	}

	if ((shared = (shared_mem*)shmat(shmid, shmadd, 0)) == (shared_mem *)-1)
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

	input_struct->temp = (int*) malloc(sizeof(int)* n);

	input_struct->n = n;
	input_struct->array = arr;

	// Sort
	//*************************************************************
	pthread_t tid[0];           	//Ids for threads
	pthread_attr_t	attr;           	//Attribute
	
	
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);  	
 	
	pthread_create(&tid[0], &attr, mergeSort, (void*)input_struct);
	pthread_join(tid[0], NULL);
	

	for (i = 0; i < n; ++i)
		printf("Output: %d\n", arr[i]);

	
	//Clean Up
	//*************************************************************
	free(input_struct->temp);
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
