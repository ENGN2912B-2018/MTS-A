#pragma once

#include "imageFunctions.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <array>

class Image
{

public:
    Image(unsigned row, unsigned column)
    {
      row_=row;
      column_=column;
      data_ = new double* [row_];
      for(int i=0; i<row_; i++){ data_[i] = new double [column_]; }
      for(int i=0; i<row_; i++){ for(int j=0; j<column_; j++){ data_[i][j]=0; } }
    }

    Image(std::string const& filename)
    {
      std::ifstream inputFileStream(filename, std::ios::in);
      std::stringstream fileContent;
      std::string fileType;
      double maxIntensity_;

      fileContent << inputFileStream.rdbuf();
      fileContent >> fileType;

      //could also check file extension with boost filesystem instead.
      if(fileType != "P2"){ std::cerr << "Wrong file type, please select a pgm file instead." << std::endl; exit(0); }
      fileContent >> column_ >> row_;
      fileContent >> maxIntensity_;

      data_ = new double* [row_];
      for(int i=0; i<row_; i++){ data_[i] = new double [column_]; }
      for(int i=0; i<row_; i++){ for(int j=0; j<column_; j++){ fileContent >> data_[i][j]; } }

      inputFileStream.close();
  }

    ~Image(){ for(int i=0; i<row_; i++){ delete[] data_[i]; } delete[] data_; }

    double* compress(unsigned int compressionRatio, unsigned int blockSize=8)
    {
        Image coefImage = dct();
        // flatten 2d array into 1d array.
        double* compressedCoef = new double[blockSize * blockSize];
        double* flattenedArray = flattenArray(coefImage.data_, blockSize);
        std::vector<int> quantVec;
        // determine quant matrix based on compression ratio.
        if(compressionRatio != 0 && compressionRatio < 50){ for(int i=0; i<quantMatrix.size(); i++){ quantMatrix[i] *= (int)(50/compressionRatio); } }
        if(compressionRatio != 0 && compressionRatio > 50){ for(int i=0; i<quantMatrix.size(); i++){ quantMatrix[i] *= (int)((100-n)/compressionRatio); } }
        // perform quantization if user requires compression.
        if(compressionRatio != 0){ for(int i=0; i<quantMatrix.size(); i++){ flattenedArray[i] = (int)( flattenedArray[i]/quantMatrix[i] ); quantVec.push_back(flattenedArray[i]); } }

        if(compressionRatio != 0){ for(int i=0; i< blockSize * blockSize; i++){ compressedCoef[i] = flattenedArray[i] * quantMatrix[i]; } }
        else{ for(int i=0; i< blockSize * blockSize; i++){ compressedCoef[i] = flattenedArray[i]; } }

        delete[] flattenedArray;
        return compressedCoef;
    }

    Image compress()
    {
        Image coefImage = dct();
        return coefImage;
    }

    std::vector<double> decompress(double* array, unsigned int blockSize)
    {
        int i, j;
        std::vector<double> uncompressedIntensities;

        double** expandedArray = expandArray(array, blockSize);
        Image pixelImage = idct(expandedArray);

        for(i=0; i<blockSize; i++){
            for(j=0; j<blockSize; j++){
                uncompressedIntensities.push_back( (int)(pixelImage.data_[i][j]) );
            }
        }

        return uncompressedIntensities;
    }

    Image decompress()
    {
        Image pixelImage = idct();
        return pixelImage;
    }

    double** data_;
    std::vector<int> quantMatrix = {16, 11, 10, 16, 24, 40, 51, 61, 12, 12, 14, 19, 26,
    58, 60, 55, 14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62, 18, 22,
    37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64, 78, 87, 103, 121,
    120, 101, 72, 92, 95, 98, 112, 100, 103, 99};
    std::vector< std::vecotr<int> > pixInt;
    std::vector< std::vector<double> > pixCoef;

private:

    Image dct()
    {
      Image coefImage(row_, column_);
      int i, j, m, n;
      double c_i, c_j, sum;

      for(i=0; i<row_; i++){
        for(j=0; j<column_; j++){

            sum = 0.0;

            if(i==0){ c_i = std::sqrt(1.0/row_); }
            else{ c_i = std::sqrt(2.0/row_); }

            if(j==0){ c_j = std::sqrt(1.0/column_); }
            else{ c_j = std::sqrt(2.0/column_); }

            for(m=0; m<row_; m++){
              for(n=0; n<column_; n++){
                  sum += data_[m][n] * std::cos( (2.0 * m + 1) * i * pi / (2.0 * row_) ) * std::cos( (2.0 * n + 1) * j * pi / (2.0 * column_) );
            } }

            coefImage.data_[i][j] = c_i * c_j * sum;
      } }

      return coefImage;
    }

    Image idct()
    {
      Image pixelImage(row_, column_);
      int i, j, m, n;
      double c_m, c_n, sum;

      for(i=0; i<row_; i++){
        for(j=0; j<column_; j++){

            sum = 0.0;

            for(m=0; m<row_; m++){
              for(n=0; n<column_; n++){

                  if(m==0){ c_m = std::sqrt(1.0/row_); }
                  else{ c_m = std::sqrt(2.0/row_); }

                  if(n==0){ c_n = std::sqrt(1.0/column_); }
                  else{ c_n = std::sqrt(2.0/column_); }

                  sum += c_m * c_n * data_[m][n] * std::cos( (2.0 * i + 1) * m * pi / (2.0 * row_) ) * std::cos( (2.0 * j + 1) * n * pi / (2.0 * column_) );
            } }

            pixelImage.data_[i][j] =  sum;
        }
      }

      return pixelImage;
    }

    Image idct(double** data)
    {
      Image pixelImage(row_, column_);
      int i, j, m, n;
      double c_m, c_n, sum;

      for(i=0; i<row_; i++){
        for(j=0; j<column_; j++){

            sum = 0.0;

            for(m=0; m<row_; m++){
              for(n=0; n<column_; n++){

                  if(m==0){ c_m = std::sqrt(1.0/row_); }
                  else{ c_m = std::sqrt(2.0/row_); }

                  if(n==0){ c_n = std::sqrt(1.0/column_); }
                  else{ c_n = std::sqrt(2.0/column_); }

                  sum += c_m * c_n * data[m][n] * std::cos( (2.0 * i + 1) * m * pi / (2.0 * row_) ) * std::cos( (2.0 * j + 1) * n * pi / (2.0 * column_) );
            } }

            pixelImage.data_[i][j] =  sum;
        }
      }

      return pixelImage;
    }

    double pi = 3.14159265358979324;
    unsigned row_;
    unsigned column_;

};
