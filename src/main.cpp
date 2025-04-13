#include "ImageInputNode.h"
#include "BrightnessContrastNode.h"
#include "OutputNode.h"
#include "ColorChannelSplitterNode.h"
#include "BlurNode.h"
#include "BlendNode.h"
#include "ThresholdNode.h"
#include "EdgeDetectionNode.h"
#include "NoiseGenerationNode.h"
#include <opencv2/opencv.hpp>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "OpenGLHelper.h"
#include "NodeBase.h"
#include <string>
#include "tinyfiledialogs.h"

// Function declarations
void initGLFW();
void initImGui();
void shutdownImGui();
void renderUI();

// Global node declarations
static ImageInputNode imageInputNode;
static BrightnessContrastNode brightnessContrastNode;
static ColorChannelSplitterNode colorChannelSplitterNode(brightnessContrastNode);
static BlurNode blurNode;
static ThresholdNode thresholdNode; //
static EdgeDetectionNode edgeDetectionNode;
static BlendNode blendNode;
static OutputNode outputNode;
static NoiseGenerationNode noiseGenerationNode;

// Global variables
GLFWwindow* window = nullptr;
cv::Mat image;
NodeBase* selectedNode = nullptr; // Track the currently selected node

int main() {
    // Initialize GLFW and ImGui/OpenGL
    initGLFW();
    initImGui();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        renderUI();
    }

    shutdownImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void initGLFW() {
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed!" << std::endl;
        exit(1);
    }
    window = glfwCreateWindow(800, 600, "Node-Based Image Processing", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        std::cerr << "Window creation failed!" << std::endl;
        exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
}

void initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void shutdownImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void renderUI() {
    // Start the frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // File Operations Window
    ImGui::Begin("File Operations");
    if (ImGui::Button("Open Image")) {
        const char* filters[] = { "*.jpg", "*.png", "*.bmp" };
        const char* filePath = tinyfd_openFileDialog("Select an Image", "", 3, filters, "Image Files (*.jpg, *.png, *.bmp)", 0);
        if (filePath) {
            imageInputNode.loadImage(filePath);
            selectedNode = &imageInputNode; // Automatically select the input node
        }
    }
    if (ImGui::Button("Save Image")) {
        const char* savePath = tinyfd_saveFileDialog("Save Image", "output.jpg", 0, nullptr, "Image Files (*.jpg, *.png, *.bmp)");
        if (savePath) {
            outputNode.saveImage(savePath);
        }
    }
    ImGui::End();

    // Node Selection Window
    ImGui::Begin("Node Selection");
    if (ImGui::Button("Image Input Node")) {
        selectedNode = &imageInputNode;
    }
    if (ImGui::Button("Brightness/Contrast Node")) {
        selectedNode = &brightnessContrastNode;
    }
    if (ImGui::Button("Color Channel Splitter Node")) {
        selectedNode = &colorChannelSplitterNode;
    }
    if (ImGui::Button("Blur Node")) {
        selectedNode = &blurNode;
    }

    if (ImGui::Button("Blend Node")) {
        selectedNode = &blendNode;
    }
    if (ImGui::Button("Output Node")) {
        selectedNode = &outputNode;
    }
    if (ImGui::Button("Threshold Node")) {
        selectedNode = &thresholdNode;
    }
    if (ImGui::Button("Edge Detection Node")) {
        selectedNode = &edgeDetectionNode;
    }
    if (ImGui::Button("Noise Generation Node")) {
        selectedNode = &noiseGenerationNode;
    }
    ImGui::End();

    // Connect the pipeline
    if (!imageInputNode.getOutputImage().empty()) {
        brightnessContrastNode.setInputImage(imageInputNode.getOutputImage());
        if (brightnessContrastNode.dirty) {
            brightnessContrastNode.process();
        }
        
        if (!brightnessContrastNode.getOutputImage().empty()) {
            colorChannelSplitterNode.setInputImage(brightnessContrastNode.getOutputImage());
            if (colorChannelSplitterNode.dirty) {
                colorChannelSplitterNode.process();
            }
            
            if (!colorChannelSplitterNode.getOutputImage().empty()) {
                blurNode.setInputImage(colorChannelSplitterNode.getOutputImage());
                if (blurNode.dirty) {
                    blurNode.process();
                }
                
                if (!blurNode.getOutputImage().empty()) {
                    // BlendNode: blend blurred output with original input image
                    blendNode.setBlendImage(blurNode.getOutputImage(), imageInputNode.getOutputImage());
                    if (blendNode.dirty) {
                        blendNode.process();
                    }
                    
                    if (!blendNode.getOutputImage().empty()) {
                        // Threshold Node: process the blended output
                        thresholdNode.setInputImage(blendNode.getOutputImage());
                        if (thresholdNode.dirty) {
                            thresholdNode.process();
                        }
                        
                        if (!thresholdNode.getOutputImage().empty()) {
                            // Noise Generation Node: add noise to the thresholded output
                            noiseGenerationNode.setInputImage(thresholdNode.getOutputImage());
                            if (noiseGenerationNode.dirty) {
                                noiseGenerationNode.process();
                            }
                            
                            if (!noiseGenerationNode.getOutputImage().empty()) {
                                // Edge Detection Node: process the noisy output
                                edgeDetectionNode.setInputImage(noiseGenerationNode.getOutputImage());
                                if (edgeDetectionNode.dirty) {
                                    edgeDetectionNode.process();
                                }
                                
                                if (!edgeDetectionNode.getOutputImage().empty()) {
                                    // Output Node: display the final edge detection result
                                    outputNode.setInputImage(edgeDetectionNode.getOutputImage());
                                    if (outputNode.dirty) {
                                        outputNode.process();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // Properties Window
    ImGui::Begin("Properties");
    if (selectedNode) {
        selectedNode->drawUI(); // Display the selected node's properties
    } else {
        ImGui::Text("No node selected.");
    }
    ImGui::End();

    // Preview Window
    ImGui::Begin("Preview");
    if (!outputNode.getOutputImage().empty()) {
        cv::Mat resizedPreview;
        float previewWidth = 300.0f;
        float scale = previewWidth / outputNode.getOutputImage().cols;
        cv::resize(outputNode.getOutputImage(), resizedPreview, cv::Size(), scale, scale);
        if (resizedPreview.channels() == 1) {
            cv::cvtColor(resizedPreview, resizedPreview, cv::COLOR_GRAY2RGBA);
        } else if (resizedPreview.channels() == 3) {
            cv::cvtColor(resizedPreview, resizedPreview, cv::COLOR_BGR2RGBA);
        }
        GLuint textureId = 0;
        textureId = OpenGLHelper::cvMatToTexture(resizedPreview, textureId);
        ImGui::Image((ImTextureID)(uintptr_t)textureId, ImVec2(resizedPreview.cols, resizedPreview.rows));
    } else {
        ImGui::Text("No output image available.");
    }
    ImGui::End();

    // End the frame and render
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}