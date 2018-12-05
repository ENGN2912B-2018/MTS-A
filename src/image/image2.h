#pragma once

#include "imageFunctions.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
#include <string>
#include <bitset>
#include <algorithm>
#include <vector>
#include <array>

class Image
{

public:

    Image(std::string const& fileName)
    {
        std::ifstream inputFileStream(fileName, std::ios::in | std::ios::binary);
        std::stringstream fileContent;
        std::string fileType;
        char temp;

        fileContent << inputFileStream.rdbuf();
        fileContent >> fileType;

        //could also check file extension with boost filesystem instead.
        if(fileType != "P5" and fileType != "P2"){ std::cerr << "Wrong file type, please select a pgm file instead." << std::endl; exit(0); }
        fileContent >> fileColumns_ >> fileRows_;
        if(fileColumns_%8 != 0 || fileRows_%8 != 0){ std::cerr << "Incorrect image size." << std::endl; exit(0); }
        fileContent >> maxIntensity_;
        std::cout << "File Type: " << fileType << std::endl;
        std::cout << "Rows: " << fileRows_ << "    " << "Columns: " << fileColumns_ << std::endl;
        std::cout << "Max Intensity: " << maxIntensity_ << std::endl;

        setMatrices(fileRows_, fileColumns_);

        for(int i=0; i<fileRows_; i++)
        {
            for(int j=0; j<fileColumns_; j++)
            {
                fileContent >> temp;
                intMatrix_[i][j] = (uint32_t)(temp);
            }
        }

        inputFileStream.close();
    }

    void saveImage(std::string const& fileName)
    {
        int i, j;
        std::ofstream outputFileStream(fileName, std::ios::out | std::ios::binary);

        if( outputFileStream.is_open() )
        {
            outputFileStream << "P2\n" << fileColumns_ << " " << fileRows_ << "\n" << maxIntensity_ << "\n";

            for(i=0; i<fileRows_; i++)
            {
                for(j=0; j<fileColumns_; j++)
                {
                    outputFileStream << intMatrix_[i][i] << " ";
                }
            }
        }

        outputFileStream.close();
    }

    void setMatrices(int row, int column)
    {
        intMatrix_.resize(row);
        for(int i=0; i<intMatrix_.size(); i++)
        {
            intMatrix_[i].resize(column);
        }

        coefMatrix_.resize(row);
        for(int i=0; i<coefMatrix_.size(); i++)
        {
            coefMatrix_[i].resize(column);
        }
    }

    void quantization(unsigned int qRatio = 0)
    {
        int i, j;

        if(qRatio != 0 && qRatio <= 50)
        {
            for(i=0; i<qMatrix.size(); i++)
            {
                for(j=0; j<qMatrix[0].size(); j++)
                {
                    qMatrix[i][j] = (int)( qMatrix[i][j] * (50/qRatio) );
                }
            }
        }

        else if(qRatio != 0 && qRatio > 50)
        {
            for(i=0; i<qMatrix.size(); i++)
            {
                for(j=0; j<qMatrix[0].size(); j++)
                {
                    qMatrix[i][j] = (int)( qMatrix[i][j] * (100-qRatio)/50 );
                }
            }
        }

        for(i=0; i < fileRows_/8; i++)
        {
            for(j=0; j < fileColumns_/8; j++)
            {
                quantization(8*i, 8*j, qRatio);
            }
        }

    }

    void quantization(unsigned start_row, unsigned start_column, unsigned int qRatio)
    {
        int i, j, m, n;

        if(qRatio != 0 && qRatio <= 50)
        {
            for(i=start_row; i < start_row + blockSize_; i++)
            {
                for(j=start_column; j < start_column + blockSize_; j++)
                {
                    m = i%8;
                    n = j%8;
                    coefMatrix_[i][j] = (int)( coefMatrix_[i][j]/qMatrix[m][n] );
                    coefMatrix_[i][j] *= qMatrix[m][n];
                }
            }
        }

        else if(qRatio != 0 && qRatio > 50)
        {
            for(i=start_row; i < start_row + blockSize_; i++)
            {
                for(j=start_column; j < start_column + blockSize_; j++)
                {
                    m = i%8;
                    n = j%8;
                    coefMatrix_[i][j] = (int)( coefMatrix_[i][j]/qMatrix[m][n] );
                    coefMatrix_[i][j] *= qMatrix[m][n];
                }
            }
        }

    }

    void compress(unsigned int qRatio)
    {
        dct();
        quantization(qRatio);
    }

    void decompress()
    {
        idct();
    }

    void decompress(std::vector< std::vector<double> > coefMatrix)
    {
        idct(coefMatrix);
    }

    std::vector< std::vector<int> > qMatrix = { {16, 11, 10, 16, 24, 40, 51, 61}, {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56}, {14, 17, 22, 29, 51, 87, 80, 62}, {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92}, {49, 64, 78, 87, 103, 121, 120, 101}, {72, 92, 95, 98, 112, 100, 103, 99} };
    std::vector< std::vector<double> > intMatrix_;
    std::vector< std::vector<double> > coefMatrix_;

private:

    void dct()
    {
        int i, j;
        for(i=0; i < fileRows_/8; i++)
        {
            for(j=0; j < fileColumns_/8; j++)
            {
                dct(8*i, 8*j);
            }
        }

    }

    void dct(unsigned start_row, unsigned start_column)
    {
        int i, j, m, n;
        double c_i, c_j, sum;

        for(i=start_row; i < start_row + blockSize_; i++)
        {
            for(j=start_column; j < start_column + blockSize_; j++)
            {
                sum = 0.0;

                if(i%8 == 0){ c_i = std::sqrt(1.0/blockSize_); }
                else{ c_i = std::sqrt(2.0/blockSize_); }

                if(j%8 == 0){ c_j = std::sqrt(1.0/blockSize_); }
                else{ c_j = std::sqrt(2.0/blockSize_); }

                for(m=start_row; m < start_row + blockSize_; m++)
                {
                    for(n=start_column; n < start_column + blockSize_; n++)
                    {
                        sum += intMatrix_[m][n] * std::cos( (2.0 * (m%8) + 1) * (i%8) * pi / (2.0 * blockSize_) ) * std::cos( (2.0 * (n%8) + 1) * (j%8) * pi / (2.0 * blockSize_) );
                    }
                }

            coefMatrix_[i][j] = c_i * c_j * sum;
            }
        }

    }

    void idct()
    {
        int i, j;
        for(i=0; i < fileRows_/8; i++)
        {
            for(j=0; j < fileColumns_/8; j++)
            {
                idct(8*i, 8*j);
            }
        }
    }

    void idct(unsigned start_row, unsigned start_column)
    {
        int i, j, m, n;
        double c_m, c_n, sum;

        for(i=start_row; i < start_row + blockSize_; i++)
        {
            for(j=start_column; j < start_column + blockSize_; j++)
            {
                sum = 0.0;

                for(m=start_row; m < start_row + blockSize_; m++)
                {
                    for(n=start_column; n < start_column + blockSize_; n++)
                    {
                        if(m%8 == 0){ c_m = std::sqrt(1.0/blockSize_); }
                        else{ c_m = std::sqrt(2.0/blockSize_); }

                        if(n%8 == 0){ c_n = std::sqrt(1.0/blockSize_); }
                        else{ c_n = std::sqrt(2.0/blockSize_); }

                        sum += c_m * c_n * coefMatrix_[m][n] * std::cos( (2.0 * (i%8) + 1) * (m%8) * pi / (2.0 * blockSize_) ) * std::cos( (2.0 * (j%8) + 1) * (n%8) * pi / (2.0 * blockSize_) );
                    }
                }

            intMatrix_[i][j] = (int)(sum);
            }
        }

    }

    void idct(std::vector< std::vector<double> > coefMatrix)
    {
        int i, j;
        for(i=0; i < fileRows_/8; i++)
        {
            for(j=0; j < fileColumns_/8; j++)
            {
                idct(8*i, 8*j, coefMatrix);
            }
        }
    }

    void idct(unsigned start_row, unsigned start_column, std::vector< std::vector<double> > coefMatrix)
    {
        int i, j, m, n;
        double c_m, c_n, sum;

        for(i=start_row; i < start_row + blockSize_; i++)
        {
            for(j=start_column; j< start_column + blockSize_; j++)
            {
                sum = 0.0;

                for(m=start_row; m < start_row + blockSize_; m++)
                {
                    for(n=start_column; n < start_column + blockSize_; n++)
                    {
                        if(m%8 == 0){ c_m = std::sqrt(1.0/blockSize_); }
                        else{ c_m = std::sqrt(2.0/blockSize_); }

                        if(n%8 == 0){ c_n = std::sqrt(1.0/blockSize_); }
                        else{ c_n = std::sqrt(2.0/blockSize_); }

                        sum += c_m * c_n * coefMatrix[m][n] * std::cos( (2.0 * (i%8) + 1) * (m%8) * pi / (2.0 * blockSize_) ) * std::cos( (2.0 * (j%8) + 1) * (n%8) * pi / (2.0 * blockSize_) );
                    }
                }

            intMatrix_[i][j] = (int)(sum);
            }
        }

    }

    double pi = 3.14159265358979324;
    unsigned fileRows_, fileColumns_, maxIntensity_;
    const unsigned blockSize_ = 8;

};
