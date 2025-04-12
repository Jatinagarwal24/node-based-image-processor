#include "ImageInputNode.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <imgui.h>
#include "OpenGLHelper.h"  // This header defines cvMatToTexture

ImageInputNode::ImageInputNode() : NodeBase("Image Input") {}

void ImageInputNode::process() {
    // Simply pass the input image to the output
    outputImage = inputImage.clone();
}

// void ImageInputNode::drawUI() {
//     ImGui::Text("Image Input Node");
//     if (!inputImage.empty()) {
//         ImGui::Text("Image loaded successfully.");
//         GLuint textureID = 0;
//         // Convert the cv::Mat to an OpenGL texture using cvMatToTexture
//         textureID = cvMatToTexture(inputImage, textureID);
//         if (textureID != 0) {
//             // Convert textureID to ImTextureID via uintptr_t
//             ImTextureID texID = (ImTextureID)(uintptr_t)textureID;
//             ImGui::Image(texID, ImVec2(static_cast<float>(inputImage.cols), static_cast<float>(inputImage.rows)));
//             glDeleteTextures(1, &textureID);  // Clean up the texture after use
//         }
//     } else {
//         ImGui::Text("No image loaded.");
//     }
// }
void ImageInputNode::drawUI() {
    ImGui::Text("Image Input Node");

    if (!inputImage.empty()) {
        ImGui::Text("Image loaded successfully.");

        // Only create texture if not already created
        if (textureID == 0) {
            OpenGLHelper::cvMatToTexture(inputImage, textureID);
        }

        if (textureID != 0) {
            ImTextureID texID = (ImTextureID)(uintptr_t)textureID;
            ImGui::Image(texID, ImVec2(static_cast<float>(inputImage.cols), static_cast<float>(inputImage.rows)));
        }
    } else {
        ImGui::Text("No image loaded.");

        // Clean up texture if image is cleared
        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
            textureID = 0;
        }
    }
}

void ImageInputNode::setInputImage(const cv::Mat& image) {
    inputImage = image;
    outputImage = image.clone(); // Update output image immediately
}

const cv::Mat& ImageInputNode::getOutputImage() const {
    return outputImage;
}

void ImageInputNode::loadImage(const std::string& filePath) {
    inputImage = cv::imread(filePath,cv::IMREAD_COLOR);
    if (inputImage.empty()) {
        std::cerr << "Failed to load image from: " << filePath << std::endl;
    } else {
        std::cout << "Image loaded successfully from: " << filePath << std::endl;
        std::cout << "Image size: " << inputImage.cols << "x" << inputImage.rows 
                  << ", type: " << inputImage.type() << std::endl;
        outputImage = inputImage.clone();  // Immediately update outputImage
        dirty = true;
    }
}