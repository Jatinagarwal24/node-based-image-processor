#include "OutputNode.h"
#include <imgui.h>
#include <opencv2/opencv.hpp>
#include <iostream>

OutputNode::OutputNode()
    : NodeBase("OutputNode")
{
}

void OutputNode::process() {
    // For an output node, we simply pass the input image to the output.
    if (!inputImage.empty()) {
        outputImage = inputImage.clone();
    }
}

void OutputNode::drawUI() {
    ImGui::Text("Output Node");

    if (ImGui::Button("Save Output")) {
        saveImage("C:\\Users\\jatin\\OneDrive\\Pictures\\output.png"); // or let user choose path later
    }

    if (!outputImage.empty()) {
        ImGui::Text("Output image available.");
        // Optional preview
        // GLuint textureID = cvMatToTexture(outputImage, existingTextureID);
        // ImGui::Image((ImTextureID)(uintptr_t)textureID, ImVec2(...));
    } else {
        ImGui::Text("No output image available.");
    }
}

void OutputNode::setInputImage(const cv::Mat& image) {
    // Set the inherited inputImage.
    inputImage = image;
    // Optionally update outputImage immediately.
    outputImage = image.clone();
}

const cv::Mat& OutputNode::getOutputImage() const {
    return outputImage;
}

void OutputNode::saveImage(const std::string& filePath) {
    outputImage = inputImage.clone(); // Ensure outputImage is set before saving
    if (outputImage.empty()) {
        std::cerr << "No output image to save." << std::endl;
        return;
    }
    if (!cv::imwrite(filePath, outputImage)) {
        std::cerr << "Failed to save image to " << filePath << std::endl;
    } else {
        std::cout << "Image saved to " << filePath << std::endl;
    }
}
