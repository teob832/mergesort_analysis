#include <iostream>
#include "Helper.hpp"
using namespace std;


template <class T> void selectionsort(T* data, int size){
    //printArray(data, size);
    for (int i = 0; i < size; i++){
        int min = i;
        for (int j = i; j < size; j++){				//Searches array from index i for minimum value
            if (data[j] <= data[min]) min = j;			//Assigns min value index
        }
        swap(data[i], data[min]);				//Swaps data at index i with the data at index min
    }
    //printArray(data, size);

}
template <class T> void insertionsort(T* data, int size){
    //printArray(data, size);
    for (int i = 1, j; i < size; i++){				
        for (j = i; (j > 0) && (data[j-1] > data[j]); j--){	//Goes through the array from index i down to zero or until the data of the next index is no longer larger than the data of the current index 
            swap(data[j-1],data[j]);				//Swaps the data at the index with the data of the index below, effectively pushing the data through the array until the data of the lower index is less than or equal
        }
    }
    //printArray(data, size);
}
template <class T> void mergesort(T* data, int size, T* temp){
    if (size > 1){
        int l = 0;
        int r = 0;
        int s = 0;
        int mid = (size+1)/2;
        T* left = data;					//Copies array from index 0
        T* right = &data[mid];				//Copies array from index mid
        mergesort(left, mid, temp);			//recursive calls
        mergesort(right, size - mid, &temp[mid]);
        while(l<mid && r<(size-mid)){			//Assumes both arrays left and right are ordered; Until l or r indexes reach their max values....   
            if(left[l] < right[r]){			//Compare the values of data[l] and data[r]
                temp[s] = left[l];			//Assign the data of the lower value to a temp array
                l++;					//Increment the index of the array of the copied data
            }
            else{
                temp[s] = right[r];
                r++;
            }
            s++;					//last index of ordered and copied data
        }						
        for (int i = l, j = s; i <= (mid-1), j <= (s+mid-l-1); i++, j++){	//The remaining data of arrays left and right are ordered so simply copy the rest to the temp array
            temp[j] = left[i];
        }
        s += (mid - l);
        for (int i = r, j = s; i <= (size-mid-1), j <= (s+size-mid-1-r); i++, j++){
            temp[j] = right[i];
        }
        for (int i = 0; i <= size-1; i++){
            data[i] = temp[i];							//Copy all of temp to data
        }
    }
    //printArray(data, size);
}
template <class T> void quicksort(T* data, int size){
    if(size <= 1){
        return;
    }
    int pivot;
    int mid = (size+1)/2;
    int pivotTest = medianof3(data[0], data[mid], data[size-1]);		//Find the median value at indexes 0, mid, and size-1 
    if(pivotTest == 1) pivot = 0;
    else if (pivotTest == 2) pivot = mid;
    else if (pivotTest == 3) pivot = size - 1;
    Swap(data[pivot], data[0]);							//Swap pivot to index 0
    int left = 0;
    int right = size - 1;
    do{
       while(left < right && data[left] <= data[0]) left++;			//Searches for the index of the first data greater than the pivot	
       while(left < right && data[right] > data[0]) right--;			//Searches for the index of the first data less than or equal to the pivot
       Swap(data[left], data[right]);						//Swaps the data of both these indexes so their values are ordered relative to the pivot
    }while (left < right);							//Continues doing this until indexes dont overlap 
    if (data[left] > data[0]) left--;						
    Swap(data[0], data[left]);
    quicksort(data, left);							//Recursive calls
    quicksort(&data[left+1],size-left-1);
}
