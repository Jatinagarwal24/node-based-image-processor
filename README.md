

---

# Node-Based Image Processing

## Project Overview

This project is a node-based interface for image manipulation developed in C++. It allows users to load images, process them through a series of interconnected nodes, preview changes in real time, and save the final output. Inspired by node-based editors like Substance Designer, the application offers a visual, modular approach to applying various image processing operations without relying on imnodes.

The interface consists of four main windows:
- **File Operations:** For opening and saving images.
- **Node Selection:** To choose from a list of processing nodes.
- **Properties:** To adjust the parameters of the selected node.
- **Preview:** To view the result of the processing pipeline.

## Features Implemented

### Core Functionality
- **Graphical User Interface:**  
  The application uses [ImGui](https://github.com/ocornut/imgui) with OpenGL (via GLFW) to render the interface. The design includes:
  - **File Operations:**  
    - Open image files from the file system (supports JPG, PNG, BMP).
    - Save processed images with selectable format and quality settings.
  - **Real-Time Image Processing:**  
    The image processing is structured as a series of nodes:
    - **Image Input Node:** Loads an image and displays metadata (dimensions, file size, format).
    - **Brightness/Contrast Node:** Adjusts brightness (-100 to +100) and contrast (0 to 3) with reset functionality.
    - **Color Channel Splitter Node:** Separates an image into its RGB/A channels with an option for grayscale outputs.
    - **Blur Node:** Applies Gaussian blur with configurable radius (1–20px) and supports different blur types.
    - **Threshold Node:** Converts images to binary form using various thresholding methods (binary, adaptive, Otsu) and shows a histogram.
    - **Blend Node:** Combines two images using multiple blend modes (e.g., normal, multiply, screen, overlay, difference) with an opacity slider.
    - **Noise Generation Node:** Generates procedural noise (Perlin, Simplex, Worley) with adjustable parameters.
    - **Edge Detection Node:** Implements both Sobel and Canny edge detection with parameter configuration.
    - **Convolution Filter Node:** Allows custom 3x3 or 5x5 convolution kernels, with presets like sharpen, emboss, and edge enhance.
    - **Output Node:** Displays the final processed image and handles saving it to disk.

### Node Workflow
- **Node Selection and Configuration:**  
  Users select a node from the Node Selection window. The node’s parameters are then adjustable through the Properties window, with changes updating the Preview window in real time.
  
- **Processing Pipeline:**  
  The application processes nodes in a sequential manner starting from the image input and flowing through various processing nodes to produce the final output displayed in the Preview window.

- **Performance Considerations:**  
  The pipeline includes caching mechanisms to avoid redundant processing, ensuring that only nodes marked as “dirty” are re-processed.

## Build Instructions

### Prerequisites
- **Compiler:** A C++ compiler supporting C++11 or higher.
- **Libraries:**  
  - [OpenCV](https://opencv.org/) for image processing.
  - [GLFW](https://www.glfw.org/) for window creation and OpenGL context management.
  - [ImGui](https://github.com/ocornut/imgui) for the GUI.
  - [tinyfiledialogs](http://tinyfiledialogs.sourceforge.net/) for file dialog operations.
- **MSYS2 (64-bit):**  
  This project was developed and tested with MSYS2 64-bit.

### Setup and Build Steps

1. **Install MSYS2 (64-bit):**  
   Download and install MSYS2 from the [official website](https://www.msys2.org/).

2. **Install Dependencies:**  
   Use the MSYS2 package manager or build dependencies from source. For example, to install some of the required libraries:
   ```bash
   pacman -S mingw-w64-x86_64-opencv mingw-w64-x86_64-glfw mingw-w64-x86_64-glew
   ```
   Download and integrate ImGui and tinyfiledialogs into your project’s directory structure.

3. **Configure the Project:**  
   - Place all source and header files in a dedicated project directory.
   - Ensure your build system (Makefile, CMake, etc.) is set up to include the necessary include directories and library paths for OpenCV, GLFW, ImGui, and tinyfiledialogs.

4. **Compile the Project:**  
   Here is an example using `g++`:
   ```bash
   g++ -std=c++11 -Ipath/to/opencv/include -Ipath/to/imgui -Ipath/to/tinyfiledialogs -Ipath/to/glfw/include \
       main.cpp -Lpath/to/opencv/lib -Lpath/to/glfw/lib \
       -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lglfw -lGL -o NodeBasedImageProcessing
   ```
   Adjust the command as necessary for your local environment and installation paths.

5. **Run the Application:**  
   After a successful build, run the application in MSYS 64 BIT Terminal:
   ```bash
   ./NodeImageProcesser.exe
   ```
   This will launch the application displaying the four main windows (File Operations, Node Selection, Properties, and Preview).

## Usage

1. **Opening an Image:**  
   In the **File Operations** window, click on **Open Image** to load an image from the file system.

2. **Selecting and Configuring Nodes:**  
   - Go to the **Node Selection** window and click on the node you wish to modify.
   - Adjust its parameters in the **Properties** window. The Preview window updates in real time to show processing results.

3. **Saving the Processed Image:**  
   After making the desired adjustments, use the **Save Image** button in the File Operations window to save the final output.

## Additional Information

- **Error Handling:**  
  The project features basic error handling for issues like invalid file operations or connections between nodes.

- **Extensibility:**  
  The application is designed with modularity in mind. New nodes and processing capabilities can be added easily by extending the node framework and updating the processing logic.

- **Documentation:**  
  The source code is well-documented with inline comments to explain the functionality and design of each node and component.

---
