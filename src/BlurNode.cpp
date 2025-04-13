#include "BlurNode.h"
#include <imgui.h>
#include <opencv2/imgproc.hpp>
#include <iostream>

BlurNode::BlurNode()
    : NodeBase("Blur Node"),
      blurRadius(5),
      uniformBlur(true),
      directionHorizontal(true),
      useBlurNode(false),
      processed(false)
{
}

void BlurNode::setInputImage(const cv::Mat& image) {
    if (image.empty())
        return;
    inputImage = image;
    dirty = true;
}

const cv::Mat& BlurNode::getOutputImage() const {
    if (!useBlurNode || outputImage.empty())
        return inputImage;
    return outputImage;
}

void BlurNode::process() {
    if (inputImage.empty()) return;

    if (!useBlurNode) {
        outputImage = inputImage.clone();  // Just clone original to maintain consistency
        processed = true;
        dirty = false;
        return;
    }

    int kernelSize = blurRadius * 2 + 1;

    if (uniformBlur) {
        cv::GaussianBlur(inputImage, outputImage, cv::Size(kernelSize, kernelSize), 0);
    } else {
        if (directionHorizontal)
            cv::GaussianBlur(inputImage, outputImage, cv::Size(kernelSize, 1), 0);
        else
            cv::GaussianBlur(inputImage, outputImage, cv::Size(1, kernelSize), 0);
    }

    // Generate 1D kernel preview
    cv::Mat kernel = cv::getGaussianKernel(kernelSize, -1, CV_32F);
    kernelPreview = (uniformBlur || directionHorizontal) ? kernel.t() : kernel;

    processed = true;
    dirty = false;
}

void BlurNode::drawUI() {
    ImGui::Text("Blur Node");

    bool changed = false;

    changed |= ImGui::Checkbox("Use Blur Node", &useBlurNode);
    changed |= ImGui::SliderInt("Blur Radius", &blurRadius, 1, 20);
    changed |= ImGui::Checkbox("Uniform Blur (2D)", &uniformBlur);

    if (!uniformBlur)
        changed |= ImGui::Checkbox("Horizontal Blur", &directionHorizontal);

    if (changed) dirty = true;

    if (dirty) process();

    if (!kernelPreview.empty() && useBlurNode) {
        ImGui::Text("Kernel Preview:");
        for (int i = 0; i < kernelPreview.cols; ++i) {
            float val = kernelPreview.at<float>(0, i);
            ImGui::SameLine();
            ImGui::Text("%.3f", val);
        }
    }

    ImGui::Text("%s", getOutputImage().empty() ? "No output image." : "Output image ready.");
    if (ImGui::Button("Reset")) {
        reset(); // Call the reset method
    }
}
