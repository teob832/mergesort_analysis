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





//Array Used to pass the argument
struct Arg
{
    int n;      //Size of the array
    int* array;
	int mid
	
};


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


//Starter
void* starter(int n, int* arr)
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




int main()
{
	int n;
	int* arr;
	int i;
	FILE* fp;


    //Call array generator


	//Read the file
    //*************************************************************
	fp = fopen("input.txt", "r");
	fscanf(fp, "%d", &n);
	arr = (int*) malloc(sizeof(int) * n);


	for (i = 0; i < n; ++i)
	{
		fscanf(fp, "%d", &arr[i]);
	}


    //Start the program
    //*************************************************************
    starter(n, &arr);



	//Clean Up
    //*************************************************************
	fclose(fp);
	free(arr);
    return 0;
}
