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

#define INPUT_SIZE 10000000  // The size of input to sort
#define MAX_THREADS 2	// The maximum number of threads spawned
#define MIN_SIZE 4	// The smallest subproblem which we will multithread
#define SHMKEY ((key_t) 9999)   // Shared Mem Key

sem_t mutex;

// Struct Used to pass the argument
struct Arg
{
    long n;
    long* array;
    long* temp;
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

// Returns true if a new thread should be created
// based on MAX_THREADS and MIN_SIZE
int shouldCreateThread(long array_size)
{
    sem_wait(&mutex);
    int num_threads = shared->thread_count;
    sem_post(&mutex);

    if(num_threads < MAX_THREADS
    && array_size >= MIN_SIZE)
        return 1;
    return 0;
}

//MergeSort 
void* mergeSort(void* arg_in)
{
    long size = ((struct Arg*)arg_in)->n;

    // Base Case
    if (size <= 1) return 0;

    // Set Left and Right arguments
    //*************************************************************
    long l = 0, r = 0, s = 0, i = 0, j = 0;
    long mid = (size + 1) / 2;

	struct Arg* arg_left = (struct Arg*) malloc(sizeof(struct Arg));
	struct Arg* arg_right = (struct Arg*) malloc(sizeof(struct Arg));	
 
	arg_left->n = mid;
	arg_right->n = size - mid;
	arg_left->array = ((struct Arg*)arg_in)->array;
	arg_right->array = ((struct Arg*)arg_in)->array + mid;
	arg_left->temp = ((struct Arg*)arg_in)->temp;
	arg_right->temp = ((struct Arg*)arg_in)->temp + mid;

    // Set Up Threads
    //*************************************************************
    pthread_t tid[2];           	//Ids for threads
    pthread_attr_t attr;           	//Attribute
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    // Recursive Calls
    //*************************************************************
    if(shouldCreateThread(arg_left->n))
    {
        pthread_create(&tid[0], &attr, mergeSort, arg_left);
        incrementThreadCount();
    }
	else
        mergeSort(arg_left);
	
    if(shouldCreateThread(arg_right->n))
    {
        pthread_create(&tid[1], &attr, mergeSort, arg_right);
        incrementThreadCount();
    }
	else	
        mergeSort(arg_right);

    // Wait for threads to finish, if any
    //*************************************************************
	if(shouldCreateThread(arg_left->n)) 
		pthread_join(tid[0], NULL);
	if(shouldCreateThread(arg_right->n))
		pthread_join(tid[1], NULL);

    // Combine Results
    //************************************************************
    while(l < mid && r < size - mid)
    {
        if(arg_left->array[l] < arg_right->array[r])
        {
            ((struct Arg*)arg_in)->temp[s] = arg_left->array[l];
            l++;
        }
        else
        {
            ((struct Arg*)arg_in)->temp[s] = arg_right->array[r];
            r++;
        }
        s++;
    }

    for (i = l, j = s; i <= mid - 1 && j <= s + mid - l - 1; i++, j++)
	    ((struct Arg*)arg_in)->temp[j] = arg_left->array[i];
        
    s += mid - l;

    for (i = r, j = s; i <= size - mid - 1 && j <= s + size - mid - 1 - r; i++, j++)
        ((struct Arg*)arg_in)->temp[j] = arg_right->array[i];

    for (i = 0; i <= size - 1; i++)
        ((struct Arg*)arg_in)->array[i] = ((struct Arg*)arg_in)->temp[i];

	free(arg_left);
	free(arg_right);

    return 0;
}

long* sortedArray(long n)
{
    long* arr = (long*) malloc(sizeof(long) * n);
    long i;
    for (i = 0; i < n; i++)
        arr[i] = i + 1;
    return arr;
}

void Swap(long* a, long* b)
{
    long t = *a;
    *a = *b;
    *b = t;
}

void genFile(long size)
{
    FILE* fp = fopen("input.txt","w");
    fprintf(fp, "%d\n", size);

    long* arr = sortedArray(size);

    long i;
    for (i = size; i > 1; i--)
    {
        long swap = rand() % i;
        Swap(&arr[swap], &arr[i - 1]);

    }

    for (i = 0; i < size; i++)
        fprintf(fp, "%d ", arr[i]);

    fprintf(fp, "\n \n");
    fclose(fp);
}

int main()
{
    // Create and connect to a shared memory segment
    //*************************************************************
    int shmid;
    char *shmadd;
    shmadd = (char *)0;

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
	
    // Read file and initialize semaphore, shared memory, and
    // input struct
    //*************************************************************
    sem_init(&mutex, 0, 1);
    shared->thread_count = 0;
    genFile(INPUT_SIZE);

    FILE* fp;
	fp = fopen("input.txt", "r");
    long n;
    fscanf(fp, "%d", &n);

    struct Arg* input_struct = (struct Arg*) malloc(sizeof(struct Arg));
    input_struct->array = (long*) malloc(sizeof(long) * n);
    input_struct->temp = (long*) malloc(sizeof(long)* n);
	input_struct->n = n;

	// Sort
	//*************************************************************
    mergeSort(input_struct);

    // Check Result
    //*************************************************************
    int sorted = 1;
    long i;
    for (i = 0; i < n - 1; i++)
    {
        if (input_struct->array[i + 1]
            < input_struct->array[i])
        {
            sorted = 0;
            break;
        }
    }

    if(sorted == 1)
        printf("Success\n");
    else
        printf("Fail\n");

//    printf("\nOutputs: ");
//    for(i = 0; i < n; ++i)
//    {
//        fscanf(fp, "%d", &(input_struct->array[i]));
//        printf("%d ",input_struct->array[i]);
//    }
//    printf("\n");

	//Clean Up
	//*************************************************************
	fclose(fp);				// Close file

    free(input_struct->array);    // Free things
    free(input_struct->temp);
    free(input_struct);

	sem_destroy(&mutex);			// De-Allocate Semaphore
	
	// De-Allocate Shared Memory
	if ((shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)
	{
		perror("shmctl");
		exit(-1);
	}    
	
	return 0;
}
