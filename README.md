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


## Network Programming


## Software Interface


## Build Instructions
The project uses CMake for compilation on the CCV environment. For the initial setup you can run the following from the source directory.
```
mkdir build
cd build
cmake -D CMAKE_CXX_COMPILER=g++ ..
```
Then compile the project by running `make` in the `build/` directory.

## Dependencies
The project depends on Boost ASIO library for asynchronous networking. It has been tested with Boost 1.63.0 and is expected to work with more recent versions. On the CCV environment you will need to have the Boost module loaded.

## Project Structure
Source code is currently kept in the `src` directory and the file structure will later be expanded as we add more code. In the root directory we keep a `main.cpp` file that can be used to test the program.

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
