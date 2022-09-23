#include <stdio.h>
#include <stdlib.h>
#define MAX 20

void swap_num(int* num1, int* num2);

void swap_num(int* num1, int* num2)
{
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

void max_heapify(int array[], int index, int heapSize){
  /* add your code here */
    int left_index = 2 * index + 1;
    int right_index = 2 * index + 2;
    int largest_index = index;
    
    if (array[left_index] > array[largest_index] && largest_index < heapSize)
        largest_index = left_index;
    else
        largest_index = index;
    
    if (array[right_index] > array[largest_index] && right_index < heapSize)
        largest_index = right_index;
    
    if (index != largest_index)
    {
        swap_num(&array[index], &array[largest_index]);
        max_heapify(array, largest_index, heapSize);
    }
}

int* build_max_heap(int array[], int arraySize){
    int j;
    for (j = arraySize/2; j >= 0; j--) {
        max_heapify(array, j, arraySize);
    }
    return array;
}

void display(int *max_heap, int arraySize){
	int counter;
    for (counter = 0; counter < arraySize; counter++) {
        printf("%d ", max_heap[counter]);
    }
}

int main(){
    int index, arraySize;
	int *array = calloc(MAX, sizeof(int));
    int *max_heap = calloc(MAX, sizeof(int));
    
    scanf("%d", &arraySize);
    
    for (index = 0; index < arraySize; index++) {
        scanf("%d", &array[index]);
    }
    max_heap = build_max_heap(array, arraySize);
    display(max_heap, arraySize);
}