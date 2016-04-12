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

int numThreads;
int arraySize;
int *inpArr;

typedef struct{
    int n;
    int *array;
} Arg;

void* mergeSortThreaded(void* arg){
    int i;
    for(i = 0; i < )
    printf("%d ")
}

void mergeSort(){
    int i, j;
    int segmentSize = arraySize/numThreads;

    Arg subarrays[numThreads];

    //actually need to create an array to store the segments
    //set pointer to this array
    int tmp[numThreads][segmentSize];
    for(i = 0; i < numThreads; i++){
        for(j = 0; j < segmentSize; j++){
            tmp[i][j] = inpArr[(i * segmentSize) + j];
            //printf("%d ", inpArr[(i * segmentSize) + j]);
            //subarrays[i].array = inpArr[i * segmentSize];
            //subarrays[i].array + j = inpArr[(i+1) * j];
        }
        subarrays[i].array = tmp[i];
        //memset(tmp, 0, sizeof(tmp));
    }

    printf("\n");
    for(i = 0; i < numThreads; i++){
        printf("Segment %d: \n", i);
        for(j = 0; j < segmentSize; j++){
            printf("%d ", subarrays[i].array[j]);
        }
        printf("\n \n");
    }

    //make threads here
    pthread_t thread[numThreads];
    //thread = malloc(sizeof(pthread_t)*numThreads);

    for(i = 0; i < numThreads; i++){
      if(pthread_create (&thread[i], NULL, mergeSortThreaded, &subarrays[i]) != 0){
        printf("Error to create threads \n");
        exit(0);
      }
    }

    //wait for threads to finish
    for(i = 0; i < numThreads; i++){
      pthread_join(thread[i],NULL);
    }
}

int main(int argc, char *argv[]){
    if(argc == 3){
        numThreads = atoi(argv[1]);
        arraySize = atoi(argv[2]);
        if(numThreads > arraySize){
            printf("Error: numThreads cannot exceed arraySize\n");
            return;
        }
    }
    else{
        printf("Usage: ./main.out numThreads arraySize \n");
        return;
    }

    int i, inputArr[arraySize];
    srand(time(NULL));
    FILE *fp = fopen("input.txt", "w");
    for(i = 0; i < arraySize; i++){
        fprintf(fp, "%d ", rand() % 100);
    }

    fclose(fp);

    FILE *fr = fopen("input.txt", "r");
    for(i = 0; i < arraySize; i++){
        fscanf(fr, "%d", &inputArr[i]);
    }

    inpArr = inputArr;
    mergeSort();
    //test array elements
    //for(i = 0; i < arraySize; i++){
    //    printf("%d ", inpArr[i]);
    //}

    printf("\n");
    printf("numThreads: %d \n", numThreads);
    printf("arraySize: %d \n", arraySize);


}
