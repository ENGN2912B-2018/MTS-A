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

## Introduction
The goal of this project is to develop a parallelized image compression service that allows for image transfer through an asynchronous network.

## Algorithms
We implemented the following algorithms independently:
- **Parallelized Discrete Cosine Transform (DCT)**

    We implemented a block-wise DCT and assigned each thread an equal amount of blocks to compress/decompress.
- **Quantization**

    The standard JPEG quantization matrix and quality ratio formalism was used to compress information stored in the high frequency Fourier coefficients.
- **Huffman Coding**

The coefficient matrix was scanned in a zigzag fashion (we also stop scanning as soon as we find more than five zero coefficients) before being Huffman encoded and vice versa (Huffman decoding then zigzag unpacking).

Following figure shows the compressed images with compression of quality ratio 1, 2, 5, 10, 20, 50, 90, respectively.
![alt text](https://drive.google.com/uc?export=view&id=1mCTFwoP2i-_JYhazqwzel1KjXjQ5uaH6)

## Network Programming
We implemented a Client-Server model using the TCP communication protocol to transfer Huffman codes through an asynchronous network. The Client-Server model allows for simple message transfer and can be easily expanded upon to allow for varying Client and Server operations.

## Software Interface
The software GUI was developed using the Qt5 Widgets library and connects the other components of the project together. The main functions of the application includes:
- Load an image file within a given directory into the GUI.
- Choose a quality ratio before compression.
- Compress the file and send the Huffman codes through the network to a central server.
- Save the decoded and then decompressed file into a local directory.

The following is a screen shot of the GUI, where the left image is the original PGM file and the right image is the compressed PGM file with quality ratio of 17 (adjustable within the GUI).

![alt text](https://drive.google.com/uc?export=view&id=1Z7Rzw5FhtaiJNbFLtEGJqIbqc0Bf_4w3)

## Dependencies and Build Instructions.
The project is built with CMake and requires Boost.Asio (v1.63.0) for asynchronous networking and the Qt (v5.10.1) for the user interface.

We have tested the software on the following platforms:

**Linux Ubuntu**
1. Install CMake, Boost and Qt using the `apt-get` pacakge manager:
```
sudo apt-get install cmake
sudo apt-get install libboost-all-dev
sudo apt-get install qt5-default
```
2. Build and compile the project by running the following commands after cloning this repo:
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
2. Load the Boost and Qt libraries to the current environment.
```
module load gcc/7.2
module load boost/1.63.0
module load qt/5.10.0
```
3. Build and compile the project by running the following commands after cloning this repo:
```
mkdir build
cd build
cmake -D CMAKE_CXX_COMPILER=g++ ..
make
```
4. Run `mainExecutable` located in the `build` directory.

Note that Windows does not support X11 forwarding by default, nor has this project been tested on Windows.

## Project Structure
The source code is kept in the `src` directory and split into the following subdirectories:
- `image`: The image folder contains the main Image class used for image processing (reading & writing) and compression (parallelized discrete cosine transform, quantization, zigzag scan).
- `algorithm`: The algorithm folder contains the code for our statistical analysis (MSE and PSNR analysis) and performing Huffman encoding and decoding.
- `client`: The client folder contains our representation of the client in the Client-Server model. The Client class uses Boost.Asio to allow for message sending to and from the server.
- `server`: The server folder contains our representation of the server in the Client-Server model. The Server class uses Boost.Asio to accept multiple client connections and send messages to and from the client.
- `gui`: The gui folder contains the code for the main client (GUI) application. The GUI application connects the networking both the Algorithm and Networking components of the project together through a user interface built with the Qt5 Library.

## Results and Conclusions
![alt text](https://drive.google.com/uc?export=view&id=1DqggjnUs2U_s_xnAFs_48H2J4W07tjgY)
![alt text](https://drive.google.com/uc?export=view&id=11y0lKNbO_Of_JofPzYjP7nRz6S8czA8P)

## Future Work
- Extend the application to support multiple file formats, such as PNG and JPEG.
- Adopt an adaptive discrete cosine transform by introducing machine learning into the image compression.
- Implement a peer-to-peer network model in order to allow for compressed image sharing.

## Author Contributions
**Luis Aguirre**:

As the software architect, Luis was responsible for building and testing of the client-server model and the asynchronous network model. He also implemented the GUI application of the software and combined each component of the project to work together in the final application. Specifically, he wrote the source code for the client class, the server class and the gui class.

**Yang Wang**:

As the algorithm engineer, Yang was responsible for designing, implementing and testing the image processing, image compression algorithms. Specifically, he wrote the source code for image compression class, statistical analysis class and Huffman coding class. He also analyzed and quantified the performance of the algorithms (space and time complexities, compression ratio and image quality trade off, sequential compressions etc.) and documented relevant proofs.
