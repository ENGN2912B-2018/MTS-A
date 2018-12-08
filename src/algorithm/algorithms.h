#pragma once

#include <tuple>
#include <cmath>

class HuffmanEncoding
{

public:

    HuffmanEncoding(std::vector<int> inputVec)
    {
        std::string tempCoef;
        unsigned tempUnsigned;
        std::tuple<std::string, unsigned> tempTuple;

        freqTable_ = getFreqTable(inputVec);
        std::cout << "\nFrequency Table:\n";
        for(std::map<std::string, unsigned>::iterator iter = freqTable_.begin(); iter != freqTable_.end(); iter++)
        {
            tempCoef = iter -> first;
            tempUnsigned = iter -> second;
            tempTuple = {tempCoef, tempUnsigned};

            std::cout << "Char: " << tempCoef << "   Frequency: " << tempUnsigned << std::endl;
            tupleVec_.push_back(tempTuple);
        }

    }


    std::map<std::string, unsigned> getFreqTable(std::vector<int> inputVec)
    {
        std::string coef;
        std::map<std::string, unsigned> freqTable;

        for(int i=0; i<inputVec.size(); i++)
        {
            coef = std::to_string( inputVec[i] );
            if( freqTable.find(coef) != freqTable.end() ){ freqTable[coef] += 1; }
            else{ freqTable[coef] = 1; }
        }

        return freqTable;
    }

    void minHeapify(int root)
    {
        int i, left, right, min;
        std::tuple<std::string, unsigned> temp;
        // consider the input array as a heap structure.
        min = root;
        left =  2 * root + 1;
        right = 2 * root + 2;

        if(left < tupleVec_.size() && std::get<1>( tupleVec_[left] ) < std::min( std::get<1>( tupleVec_[root] ), std::get<1>( tupleVec_[right] ) ) ){ min = left; }
        else if(right < tupleVec_.size() && std::get<1>( tupleVec_[right] ) < std::min( std::get<1>( tupleVec_[root] ), std::get<1>( tupleVec_[left]) ) ){ min = right; }
        else{ return; }

        temp = tupleVec_[min];
        tupleVec_[min] = tupleVec_[root];
        tupleVec_[root] = temp;

        minHeapify(min);
    }

    void maxHeapify(int root)
    {
        int i, left, right, max, unswapped;
        std::tuple<std::string, unsigned> temp;
        // consider the input array as a heap structure.
        max = root;
        left =  2 * root + 1;
        right = 2 * root + 2;

        if(left < HuffmanTree_.size() && std::get<1>( HuffmanTree_[left] ) > std::max( std::get<1>( HuffmanTree_[root] ), std::get<1>( HuffmanTree_[right] ) ) ){ max = left; unswapped = right; }
        else if(right < HuffmanTree_.size() && std::get<1>( HuffmanTree_[right] ) > std::max( std::get<1>( HuffmanTree_[root] ), std::get<1>( HuffmanTree_[left]) ) ){ max = right; unswapped = left; }
        else{ return; }

        // swap max the rooot.
        temp = HuffmanTree_[max];
        HuffmanTree_[max] = HuffmanTree_[root];
        HuffmanTree_[root] = temp;
        // swap children for binary property.
        if( std::get<1>( HuffmanTree_[max] ) < std::get<1>( HuffmanTree_[unswapped] ) )
        {
            temp = HuffmanTree_[unswapped];
            HuffmanTree_[unswapped] = HuffmanTree_[max];
            HuffmanTree_[max] = temp;
        }
        else{;}

        maxHeapify(max);
    }

    void maxHeapify()
    {
        for(int i=HuffmanTree_.size()-1; i>-1; i--){ maxHeapify(i); }
        std::cout<< "Maximum Frequency: " << std::get<1>(HuffmanTree_[0]) << "  " << std::get<1>(HuffmanTree_[1]) << "  " << std::get<1>(HuffmanTree_[2]) << std::endl;
        std::cout<< "Maximum Frequency: " << std::get<1>(HuffmanTree_[3]) << "  " << std::get<1>(HuffmanTree_[4]) << "  " << std::get<1>(HuffmanTree_[5]) << std::endl;
    }

    void minHeapify()
    {
        for(int i=tupleVec_.size()-1; i>-1; i--){ minHeapify(i); }
        std::cout<< "Minimum Frequency: " << std::get<1>(tupleVec_[0]) << std::endl;
    }

    void getHuffmanTree()
    {
        unsigned freq;
        std::string tempString;
        std::tuple<std::string, unsigned> treeNode, minTuple1, minTuple2;

        HuffmanTree_ = tupleVec_;

        while(tupleVec_.size() > 1)
        {
            minHeapify();
            minTuple1 = tupleVec_[0];
            tupleVec_.erase( tupleVec_.begin() );
            minHeapify();
            minTuple2 = tupleVec_[0];
            tupleVec_.erase( tupleVec_.begin() );

            tempString = std::get<0>(minTuple1) + "," + std::get<0>(minTuple2);
            freq = std::get<1>(minTuple1) + std::get<1>(minTuple2);
            treeNode = {tempString, freq};

            std::cout << "String: " << tempString << "   Frequency: " << freq << std::endl;
            tupleVec_.push_back(treeNode);
            HuffmanTree_.push_back(treeNode);
        }
    }

private:

    std::map<std::string, unsigned> freqTable_;
    std::vector< std::tuple<std::string, unsigned> > tupleVec_;
    std::vector< std::tuple<std::string, unsigned> > HuffmanTree_;
};


class statisticalAnalysis
{
public:

    double MSE(std::vector< std::vector<int> > intMatrix1, std::vector< std::vector<int> > intMatrix2)
    {
        int i, j;

        assert(intMatrix1.size() == intMatrix2.size() && intMatrix1[0].size() == intMatrix2[0].size());

        double mse = 0.0;

        for(i=0; i<intMatrix1.size(); i++)
        {
            for(j=0; j<intMatrix1[0].size(); j++)
            {
                mse += (intMatrix1[i][j] - intMatrix2[i][j]) * (intMatrix1[i][j] - intMatrix2[i][j]);
            }
        }

        mse /= intMatrix1.size() * intMatrix1[0].size();

        return mse;
    }

    double PSNR(int maxInt, double mse)
    {
        double psnr = 10 * log10( maxInt * maxInt / mse);

        return psnr;
    }

private:

};


// Same implementations as heap sort until Huffman tree part.
struct node
{
    char coef_;
    unsigned freq_;

    struct node* left;
    struct node* right;
};

struct minHeap
{
    unsigned size_;
    unsigned capacity_;

    struct node** nodeArray;
};

node* createNode(char coef, unsigned freq)
{
    node* newNode;
    newNode = new node;

    newNode -> coef_ = coef;
    newNode -> freq_ = freq;
    newNode -> left = newNode -> right = NULL;

    return newNode;
};

minHeap* createMinHeap(unsigned capacity)
{
    minHeap* newHeap;
    newHeap = new minHeap;

    newHeap -> capacity_ = capacity;
    newHeap -> nodeArray = new node* [capacity];

    return newHeap;
};

void swapNodes(struct node** node1, struct node** node2)
{
    struct node* tempNode;

    tempNode = *node1;
    *node1 = *node2;
    *node2 = tempNode;
};

void minHeapify(struct minHeap* minHeap, int parentNode)
{
    int min, left, right;

    min = parentNode;
    left = parentNode + 1;
    right = parentNode + 2;

    if(left < minHeap->size_ && minHeap->nodeArray[left]->freq_ < std::min(minHeap->nodeArray[parentNode]->freq_, minHeap->nodeArray[right]->freq_) )
    {
        min = left;
    }
    else if(right < minHeap->size_ && minHeap->nodeArray[right]->freq_ < std::min(minHeap->nodeArray[parentNode]->freq_, minHeap->nodeArray[left]->freq_) )
    {
        min = right;
    }
    else{ return; }

    swapNodes(&minHeap->nodeArray[min], &minHeap->nodeArray[parentNode]);

    minHeapify(minHeap, min);
};

void buildMinHeap(struct minHeap* minHeap)
{
    for(int i=(minHeap->size_-1); i>-1; i--){ minHeapify(minHeap, i); }
};

struct node* extraMinNode(struct minHeap* minHeap)
{
    struct node* minNode;

    minNode = minHeap->nodeArray[0];

    minHeap->nodeArray[0] = minHeap->nodeArray[minHeap->size_ - 1];
    minHeap->size_ -= 1;
    minHeapify(minHeap, 0);

    return minNode;
};

void insertNode(struct minHeap* minHeap, struct node* node)
{
    int i = minHeap->size_ - 1;

    // make sure that look up time is log(N).
    while(node->freq_ < minHeap->nodeArray[(i-1)/2]->freq_)
    {
        minHeap->nodeArray[i] = minHeap->nodeArray[(i-1)/2];
        i = (i - 1)/2;
    }

    minHeap->nodeArray[i] = node;
    minHeap->size_ += 1;
};

struct minHeap* mapToHeap(std::map<char, unsigned> freqTable)
{
    int i = 0;
    struct minHeap* minHeap;

    for(std::map<char, unsigned>::iterator iter = freqTable.begin(); iter != freqTable.end(); iter++)
    {
        minHeap->nodeArray[i] = createNode( iter->first, iter->second );
        i += 1;
    }

    minHeap->size_ = i;
    buildMinHeap(minHeap);

    return minHeap;
};

/*
 Now the Huffman tree algorithm.
    1. Build min heap from Frequency table.
    2. While min heap has more than 1 element, select two nodes with minimum frequency
        and add the sum as a new node.
    3. Return the root of the Huffman tree.
*/
struct node* buildHuffmanTree(std::map<char, unsigned> freqTable)
{
    struct minHeap* = mapToHeap(freqTable);

    while(minHeap->size_ > 1)
    {
        struct node *leftNode, *rightNode;
        struct node* parentNode;

        // left child should have smaller frequency because left traversal adds 0 to the bit stream.
        leftNode = extraMinNode(minHeap);
        rightNode = extraMinNode(minHeap);

        sum_freq = leftNode->freq_ + rightNode->freq_;
        // nodes with mixed chars don't matter so just use one unique char.
        parentNode = newNode('^', sum_freq);

        insertNode(minHeap, parentNode);
    }

    return extraMinNode(minHeap);
}
