# Parallel Compression Through Networks
**Course Information:**

    ENGN 2610B - Scientific Computing in C++
    Fall 2018, Brown University
    December, 2018

**Team Information:**

    Multithreading and Socket Programming - Team A

    Algorithm Engineer:
    Yang Wang, yang_wang1@brown.edu

    Software Architect:
    Luis Aguirre, luis_aguirre@brown.edu

## Introduction


## Algorithms
The full JPEG compression algorithm was implemented independently.
- Discrete Cosine Transform
- Quantization
- Huffman Coding
 
## Network Programming
Client-server model with TCP communication protocol was implemented to transfer Huffman codes through an asynchronous network.

## Software Interface


## Build Instructions and Testing.
The project uses CMake for building and it has been tested on Linux systems.
To build and compile the program, you can run the following from the 'src' directory.
```
mkdir build
cd build
cmake -D CMAKE_CXX_COMPILER=g++ ..
make
```
Then run the executable to use the software.

## Dependencies
The project depends on Boost ASIO library for asynchronous networking and QT library for user interface. It has been tested with Boost 1.63.0 and is expected to work with more recent versions. On the CCV environment you will need to have the Boost module loaded.

## Project Structure
Source code is kept in the `src` directory.
- image folder contains the main image class used for image processing(read, write) and compression algorithms(parallelized discrete cosine transform, quantization, zigzag scan).
- algorithm folder contains the code for statistical analysis(MSE and PSNR analysis) and Huffman coding(build Huffman tree, encode and decode).   
- client folder contains
- gui folder contains
- server folder contains

## Results and Conclusions
## Future Work
* Use QT or boost to handle multithreading instead of using <thread>.
* Implement a ways to shutdown the server in real-time.
* Load JPEG files through OpenCV
* Implement the multithreaded wavelet transform algorithm for JPEG.
* Implement real-time image reconstruction  method for the wavelet transform algorithm
* Design GUI application interface for clients (QT)
* Integrate client application with networking structure to allow for image upload and download

## Author Contributions
