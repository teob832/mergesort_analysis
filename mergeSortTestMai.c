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
    //if size <=1 return

    //int mid = size / 2

    //create 2 new structs; left/right

    //copy elements into left/right

    //call mergeSortThreaded on left/right

    //merge elements together
}

void mergeSort(){
    int i, j;
    int segmentSize = arraySize/numThreads;

    Arg subarrays[numThreads];

    int leftover = arraySize % numThreads;
    int tmp[numThreads][segmentSize + leftover];
    memset(tmp, -1, numThreads * (segmentSize + leftover) * sizeof(int));

    for(i = 0; i < numThreads; i++){
        for(j = 0; j < segmentSize; j++){
            tmp[i][j] = inpArr[(i * segmentSize) + j];
        }

        //handle extra elements
        if(leftover > 0 && i == numThreads - 1){
            subarrays[i].n = leftover + segmentSize;
            for(j = segmentSize; j < leftover + segmentSize; j++){
                tmp[i][j] = inpArr[(i * segmentSize) + j];
            }
        }
        else
            subarrays[i].n = segmentSize;
        subarrays[i].array = tmp[i];
    }

    printf("\n");
    for(i = 0; i < numThreads; i++){
        printf("Segment %d: \n", i);
        for(j = 0; j < segmentSize + leftover; j++){
            if(subarrays[i].array[j] == -1)
                continue;
            printf("%d ", subarrays[i].array[j]);
        }
        printf("\n \n");
    }

    //make threads
    pthread_t thread[numThreads];

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

    //merge back elements
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

    fclose(fr);

    inpArr = inputArr;
    mergeSort();

    printf("\n");
    printf("numThreads: %d \n", numThreads);
    printf("arraySize: %d \n", arraySize);


}
