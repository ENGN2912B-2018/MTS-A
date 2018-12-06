#pragma once

class HuffmanEncoding
{
    struct node
    {
        // data is our quantized coefficient.
        char data;
        unsigned freq;
        struct node* left;
        struct node* right;
    };

    struct minHeap
    {
        char a;
    };

};


class statisticalAnalysis
{
public:

    int MSE(std::vector< std::vector<int> > intMatrix1, std::vector< std::vector<int> > intMatrix2)
    {
        int i, j;

        assert(intMatrix1.size() == intMatrix2.size() && intMatrix1[0].size() == intMatrix2[0].size());

        int mse = 0;

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

private:

};
