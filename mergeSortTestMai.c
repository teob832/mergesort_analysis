#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <locale.h>

unsigned int numThreads;
int arraySize;
int *inpArr;
struct timeval start, end;
long mtime, secs, usecs, finalv, tmps;

typedef struct{
    int n;
    int *array;
} Arg;

int* sortedArray(int n)
{
    int* arr = (int*) malloc(sizeof(int) * n);
    int i;
    for (i = 0; i < n; i++)
        arr[i] = i + 1;
    return arr;
}

void Swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void genFile(int size)
{
    FILE* fp = fopen("input.txt","w");

    int* arr = sortedArray(size);

    int i;
    for (i = size; i > 1; i--)
    {
        int temp = rand() % i;
        Swap(&arr[temp], &arr[i - 1]);

    }

    for (i = 0; i < size; i++)
        fprintf(fp, "%lu ", arr[i]);

    fprintf(fp, "\n \n");
    fclose(fp);
}

void* mergeSortThreaded(void* argInput){

    int i, j;
    Arg* argCast = (Arg *) argInput;
    int size = argCast->n;

    if(size <= 1)
        return;
    else{
        int mid = size/2;

        Arg* arg_left; Arg* arg_right;
        arg_left = malloc(sizeof(Arg));
        arg_right = malloc(sizeof(Arg));

        arg_left->array = (int *)malloc(mid * sizeof(int));
        arg_right->array = (int *)malloc(sizeof(int) * (size - mid));

        //set values in left/right subarrays
        for(i = 0; i < mid; i++){
            arg_left->array[i] = argCast->array[i];
        }
        arg_left->n = mid;

        for(i = mid, j = 0; i < size; i++, j++){
            arg_right->array[j] = argCast->array[i];
        }
        arg_right->n = size - mid;

        mergeSortThreaded(arg_left);
        mergeSortThreaded(arg_right);

        //mergesort
        int left = 0; int right = 0;int t = 0;

        while(left < arg_left->n && right < arg_right->n){
            if(arg_left->array[left] < arg_right->array[right])
                argCast->array[t++] = arg_left->array[left++];
            else
                argCast->array[t++] = arg_right->array[right++];
        }

        while(left < arg_left->n){
            argCast->array[t++] = arg_left->array[left++];
        }

        while(right < arg_right->n){
            argCast->array[t++] = arg_right->array[right++];
        }

        free(arg_left->array);
        free(arg_right->array);
        free(arg_left);
        free(arg_right);
    }

}

void mergeSortHelper(Arg a[]){

    int i;
    int segmentSize = arraySize/numThreads;
    int ptrArr[numThreads];
    int *trackIndex;
    trackIndex = malloc(sizeof(int) * numThreads);
    int finalArray[arraySize];

    for(i = 0; i < numThreads; i++){
        trackIndex[i] = 0;
    }

    //set ptr to first element of each sorted array
    for(i = 0; i < numThreads; i++){
        ptrArr[i] = a[i].array[0];
    }


    int counter = 0;
    while(counter < arraySize){
        int min = 9999999999;
        int tmp;

        for(i = 0; i < numThreads; i++){
            //at end of current subarray; go to next
            if(trackIndex[i] == segmentSize || ptrArr[i] == -1)
                continue;
            if(ptrArr[i] < min){
                min = ptrArr[i];
                tmp = i;
            }
        }

        //increment index position
        trackIndex[tmp] = trackIndex[tmp] + 1;

        //set new value to pointer
        ptrArr[tmp] = a[tmp].array[trackIndex[tmp]];

        //add min value to array
        finalArray[counter] = min;

        counter++;
    }

    //set ptr location to finalArray
    free(trackIndex);
    inpArr = finalArray;
}

void mergeSort(){

    //regular mergeSort without any threads
    if(numThreads == 0){
        Arg a;
        a.array = inpArr;
        a.n = arraySize;
        mergeSortThreaded(&a);
        return;
    }

    //break initial array into numThreads subarrays
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

    //make threads
    gettimeofday(&start, NULL);

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

    gettimeofday(&end, NULL);
    secs  = end.tv_sec  - start.tv_sec;
    usecs = end.tv_usec - start.tv_usec;
    mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;
    tmps = mtime;
    printf("\nTime to sort subarrays in threads: %ld ms\n", mtime);

    //do k-way merging of sorted subarrays
    mergeSortHelper(subarrays);
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

    genFile(arraySize);

    //read file into input array
    FILE *fr = fopen("input.txt", "r");
    for(i = 0; i < arraySize; i++){
        fscanf(fr, "%d", &inputArr[i]);
    }

    fclose(fr);

    inpArr = inputArr;

    gettimeofday(&start, NULL);
    mergeSort();
    gettimeofday(&end, NULL);
    secs  = end.tv_sec  - start.tv_sec;
    usecs = end.tv_usec - start.tv_usec;
    mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;
    finalv = mtime - tmps;

    //check if array is sorted
    int sorted = 1;
    for(i = 0; i < arraySize-1; i++){
        if(inpArr[i] > inpArr[i+1]){
            sorted = 0;
            break;
        }
    }

    setlocale(LC_NUMERIC, "");
    if(numThreads == 0)
        printf("\n");
    if(sorted == 1){
        if(numThreads == 0)
            printf("Time for mergeSort: %ld ms \n", mtime);
        else{
            printf("Time for k-way merge of subarrays: %ld ms \n", finalv);
            printf("Total time: %d ms \n", finalv + tmps);
        }
        printf("Succeeded in sorting %'d elements with %d threads \n \n", arraySize, numThreads);
    }
    else
        printf("Failure to Sort\n");
}
