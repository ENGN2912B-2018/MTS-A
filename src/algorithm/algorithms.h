#pragma once

#include <tuple>
#include <bitset>
#include <cmath>

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


class HuffmanCoding
{

public:

    std::vector< std::vector<bool> > encode(std::vector<int> coefVec)
    {
        std::string HuffmanString;
        std::vector<bool> bitsVec;
        std::vector< std::vector<bool> > HuffmanBitsVec;

        getHuffmanCodes(coefVec, &HuffmanVec_);

        for(int i=0; i<HuffmanVec_.size(); i++)
        {
            HuffmanString = HuffmanVec_[i];

            bitsVec = getBitsVec( HuffmanString );
            HuffmanBitsVec.push_back(bitsVec);
        }

        return HuffmanBitsVec;
    }

    std::vector<int> decode( std::vector< std::vector<bool> > HuffmanBitsVec)
    {
        std::vector<int> coefVec;

        for(int i=0; i<HuffmanBitsVec.size(); i++)
        {
            std::cout << "Coefficient: " << decode(HuffmanBitsVec[i]) << std::endl;
            coefVec.push_back( decode(HuffmanBitsVec[i]) );
        }

        return coefVec;
    }


private:
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

    node* newNode(char coef, unsigned freq)
    {
        node* newNode;
        newNode = new node;

        newNode -> coef_ = coef;
        newNode -> freq_ = freq;
        newNode -> left = newNode -> right = NULL;

        return newNode;
    }

    minHeap* newMinHeap(unsigned capacity)
    {
        minHeap* newHeap;
        newHeap = new minHeap;

        newHeap -> capacity_ = capacity;
        newHeap -> nodeArray = new node* [capacity];

        return newHeap;
    }

    void swapNodes(struct node** node1, struct node** node2)
    {
        struct node* tempNode;

        tempNode = *node1;
        *node1 = *node2;
        *node2 = tempNode;
    }

    void minHeapify(struct minHeap* minHeap, int parentNode)
    {
        int min, left, right;

        min = parentNode;
        left = 2 * parentNode + 1;
        right = 2 * parentNode + 2;

        if(left < minHeap->size_ && minHeap->nodeArray[left]->freq_ < minHeap->nodeArray[parentNode]->freq_){ min = left; }
        if(right < minHeap->size_ && minHeap->nodeArray[right]->freq_ < minHeap->nodeArray[min]->freq_){ min = right; }
        if(min != parentNode)
        {
            swapNodes(&minHeap->nodeArray[min], &minHeap->nodeArray[parentNode]);
            minHeapify(minHeap, min);
        }

    }

    void buildMinHeap(struct minHeap* minHeap)
    {
        int length = minHeap->size_ - 1;

        for(int i = (length-1)/2; i > -1; i--){ minHeapify(minHeap, i); }
    };

    struct node* extraMinNode(struct minHeap* minHeap)
    {
        struct node* minNode;

        minNode = minHeap->nodeArray[0];

        minHeap->nodeArray[0] = minHeap->nodeArray[minHeap->size_ - 1];
        minHeap->size_ -= 1;
        minHeapify(minHeap, 0);

        return minNode;
    }

    void insertNode(struct minHeap* minHeap, struct node* node)
    {
        minHeap->size_ += 1;
        int i = minHeap->size_ - 1;
        // make sure that look up time is log(N).
        while(i && node->freq_ < minHeap->nodeArray[(i-1)/2]->freq_)
        {
            minHeap->nodeArray[i] = minHeap->nodeArray[(i-1)/2];
            i = (i - 1)/2;
        }

        minHeap->nodeArray[i] = node;
    }


    struct minHeap* vecToHeap(std::vector<int> coefVec)
    {
        char coef;
        std::map<char, unsigned> freqTable;

        for(int i=0; i<coefVec.size(); i++)
        {
            coef = static_cast<char>( coefVec[i] );
            if( freqTable.find(coef) != freqTable.end() ){ freqTable[coef] += 1; }
            else{ freqTable[coef] = 1; }
        }

        int i = 0;
        struct minHeap* minHeap = newMinHeap( coefVec.size() );

        for(std::map<char, unsigned>::iterator iter = freqTable.begin(); iter != freqTable.end(); iter++)
        {
            minHeap->nodeArray[i] = newNode( iter->first, iter->second );
            i += 1;
        }

        minHeap->size_ = i;
        buildMinHeap(minHeap);

        return minHeap;
    }

    /*
     Now the Huffman tree algorithm.
        1. Build min heap from Frequency table.
        2. While min heap has more than 1 element, select two nodes with minimum frequency
            and add the sum as a new node.
        3. Return the root of the Huffman tree.
    */
    struct node* buildHuffmanTree(std::vector<int> coefVec)
    {
        struct minHeap* minHeap = vecToHeap(coefVec);

        while(minHeap->size_ > 1)
        {
            struct node *leftChildNode, *rightChildNode;
            struct node* parentNode;

            // left child should have smaller frequency because left traversal adds 0 to the bit stream.
            leftChildNode = extraMinNode(minHeap);
            rightChildNode = extraMinNode(minHeap);
            unsigned sum_freq = leftChildNode->freq_ + rightChildNode->freq_;
            // nodes with mixed chars don't matter so just use one unique char.
            parentNode = newNode('^', sum_freq);
            parentNode->left = leftChildNode;
            parentNode->right = rightChildNode;

            insertNode(minHeap, parentNode);
        }

        return extraMinNode(minHeap);
    }

    void dfs(struct node* root, std::vector<std::string>* vec, std::string str, int i)
    {

      if(root->left)
      {
        str[i] = '0';
        dfs(root->left, vec, str, i+1);
      }

      if(root->right)
      {
        str[i] = '1';
        dfs(root->right, vec, str, i+1);
      }

      if(root->coef_ != '^')
      {
        std::cout << "Coefficient: " << (int)root->coef_ << "  Frequency: " << root->freq_ << "  Huffman Code: " << str << std::endl;
        (*vec).push_back(str);
      }

    }

    void getHuffmanCodes(std::vector<int> coefVec, std::vector<std::string>* HuffmanVector)
    {
        size_t size = coefVec.size();
        std::string HuffmanString(size, ' ');

        HuffmanRoot_ = buildHuffmanTree(coefVec);

        dfs(HuffmanRoot_, HuffmanVector, HuffmanString, 0);
    }

    std::vector<bool> getBitsVec(std::string HuffmanString)
    {
        int i=0;
        std::vector<bool> bitsVec;

        while(HuffmanString[i] != ' ')
        {
            if(HuffmanString[i] == '0'){ bitsVec.push_back(false); }
            else{ bitsVec.push_back(true); }

            i += 1;
         }

        return bitsVec;
    }

    // decode method should take in a bits vector and find the coefficient based on
    // the Huffman tree.
    int decode(std::vector<bool> bitsVec)
    {
        struct node* node = HuffmanRoot_;

        // since Huffman is prefix, decoding is unique and we just traverse through the tree.
        for(int i=0; i<bitsVec.size(); i++)
        {
            if(bitsVec[i] == false){ node = node -> left; }
            else{ node = node -> right; }

        }

        return node->coef_;
    }

    std::vector<std::string> HuffmanVec_;
    struct node* HuffmanRoot_;
};
