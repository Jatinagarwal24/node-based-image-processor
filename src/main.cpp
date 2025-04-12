#include "ImageInputNode.h"
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
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // File Operations Window
    ImGui::Begin("File Operations");
    if (ImGui::Button("Open Image")) {
        const char* filters[] = { "*.jpg", "*.png", "*.bmp" };
        const char* filePath = tinyfd_openFileDialog(
            "Select an Image", "", 3, filters, "Image Files (*.jpg, *.png, *.bmp)", 0
        );

        if (filePath) {
            imageInputNode.loadImage(filePath);
            selectedNode = &imageInputNode; // Automatically select the input node
        }
    }
    if (ImGui::Button("Save Image")) {
        const char* savePath = tinyfd_saveFileDialog(
            "Save Image", "output.jpg", 0, nullptr, "Image Files (*.jpg, *.png, *.bmp)"
        );

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
    ImGui::End();

    // Node Canvas Window
    ImGui::Begin("Node Canvas");
    ImGui::Text("Pipeline:");
    ImGui::BulletText("Image Input Node");
    ImGui::End();

    // Properties Window
    ImGui::Begin("Properties");
    if (selectedNode) {
        selectedNode->drawUI(); // Display the selected node's properties
    } else {
        ImGui::Text("No node selected.");
    }
    ImGui::End();

    // Render ImGui
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}