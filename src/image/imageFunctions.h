#pragma once

#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

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
    // search through the array in quandratic time to find the k smallest(absolute value) elements.
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
    // return the indices of these smallest elements as coefficients to abandon in the compression process.
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

void heapify(std::vector<int>* quantVec, int root)
{
    int i, left, right, temp, min;
    // consider the input array as a heap structure.
    min = root;
    left =  2 * root + 1;
    right = 2 * root + 2;

    if(left < (*quantVec).size() && (*quantVec)[left] < std::min( (*quantVec)[root], (*quantVec)[right]) ){ min = left; }
    else if(right < (*quantVec).size() && (*quantVec)[right] < std::min( (*quantVec)[root], (*quantVec)[left]) ){ min = right; }
    else{ return; }

    temp = (*quantVec)[min];
    (*quantVec)[min] = (*quantVec)[root];
    (*quantVec)[root] = temp;

    heapify(quantVec, min);
}

std::vector<int> heapify(std::vector<int> quantVec)
{
    for(int i=quantVec.size()-1; i>-1; i--){ heapify(&quantVec, i); }

    return quantVec;
}

void HuffmanEncoding(std::vector<int> quantVec)
{
    // get frequency table from quantized coefficients vector.
    std::map<int, unsigned> freqTable;
    for(int i=0; i<quantVec.size(); i++)
    {
        if( freqTable.find(quantVec[i]) != freqTable.end() ){ freqTable[quantVec[i]] += 1; }
        else{ freqTable[quantVec[i]] = 1; }
    }
    // create Huffman tree from freqTable.
    // 1. create a min heap representation of the data.
    quantVec = heapify(quantVec);
    // 2. take the two min elements and add it as a node to the Huffman tree.


}
