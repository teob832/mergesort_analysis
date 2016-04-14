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

unsigned int numThreads;
int arraySize;
int *inpArr;

typedef struct{
    int n;
    int *array;
} Arg;

void* mergeSortThreaded(void* argInput){

    int i, j;
    Arg* argCast = (Arg *) argInput;
    int size = argCast->n;

    if(size <= 1)
        return;
    else{
        int mid = size/2;

        Arg arg_left, arg_right;
        int arrL [mid], arrR [size - mid];

        arg_left.array = malloc(sizeof(int) * (mid ));
        arg_right.array = malloc(sizeof(int) * (size - mid));

        arg_left.array = arrL;
        arg_right.array = arrR;

        //set values in left/right subarrays
        for(i = 0; i < mid; i++){
            arg_left.array[i] = argCast->array[i];
        }
        arg_left.n = mid;

        for(i = mid, j = 0; i < size; i++, j++){
            arg_right.array[j] = argCast->array[i];
        }
        arg_right.n = size - mid;

    /*  Debug purposes
        printf("Threaded left: \n");
        for(i = 0; i <= mid; i++){
            printf(" %d ", arg_left.array[i]);
        }

        printf("\n Threaded right: \n");
        for(i = 0; i < size - mid - 1; i++){
            printf(" %d ", arg_right.array[i]);
        }
    */

        mergeSortThreaded(&arg_left);
        mergeSortThreaded(&arg_right);

        int left = 0; int right = 0;int t = 0;

        while(left < arg_left.n && right < arg_right.n){
            //seg_fault here
            if(arg_left.array[left] < arg_right.array[right])
                argCast->array[t++] = arg_left.array[left++];
            else
                argCast->array[t++] = arg_right.array[right++];
        }

        while(left < arg_left.n){
            argCast->array[t++] = arg_left.array[left++];
        }

        while(right < arg_right.n){
            argCast->array[t++] = arg_right.array[right++];
        }

    }

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

/*  Debug Purposes
    printf("\n");
    for(i = 0; i < numThreads; i++){
        printf("Sorted Segment %d: \n", i);
        for(j = 0; j < segmentSize + leftover; j++){
            if(subarrays[i].array[j] == -1)
                continue;
            printf("%d ", subarrays[i].array[j]);
        }
        printf("\n \n");
    }
*/

    //merge back elements
}

int main(int argc, char *argv[]){
    if(argc == 3){
        numThreads = (unsigned) atoi(argv[1]);
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

/*  Debug Purposes
    printf("\n Sorted Array: \n");
    for(i = 0; i < arraySize; i++){
        printf("%d ", &inpArr[i]);
    }
*/

    printf("\n");
    printf("numThreads: %d \n", numThreads);
    printf("arraySize: %d \n", arraySize);


}
