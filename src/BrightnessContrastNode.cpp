#include "BrightnessContrastNode.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "debugUtils.h"

BrightnessContrastNode::BrightnessContrastNode()
    : NodeBase("BrightnessContrast"), brightness(0.0f), contrast(1.0f), processed(false) {}

void BrightnessContrastNode::process() {
    if (inputImage.empty()) {
        std::cout << "BrightnessContrastNode::process(): inputImage is empty" << std::endl;
        return;
    }

    inputImage.convertTo(outputImage, -1, contrast, brightness);
    processed = true; // Mark as processed
    dirty = false;

    double minVal, maxVal;
    cv::minMaxLoc(outputImage, &minVal, &maxVal);
    std::cout << "BrightnessContrastNode::process():" << std::endl;
    std::cout << "  Output image size: " << outputImage.cols << " x " << outputImage.rows << std::endl;
    std::cout << "  Pixel value range: " << minVal << " to " << maxVal << std::endl;
    debugImage(outputImage, "BrightnessContrastNode");
}

void BrightnessContrastNode::setInputImage(const cv::Mat& image) {
    if (image.size() != inputImage.size() || 
        image.type() != inputImage.type() || 
        cv::norm(image, inputImage) > 1e-6) {  // Tolerance-based comparison
        inputImage = image;
        dirty = true; // Mark as needing processing
        std::cout << "BrightnessContrastNode::setInputImage(): " 
                  << image.cols << " x " << image.rows << std::endl;
    }
}

const cv::Mat& BrightnessContrastNode::getOutputImage() const {
    return outputImage;
}

bool BrightnessContrastNode::isImageProcessed() const {
    return !outputImage.empty();
}

void BrightnessContrastNode::drawUI() {
    ImGui::Text("Brightness/Contrast Node");
    bool changed = false;
    changed |= ImGui::SliderFloat("Brightness", &brightness, -100.0f, 100.0f);
    changed |= ImGui::SliderFloat("Contrast", &contrast, 0.0f, 3.0f);

    if (changed) {
        dirty = true;  // Mark dirty if user changes sliders
    }

}