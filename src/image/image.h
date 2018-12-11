#pragma once

#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
#include <string>
#include <bitset>
#include <algorithm>
#include <vector>
#include <array>
#include "algorithm/algorithms.h"
#define pi 3.14159265358979324

class Image
{

public:

    Image();

    Image(std::string fileName, bool binaryFlag){ readFile(fileName, binaryFlag); }

    void readImage(std::string fileName, bool binaryFlag){ readFile(fileName, binaryFlag); }

    void saveImage(std::string fileName, bool binaryFlag){ saveFile(fileName, binaryFlag); }

    void sequentialCompression(std::string destinationFolder, bool binaryFlag){ seqComp(destinationFolder, binaryFlag); }

    void compress(unsigned int qRatio)
    {
        std::cout << "Compressing file with quality ratio of " << qRatio << "...\n" << std::endl;
        dct();
        quantization(qRatio);
        std::cout << "File compression completed.\n" << std::endl;
    }

    void decompress(){ idct(); }

    void decompress(std::vector< std::vector<double> > coefMatrix){ idct(coefMatrix); }

    std::vector<std::vector<bool>> HuffmanEncode()
    {
        std::vector<std::vector<bool>> HuffmanVec;

        zigzagScan();
        HuffmanVec = Huffman_.encode(coefVector_);

        return HuffmanVec;
    }

    void HuffmanDecode(std::vector<std::vector<bool>> HuffmanVec)
    {
        std::vector<int> decodedVec;

        decodedVec = Huffman_.decode(HuffmanVec);
        unzigzag(decodedVec);
    }

private:

    void setMatrices(int row, int column)
    {
        int i, j;

        intMatrix_.resize(row);
        for(i=0; i<row; i++)
        {
            intMatrix_[i].resize(column);
        }

        compIntMatrix_.resize(row);
        for(i=0; i<row; i++)
        {
            compIntMatrix_[i].resize(column);
        }

        coefMatrix_.resize(row);
        for(i=0; i<row; i++)
        {
            coefMatrix_[i].resize(column);
        }

    }

    void readFile(std::string fileName, bool binaryFlag)
    {
        if(binaryFlag == true)
        {
            std::ifstream inputFile(fileName, std::ios::in | std::ios::binary | std::ios::ate);
            std::streampos fileSize;
            std::vector<char> temp;
            char * memBlock;
            uint8_t pixelValue;
            int i, j;
            int k = 0;

            if( inputFile.is_open() )
            {
                fileName_ = fileName;
                fileSize = inputFile.tellg();
                std::cout << "\nFile Size(bytes): " << fileSize << std::endl;
                memBlock = new char [fileSize];
                inputFile.seekg(0, std::ios::beg);
                inputFile.read(memBlock, fileSize);

                while(memBlock[k] != '\n'){ temp.push_back(memBlock[k]); k += 1; }
                std::string fileType( temp.begin(), temp.end() );
                if(fileType != "P5"){ std::cerr << "Wrong file type, please select a binary pgm file instead." << std::endl; exit(0); }
                temp.clear();
                k += 1;

                while(memBlock[k] != ' '){ temp.push_back(memBlock[k]); k += 1; }
                std::string fileColumns( temp.begin(), temp.end() );
                fileColumns_ = std::stoi(fileColumns, nullptr);
                temp.clear();
                k += 1;

                while(memBlock[k] != '\n'){ temp.push_back(memBlock[k]); k += 1; }
                std::string fileRows( temp.begin(), temp.end() );
                fileRows_ = std::stoi(fileRows, nullptr);
                temp.clear();
                k += 1;

                while(memBlock[k] != '\n'){ temp.push_back(memBlock[k]); k += 1; }
                std::string maxIntensity( temp.begin(), temp.end() );
                maxIntensity_ = std::stoi(maxIntensity, nullptr);
                k += 1;

                std::cout << "File Type: " << fileType << std::endl;
                std::cout << "Columns: " << fileColumns_ << "   Rows: " << fileRows_ << std::endl;
                std::cout << "Max Intensity: " << maxIntensity_ << "\n" << std::endl;

                setMatrices(fileRows_, fileColumns_);

                for(i=0; i<fileRows_; i++)
                {
                    for(j=0; j<fileColumns_; j++)
                    {
                        pixelValue = memBlock[k];
                        intMatrix_[i][j] = (int)pixelValue;
                        k += 1;
                    }
                }

                delete[] memBlock;
                inputFile.close();
            }
            else{ std::cerr << "Could not open file." << std::endl; }
        }
        // if not binary, read as an ASCII file.
        else
        {
            std::ifstream inputFile(fileName, std::ios::in);

            if( inputFile.is_open() )
            {
                fileName_ = fileName;
                std::stringstream fileContent;
                std::string fileType;

                fileContent << inputFile.rdbuf();
                fileContent >> fileType;

                //could also check file extension with boost filesystem instead.
                if(fileType != "P2"){ std::cerr << "Wrong file type, please select an ASCII pgm file instead." << std::endl; exit(0); }
                fileContent >> fileColumns_ >> fileRows_;
                if(fileColumns_%8 != 0 || fileRows_%8 != 0){ std::cerr << "Incorrect image size." << std::endl; exit(0); }
                fileContent >> maxIntensity_;
                std::cout << "\n\nFile Type: " << fileType << std::endl;
                std::cout << "Rows: " << fileRows_ << "    " << "Columns: " << fileColumns_ << std::endl;
                std::cout << "Max Intensity: " << maxIntensity_ << "\n\n";

                setMatrices(fileRows_, fileColumns_);

                for(int i=0; i<fileRows_; i++)
                {
                    for(int j=0; j<fileColumns_; j++)
                    {
                        fileContent >> intMatrix_[i][j];
                    }
                }

                inputFile.close();
            }
        }
    }

    void saveFile(std::string fileName, bool binaryFlag)
    {
        int i, j;

        if(binaryFlag == true)
        {
            unsigned char bits;
            std::ofstream outputFileStream(fileName, std::ios::out | std::ios::binary);

            outputFileStream << "P5\n" << fileColumns_ << " " << fileRows_ << "\n" << maxIntensity_ << "\n";
            for(i=0; i<fileRows_; i++){ for(j=0; j<fileColumns_; j++){ bits = compIntMatrix_[i][j]; outputFileStream << bits; } }

            outputFileStream.close();
        }

        else
        {
            std::ofstream outputFileStream(fileName, std::ios::out);

            outputFileStream << "P2\n" << fileColumns_ << " " << fileRows_ << "\n" << maxIntensity_ << "\n";
            for(i=0; i<fileRows_; i++){ for(j=0; j<fileColumns_; j++){ outputFileStream << compIntMatrix_[i][j] << " "; } }

            outputFileStream.close();
        }

    }

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

    // performs zigzag scan on a single block of quantized coefficients and return the vector.
    std::vector<int> zigzagScan(unsigned startingRow, unsigned startingColumn)
    {
        int j, coefficient;
        int zeroCount = 0;
        int i = startingRow, k = startingRow;

        std::vector<int> zigzagCoefVec;

        while(k < startingRow + blockSize_)
        {
            while(i <= k)
            {
                j = k - i + startingColumn;
                coefficient = coefMatrix_[i][j];

                if(coefficient == 0)
                {
                    // stop scanning as soon as we get five zero coefficients. Use 2220 to
                    // represent end of block since the theoretical max cofficient is 2040.
                    if(zeroCount >= 5)
                    {
                        zigzagCoefVec.push_back(2220);
                        return zigzagCoefVec;
                    }
                    else
                    {
                        zigzagCoefVec.push_back(0);
                        zeroCount += 1;
                        i += 1;
                    }

                }
                else
                {
                    zigzagCoefVec.push_back( coefficient );
                    i += 1;
                }

            }

            k += 1;
            i = startingRow;
        }

        return zigzagCoefVec;
    }

    // perform zigzag scan for all quantized coefficients blocks and save the result to
    // coefVector_ before performing Huffman encoding.
    void zigzagScan()
    {
        int i, j, k;

        std::vector<int> zigzagCoefVec;

        for(i=0; i < fileRows_/8; i++)
        {
            for(j=0; j < fileColumns_/8; j++)
            {
                zigzagCoefVec = zigzagScan(8*i, 8*j);

                for(k=0; k < zigzagCoefVec.size(); k++){ coefVector_.push_back( zigzagCoefVec[k] ); }
            }
        }

    }

    void unzigzag(std::vector<int> decodedVec)
    {
        // Perform 0 padding to all the blocks.
        int i, j, k, l, m;
        int hBlockCount, startingRow, startingColumn;

        std::vector<int> coefVec;
        std::vector< std::vector<int> > inputCoefMatrix;

        for(int i=0; i < decodedVec.size(); i++)
        {
            if(decodedVec[i] == 2220)
            {
                // resize a vector initializes the empty elements to 0 anyway.
                coefVec.resize(64);
                inputCoefMatrix.push_back(coefVec);
                coefVec.clear();
            }
            else{ coefVec.push_back(decodedVec[i]); }

        }

        // Unpack the coefficients in zigzag fashion as before.
        hBlockCount = fileColumns_/8;

        for(k = 0; k < inputCoefMatrix.size(); k++)
        {
            startingRow = (k / hBlockCount) * 8;
            startingColumn = (k % hBlockCount) * 8;

            i = startingRow;
            l = startingRow;
            m = 0;

            while(l < startingRow + blockSize_)
            {
                while(i <= l)
                {
                    j = l - i + startingColumn;
                    coefMatrix_[i][j] = inputCoefMatrix[k][m];

                    m += 1;
                    i += 1;
                }

                l += 1;
                i = startingRow;
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

    // performs inverse discrete cosine transform on a single block defined by the starting row and starting column.
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

            compIntMatrix_[i][j] = (int)(sum);
            }
        }

    }

    // performs idct on a input coefMatrix.
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

            compIntMatrix_[i][j] = (int)(sum);
            }
        }

    }

    // compress the image sequentially by incrementing quality ratio 10 at a time and save all the images in a destination folder.
    // error analysis is also performed.
    void seqComp(std::string compressedFileFolder, bool binaryFlag)
    {
        std::string compressedFileName;
        std::vector< std::vector<int> > originalInt, compressedInt;

        for(int i=0; i<100; i+=10)
        {
            compress(i);
            decompress();

            if(binaryFlag == true)
            {
                compressedFileName = compressedFileFolder + std::to_string(i) + ".binary.pgm";

                double mse = stats_.MSE(intMatrix_, compIntMatrix_);
                std::cout << "\nqRatio: "  << i << "   MSE: " << mse << std::endl;

                double psnr = stats_.PSNR(maxIntensity_, mse);
                std::cout << "\nqRatio: "  << i << "   PSNR: " << psnr << std::endl;

                saveImage(compressedFileName, true);
            }
            else
            {
                compressedFileName = compressedFileFolder + std::to_string(i) + ".ascii.pgm";

                double mse = stats_.MSE(intMatrix_, compIntMatrix_);
                std::cout << "\nqRatio: "  << i << "   MSE: " << mse << std::endl;

                double psnr = stats_.PSNR(maxIntensity_, mse);
                std::cout << "\nqRatio: "  << i << "   PSNR: " << psnr << std::endl;

                saveImage(compressedFileName, false);
            }

        }

    }


    // file information.
    std::string fileName_;
    unsigned fileRows_, fileColumns_, maxIntensity_;
    // size of minimum compression unit in terms of number of pixels.
    const unsigned blockSize_ = 8;
    // External classes used.
    HuffmanCoding Huffman_;
    statisticalAnalysis stats_;
    // Zigzag scanned vector of coefficients to be Huffman encoded.
    std::vector<int> coefVector_;
    // matrix of pixel intensities, compressed intensities and coefficients.
    std::vector< std::vector<int> > intMatrix_;
    std::vector< std::vector<int> > compIntMatrix_;
    std::vector< std::vector<double> > coefMatrix_;
    // JEPG standard quantization matrix.
    std::vector< std::vector<int> > qMatrix = { {16, 11, 10, 16, 24, 40, 51, 61}, {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56}, {14, 17, 22, 29, 51, 87, 80, 62}, {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92}, {49, 64, 78, 87, 103, 121, 120, 101}, {72, 92, 95, 98, 112, 100, 103, 99} };
};
