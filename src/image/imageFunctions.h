#pragma once

#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>

int partition(double* array, int left, int right)
{
    int pivotIndex = right;
    double pivot = array[right];
    double temp;

    while(left <= right)
    {
        if(array[left] < pivot){ left += 1; continue; }
        if(array[right] >= pivot){ right -= 1; continue; }

        if(left == pivotIndex){ pivotIndex = right; }
        if(right == pivotIndex){ pivotIndex = left; }

        temp = array[left];
        array[left] = array[right];
        array[right] = temp;
    }

    temp = array[left];
    array[left] = array[pivotIndex];
    array[pivotIndex] = temp;

    return left;
}

void quickSort(double* array, int left, int right)
{
    if(left < right)
    {
        double boundary = partition(array, left, right);

        quickSort(array, left, boundary-1);
        quickSort(array, boundary+1, right);
    }

}

double* quickSort(double* array, int blockSize)
{
    int left = 0;
    int right = blockSize*blockSize - 1;

    quickSort(array, left, right);

    return array;
}

std::vector<int> getCompressionIndeces(double* array, unsigned int blockSize, int compressionLength)
{
    std::vector<int> compressionIndeces;
    double min;
    int i, j, minIndex;

    for(i=0; i<compressionLength; i++){
        min = abs(array[0]);
        minIndex = 0;
        for(j=0; j < blockSize * blockSize; j++){
            if(abs(array[j]) < min && std::find(compressionIndeces.begin(), compressionIndeces.end(), j) == compressionIndeces.end() )
            {
                minIndex = j;
                min = array[minIndex];
            }
        }
        compressionIndeces.push_back(minIndex);

    }
    return compressionIndeces;
}

double* flattenArray(double** array, unsigned int blockSize)
{
    int i, j;
    int k = 0;
    double* flattenedArray = new double[blockSize*blockSize];

    for(i=0; i<blockSize; i++){
        for(j=0; j<blockSize; j++){
            flattenedArray[k] = array[i][j];
            k += 1;
    }   }

    return flattenedArray;
}

double** expandArray(double* array, unsigned int blockSize)
{
    int i, j;
    int k = 0;
    double** expandedArray = new double* [blockSize];
    for(i=0; i<blockSize; i++){ expandedArray[i] = new double[blockSize]; }

    for(i=0; i<blockSize; i++){
        for(j=0; j<blockSize; j++){
            expandedArray[i][j] = array[k];
            k += 1;
        }
    }

    return expandedArray;
}
