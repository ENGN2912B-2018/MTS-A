# Parallel Compression Through Networks
**Course Information:**

    ENGN 2610B - Scientific Computing in C++

    Fall 2018, Brown University

**Team Information:**

    Multithreading and Socket Programming - Team A

    Algorithm Engineer:
    Yang Wang, yang_wang1@brown.edu

    Software Architect:
    Luis Aguirre, luis_aguirre@brown.edu


## Build instructions
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

## Proposed Project Schedule & Milestones
* *Week of November 7th*: Build basic network structures using QT and Boost libraries. e.g. Provision of server and implement basic client-server model. Choose a specific communication protocol and test
  - Task management: both Luis and Yang would work on this first task.
* *Week of November 14th*: Implement the multi-threaded wavelet transform algorithm on the server end so that uploaded JPEG files can be compressed into smaller files. Compression class should have methods such as compression and reconstruction that allow users to choose image quality(signal to noise ratio) of the decompressed image and the size of the compressed file.
  - Task management: Yang would write compression algorithm, Luis would work on thread safety and we would both contribute to the Image class.
* *Week of November 21st*: Implement the real-time image reconstruction method that allows user to visualize reconstructed images on the fly. A more advanced feature would be to store these “semi-reconstructed” images as individual frames and make a video that shows the reconstruction process.
  - Task management: Yang would implement the reconstruction method and Luis would work on real-time image plotting feature.
* *Week of November 28th*: Design the GUI or webpage so that the whole compression service is as user-friendly as possible.
  - Task management: both Luis and Yang would work on this task but Luis is the lead.
* *Week of December 5th*: Implement more complex network structures that can navigate large user traffic while maintaining high speed and high quality services.
  - Task management: both Luis and Yang would work on this task.

## Due Dates
* Project Update 3: December 7th  (11:59pm)
* Final Project Code Repository: December 16th (11:59pm)
* Final Team Project Report: December 18th (11:59pm)
* Final Team Project Presentations: December 20th (5:00pm)

## Backlog

* Use QT or boost to handle multithreading instead of using <thread>.
* Implement a ways to shutdown the server in real-time.
* Load JPEG files through OpenCV
* Implement the multithreaded wavelet transform algorithm for JPEG.
* Implement real-time image reconstruction  method for the wavelet transform algorithm
* Design GUI application interface for clients (QT)
* Integrate client application with networking structure to allow for image upload and download
