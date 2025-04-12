#include "BlurNode.h"
#include <imgui.h>
#include <opencv2/imgproc.hpp>
#include <iostream>

BlurNode::BlurNode()
    : NodeBase("Blur Node"),
      blurRadius(5),
      uniformBlur(true),
      directionHorizontal(true), // if false, vertical blur will be used when not using uniformBlur
      processed(false)
{
}

void BlurNode::setInputImage(const cv::Mat& image) {
    if (image.empty())
        return;
    // Optionally, you can add a change check similar to BrightnessContrastNode
    inputImage = image;
    dirty = true;
}

const cv::Mat& BlurNode::getOutputImage() const {
    return outputImage;
}

void BlurNode::process() {
    if (inputImage.empty())
        return;
    
    // Calculate the kernel size from blurRadius (kernelSize = 2*blurRadius+1)
    int kernelSize = blurRadius * 2 + 1;
    
    if (uniformBlur) {
        // Apply a 2D Gaussian blur (uniform in both dimensions)
        cv::GaussianBlur(inputImage, outputImage, cv::Size(kernelSize, kernelSize), 0);
    } else {
        // Apply directional blur based on the selected axis
        if (directionHorizontal) {
            cv::GaussianBlur(inputImage, outputImage, cv::Size(kernelSize, 1), 0);
        } else {
            cv::GaussianBlur(inputImage, outputImage, cv::Size(1, kernelSize), 0);
        }
    }
    
    // For educational preview, generate the 1D Gaussian kernel
    cv::Mat kernel = cv::getGaussianKernel(kernelSize, -1, CV_32F);
    if (directionHorizontal || uniformBlur) {
        kernelPreview = kernel.t(); // Display as a single row
    } else {
        kernelPreview = kernel.clone(); // Use as-is for vertical (single column)
    }
    
    processed = true;
    dirty = false;
    
    // Optionally, print some debug info
    double minVal, maxVal;
    cv::minMaxLoc(outputImage, &minVal, &maxVal);
}

void BlurNode::drawUI() {
    ImGui::Text("Blur Node");
    bool changed = false;
    
    // Slider for blur radius
    changed |= ImGui::SliderInt("Blur Radius", &blurRadius, 1, 20);
    
    // Checkbox to select between uniform (2D) blur and directional blur
    changed |= ImGui::Checkbox("Uniform Blur (2D)", &uniformBlur);
    
    // If not using uniform blur, allow choice between horizontal and vertical
    if (!uniformBlur) {
        changed |= ImGui::Checkbox("Horizontal Blur", &directionHorizontal);
    }
    
    if (changed) {
        dirty = true;
    }
    
    // Automatically process when dirty (no need for an "Apply" button)
    if (dirty) {
        process();
    }
    
    // Display the Gaussian kernel preview for educational purposes
    if (!kernelPreview.empty()) {
        ImGui::Text("Kernel Preview:");
        for (int i = 0; i < kernelPreview.cols; ++i) {
            float val = kernelPreview.at<float>(0, i);
            ImGui::SameLine();
            ImGui::Text("%.3f", val);
        }
    }
}