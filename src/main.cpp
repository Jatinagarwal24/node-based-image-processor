#include "ImageInputNode.h"
#include "BrightnessContrastNode.h"
#include "OutputNode.h"
#include "ColorChannelSplitterNode.h"
#include "BlurNode.h"
#include "BlendNode.h"
#include "ThresholdNode.h"
#include "EdgeDetectionNode.h"
#include "NoiseGenerationNode.h"
#include "ConvolutionFilterNode.h"
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
static ThresholdNode thresholdNode;
static EdgeDetectionNode edgeDetectionNode;
static BlendNode blendNode;
static OutputNode outputNode;
static NoiseGenerationNode noiseGenerationNode;
static ConvolutionFilterNode convolutionFilterNode;

GLFWwindow* window = nullptr;
NodeBase* selectedNode = nullptr;

float fontScale = 1.5f;

int main() {
    initGLFW();
    initImGui();

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
        std::cerr << "GLFW init failed!\n";
        exit(1);
    }
    window = glfwCreateWindow(1280, 720, "Node-Based Image Processing", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        std::cerr << "Window creation failed!\n";
        exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

void initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = fontScale;
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
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(display_w, 50), ImGuiCond_Always);
    ImGui::Begin("Title", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Node Based Manipulation Interface").x) * 0.5f);
    ImGui::Text("Node Based Manipulation Interface");
    ImGui::End();

    // File Operations Window
    ImGui::SetNextWindowPos(ImVec2(0, 50), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(display_w * 0.25f, display_h - 50), ImGuiCond_Always);
    ImGui::Begin("File Operations", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    if (ImGui::Button("Open Image")) {
        const char* filters[] = { "*.jpg", "*.png", "*.bmp" };
        const char* filePath = tinyfd_openFileDialog("Select an Image", "", 3, filters, "Image Files", 0);
        if (filePath) {
            imageInputNode.loadImage(filePath);
            selectedNode = &imageInputNode;
        }
    }
    if (ImGui::Button("Save Image")) {
        const char* savePath = tinyfd_saveFileDialog("Save Image", "output.jpg", 0, nullptr, "Image Files");
        if (savePath) {
            outputNode.saveImage(savePath);
        }
    }
    ImGui::End();

    // Node Selection Window
    ImGui::SetNextWindowPos(ImVec2(display_w * 0.25f, 50), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(display_w * 0.25f, display_h - 50), ImGuiCond_Always);
    ImGui::Begin("Node Selection", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    if (ImGui::Button("Image Input Node")) selectedNode = &imageInputNode;
    if (ImGui::Button("Brightness/Contrast Node")) selectedNode = &brightnessContrastNode;
    if (ImGui::Button("Color Channel Splitter Node")) selectedNode = &colorChannelSplitterNode;
    if (ImGui::Button("Blur Node")) selectedNode = &blurNode;
    if (ImGui::Button("Blend Node")) selectedNode = &blendNode;
    if (ImGui::Button("Threshold Node")) selectedNode = &thresholdNode;
    if (ImGui::Button("Noise Generation Node")) selectedNode = &noiseGenerationNode;
    if (ImGui::Button("Edge Detection Node")) selectedNode = &edgeDetectionNode;
    if (ImGui::Button("Convolution Filter Node")) selectedNode = &convolutionFilterNode;
    if (ImGui::Button("Output Node")) selectedNode = &outputNode;
    ImGui::End();

    // Properties Window
    ImGui::SetNextWindowPos(ImVec2(display_w * 0.5f, 50), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(display_w * 0.25f, display_h - 50), ImGuiCond_Always);
    ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    if (selectedNode) {
        selectedNode->drawUI();
    } else {
        ImGui::Text("No node selected.");
    }
    ImGui::End();

    // Preview Window
    ImGui::SetNextWindowPos(ImVec2(display_w * 0.75f, 50), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(display_w * 0.25f, display_h - 50), ImGuiCond_Always);
    ImGui::Begin("Preview", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    const cv::Mat& finalImage = outputNode.getOutputImage();
    if (!finalImage.empty()) {
        cv::Mat resizedPreview;
        float previewWidth = display_w * 0.25f - 20.0f;
        float scale = previewWidth / finalImage.cols;
        if (scale > 1.0f) scale = 1.0f;  // Prevent upscaling
        int previewHeight = static_cast<int>(finalImage.rows * scale);

        try {
            cv::resize(finalImage, resizedPreview, cv::Size(static_cast<int>(previewWidth), previewHeight));

            if (resizedPreview.channels() == 1) {
                cv::cvtColor(resizedPreview, resizedPreview, cv::COLOR_GRAY2RGBA);
            } else if (resizedPreview.channels() == 3) {
                cv::cvtColor(resizedPreview, resizedPreview, cv::COLOR_BGR2RGBA);
            }

            GLuint textureId = 0;
            textureId = OpenGLHelper::cvMatToTexture(resizedPreview, textureId);
            ImGui::Image((ImTextureID)(uintptr_t)textureId, ImVec2(resizedPreview.cols, resizedPreview.rows));
        } catch (const cv::Exception& e) {
            ImGui::Text("Failed to generate preview: %s", e.what());
        }
    } else {
        ImGui::Text("No output image available.");
    }
    ImGui::End();

    // Image pipeline execution
    if (!imageInputNode.getOutputImage().empty()) {
        brightnessContrastNode.setInputImage(imageInputNode.getOutputImage());
        if (brightnessContrastNode.dirty) brightnessContrastNode.process();

        if (!brightnessContrastNode.getOutputImage().empty()) {
            colorChannelSplitterNode.setInputImage(brightnessContrastNode.getOutputImage());
            if (colorChannelSplitterNode.dirty) colorChannelSplitterNode.process();

            if (!colorChannelSplitterNode.getOutputImage().empty()) {
                blurNode.setInputImage(colorChannelSplitterNode.getOutputImage());
                if (blurNode.dirty) blurNode.process();

                if (!blurNode.getOutputImage().empty()) {
                    blendNode.setBlendImage(blurNode.getOutputImage(), imageInputNode.getOutputImage());
                    if (blendNode.dirty) blendNode.process();

                    if (!blendNode.getOutputImage().empty()) {
                        thresholdNode.setInputImage(blendNode.getOutputImage());
                        if (thresholdNode.dirty) thresholdNode.process();

                        if (!thresholdNode.getOutputImage().empty()) {
                            noiseGenerationNode.setInputImage(thresholdNode.getOutputImage());
                            if (noiseGenerationNode.dirty) noiseGenerationNode.process();

                            if (!noiseGenerationNode.getOutputImage().empty()) {
                                edgeDetectionNode.setInputImage(noiseGenerationNode.getOutputImage());
                                if (edgeDetectionNode.dirty) edgeDetectionNode.process();

                                if (!edgeDetectionNode.getOutputImage().empty()) {
                                    convolutionFilterNode.setInputImage(edgeDetectionNode.getOutputImage());
                                    if (convolutionFilterNode.dirty) convolutionFilterNode.process();

                                    if (!convolutionFilterNode.getOutputImage().empty()) {
                                        outputNode.setInputImage(convolutionFilterNode.getOutputImage());
                                        if (outputNode.dirty) outputNode.process();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    ImGui::Render();
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}
