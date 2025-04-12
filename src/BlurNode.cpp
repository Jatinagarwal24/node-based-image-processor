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
    // Optionally, you can check if the input image has changed as in BrightnessContrastNode
    inputImage = image;
    dirty = true;
    // std::cout << "BlurNode::setInputImage(): " << image.cols << "x" << image.rows << std::endl;
}

const cv::Mat& BlurNode::getOutputImage() const {
    return outputImage;
}

void BlurNode::process() {
    if (inputImage.empty()) {
        // std::cout << "BlurNode::process(): input image is empty" << std::endl;
        return;
    }
    
    // Calculate the kernel size from blurRadius (kernelSize = 2*blurRadius+1)
    int kernelSize = blurRadius * 2 + 1;
    
    if (uniformBlur) {
        // Apply a 2D Gaussian blur (uniform in both dimensions)
        cv::GaussianBlur(inputImage, outputImage, cv::Size(kernelSize, kernelSize), 0);
    } else {
        // Apply directional blur
        if (directionHorizontal) {
            // Horizontal blur only
            cv::GaussianBlur(inputImage, outputImage, cv::Size(kernelSize, 1), 0);
        } else {
            // Vertical blur only
            cv::GaussianBlur(inputImage, outputImage, cv::Size(1, kernelSize), 0);
        }
    }
    
    // For educational preview, generate the 1D Gaussian kernel (if horizontal, display in one row)
    cv::Mat kernel = cv::getGaussianKernel(kernelSize, -1, CV_32F);
    if (directionHorizontal || uniformBlur) {
        kernelPreview = kernel.t(); // Display as a single row
    } else {
        kernelPreview = kernel.clone(); // Use as-is for vertical (single column)
    }
    
    processed = true;
    dirty = false;
    
    // Print some debug info
    double minVal, maxVal;
    cv::minMaxLoc(outputImage, &minVal, &maxVal);
    // std::cout << "BlurNode::process()\n"
    //           << "  Output image size: " << outputImage.cols << " x " << outputImage.rows << "\n"
    //           << "  Pixel range: " << minVal << " to " << maxVal << std::endl;
}

void BlurNode::drawUI() {
    ImGui::Text("Blur Node");
    bool changed = false;
    
    // Slider to adjust blur radius (affecting kernel size)
    changed |= ImGui::SliderInt("Blur Radius", &blurRadius, 1, 20);
    
    // Checkbox to choose between a full 2D (uniform) Gaussian blur versus a directional blur.
    changed |= ImGui::Checkbox("Uniform Blur (2D)", &uniformBlur);
    
    // If not using uniform blur, allow selection of horizontal vs vertical
    if (!uniformBlur) {
        changed |= ImGui::Checkbox("Horizontal Blur", &directionHorizontal);
    }
    
    if (changed) {
        dirty = true;
    }
    
    if (ImGui::Button("Apply Blur")) {
        process();
        dirty = false;
    }
    
    // Display a preview of the Gaussian kernel values for educational purposes
    if (!kernelPreview.empty()) {
        ImGui::Text("Kernel Preview:");
        // If the kernel preview is a row, display the values.
        for (int i = 0; i < kernelPreview.cols; ++i) {
            float val = kernelPreview.at<float>(0, i);
            ImGui::SameLine();
            ImGui::Text("%.3f", val);
        }
    }
}