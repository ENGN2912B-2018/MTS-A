# Parallel Image Compression Through Networks
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

**Copyright:**


## Introduction
Goal of this program is to provide users a parallelized image compression service through an asynchronous network.

## Algorithms
We implemented the following algorithms independently:
- **Parallelized Discrete Cosine Transform (DCT)**

    We implemented a block-wise DCT and assigned each thread an equal amount of blocks to compress/decompress.    
- **Quantization**

    The standard JPEG quantization matrix and quality ratio formalism was used to compress information stored in the high frequency Fourier coefficients.
- **Huffman Coding**

    The coefficient matrix was scanned in a zigzag fashion (we also stop scanning as soon as we find more than five zero coefficients) before being Huffman encoded and vice versa (Huffman decoding then zigzag unpacking).

## Network Programming
Client-server model with TCP communication protocol was implemented to transfer Huffman codes through an asynchronous network.

## Software Interface
Software GUI was developed using the QT library. Main functions of the software includes:
- Load an image file within a given directory into the GUI.
- Choose a quality ratio before compression.
- Compress the file and send the Huffman codes through the network.
- Save the decoded and then decompressed file into a directory.

Following is a screen shot of the GUI, where the left image in the GUI is the original PGM file and the right image in the GUI is the compressed PGM file with quality ratio of 8(adjustable within the GUI).
![alt text](https://drive.google.com/uc?export=view&id=1W-FjME4PIWIParbmmCC2_-gK6O3IPK0G)

## Dependencies and Build Instructions.
The project uses CMake for building and it requires the boost:asio library for asynchronous networking and finally, QT library for the user interface.

We have tested the software on the following systems:

**Linux Ubuntu**
1. Download cmake, boost and qt libraries using apt-get pacakge manager:
```
sudo apt-get install cmake
sudo apt-get install libboost-all-dev
sudo apt-get install qt5-default
```
2. Build and compile the program by running the following commands after cloning this repo:
```
mkdir build
cd build
cmake -D CMAKE_CXX_COMPILER=g++ ..
make
```
3. Run `mainExecutable` created in the `build` directory.

**Brown CCV**
1. ssh into a CCV node with X11 forwarding enabled. i.e. Type in the following command:
```
ssh <account_name>@ssh.ccv.brown.edu -X
```
2. Load boost and qt libraries to the current node.
```
module load gcc/7.2
module load boost/1.63.0
module load qt/5.7.0
```
3. Build and compile the program by running the following commands after cloning this repo:
```
mkdir build
cd build
cmake -D CMAKE_CXX_COMPILER=g++ ..
make
```
4. Run `mainExecutable` created in the `build` directory.

Note that Windows does not support X11 forwarding nor has this project been tested on Windows.

## Project Structure
Source code is kept in the `src` directory.
- Image folder contains the main image class used for image processing (read, write) and image compression (parallelized discrete cosine transform, quantization, zigzag scan).
- Algorithm folder contains the code for statistical analysis (MSE and PSNR analysis) and Huffman coding (build Huffman tree, encode and decode).   
- Client folder contains
- GUI folder contains
- Server folder contains

## Results and Conclusions
![alt text](https://drive.google.com/uc?export=view&id=1DqggjnUs2U_s_xnAFs_48H2J4W07tjgY)
![alt text](https://drive.google.com/uc?export=view&id=11y0lKNbO_Of_JofPzYjP7nRz6S8czA8P)

## Future Work
- Extend to other file formats such as PNG and JPEG.
- Adopt adaptive discrete cosine transform by introducing machine learning algorithms.
- Add peer-to-peer network model.

## Author Contributions
**Luis Aguirre**:     

As the software architect, Luis was responsible for building and testing of the client-server model and the asynchronous network model. He also designed the GUI of the software. Specifically, he wrote the source code for client class, server class and the gui class.

**Yang Wang**:  

As the algorithm engineer, Yang was responsible for designing, implementing and testing the image processing, image compression algorithms. Specifically, he wrote the source code for image compression class, statistical analysis class and Huffman coding class. He also analyzed and quantified the performance of the algorithms (space and time complexities, compression ratio and image quality trade off, sequential compressions etc.) and documented relevant proofs.  
