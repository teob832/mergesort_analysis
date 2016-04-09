/***********************************************************************
 * Project4: MergeSort Analysis
 * Authors: OY, MS, AI, NM
 * Date: April 9, 2016
 *********************************************************************/

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





int main()
{

    startProgram(n, arr);
}
