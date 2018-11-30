#pragma once

#include <string>    // for string
#include <vector>    // for vector
#include <cassert>   // for assert
#include <cstddef>   // for size_t
#include <fstream>   // for ifstream, ofstream
#include <iostream>  // for cout, cerr

using std::ios;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

namespace image {

/// Reads a PGM file from a filename and stores the contents as a vector
/// of vectors
vector<vector<unsigned>> readImage(const string &filename) {
  ifstream input(filename);
  assert(input.is_open());

  string type;
  unsigned max = 0;
  unsigned width = 0;
  unsigned height = 0;

  input >> type;
  input >> width;
  input >> height;
  input >> max;

  vector<vector<unsigned>> grid;
  grid.resize(height);
  for (unsigned i = 0; i < height; i++) {
    for (unsigned j = 0; j < width; j++) {
      unsigned val;
      input >> val;
      grid[i].push_back(val);
    }
  }

  input.close();
  return grid;
}


// TODO:
/// Converts an integer values for numbers to double coefficients.
vector<vector<double>> toCoefficients(const vector<vector<unsigned>> img) {
  vector<vector<double>> coeff;
  coeff.resize(img.size());
  for (unsigned i = 0; i < img.size(); i++) {
    for (unsigned j = 0; j < img[i].size(); j++) {
      coeff[i].push_back(i + j);
    }
  }

  return coeff;
}


/// Converts a vector of characters to a 2d grid of coefficients.
/// The dimensions are appended at the front of the character stream.
vector<vector<double>> deserialize(vector<char> &val) {
  unsigned width;
  unsigned height;
  memcpy(&width, &val.at(0), sizeof(unsigned));
  memcpy(&height, &val.at(sizeof(unsigned)), sizeof(unsigned));

  vector<vector<double>> img;
  int start = sizeof(unsigned) * 2;
  for (int i = 0; i < height; i++) { vector<double> row; for (int j = 0; j < width; j++) {
      double number;
      int index = i * width + j;
      memcpy(&number, &val.at(start + index* sizeof(double)), sizeof(double));
      row.push_back(number);
    }
    img.push_back(row);
  }

  return img;
}



/// Converts a grid of coefficients to a vector of characters.
vector<char> serialize(vector<vector<double>> &coeff) {
  vector<char> converted;

  unsigned height = coeff.size();
  unsigned width = coeff[0].size();

  char* width_ptr = reinterpret_cast<char*>(&width);
  char* height_ptr = reinterpret_cast<char*>(&height);
  for (size_t i = 0; i < sizeof(unsigned); i++) converted.push_back(width_ptr[i]);
  for (size_t i = 0; i < sizeof(unsigned); i++) converted.push_back(height_ptr[i]);

  // convert coeff to an array of character
  for (int i = 0; i < coeff.size(); i++) {
    for (int j = 0; j < coeff[i].size(); j++) {
      double v= coeff[i][j];
      char* ptr = reinterpret_cast<char*>(&v);
      for (size_t k = 0; k < sizeof(double); k++) converted.push_back(ptr[k]);
    }
  }

  return converted;
}

} // namespace image
