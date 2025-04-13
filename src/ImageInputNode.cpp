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

//         // Only create texture if not already created
//         if (textureID == 0) {
//             OpenGLHelper::cvMatToTexture(inputImage, textureID);
//         }

//         if (textureID != 0) {
//             ImTextureID texID = (ImTextureID)(uintptr_t)textureID;
//             ImGui::Image(texID, ImVec2(static_cast<float>(inputImage.cols), static_cast<float>(inputImage.rows)));
//         }
//     } else {
//         ImGui::Text("No image loaded.");

//         // Clean up texture if image is cleared
//         if (textureID != 0) {
//             glDeleteTextures(1, &textureID);
//             textureID = 0;
//         }
//     }
// }

void ImageInputNode::drawUI() {
    ImGui::Text("Image Input Node");

    if (!inputImage.empty()) {
        ImGui::Text("Image loaded successfully.");

        // Regenerate texture if image was marked as dirty
        if (dirty) {
            if (textureID != 0) {
                glDeleteTextures(1, &textureID); // Delete old texture
                textureID = 0;
            }
            OpenGLHelper::cvMatToTexture(inputImage, textureID); // Generate new texture
            dirty = false;
        }

        if (textureID != 0) {
            ImTextureID texID = (ImTextureID)(uintptr_t)textureID;
            float maxWidth = 200.0f; // Optional: scale to fit UI
            float scale = std::min(maxWidth / inputImage.cols, 1.0f);
            ImVec2 imageSize(inputImage.cols * scale, inputImage.rows * scale);
            ImGui::Image(texID, imageSize);
        }
    } else {
        ImGui::Text("No image loaded.");

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