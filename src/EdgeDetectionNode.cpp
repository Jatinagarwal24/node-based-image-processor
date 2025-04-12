#include "EdgeDetectionNode.h"
#include <imgui.h>
#include <opencv2/imgproc.hpp>
#include <iostream>

EdgeDetectionNode::EdgeDetectionNode()
    : NodeBase("Edge Detection Node"),
      overlayEdges(true),
      method(EdgeMethod::Sobel),
      sobelKernelSize(1),
      cannyThreshold1(100),
      cannyThreshold2(200)
{
}

void EdgeDetectionNode::setInputImage(const cv::Mat& image) {
    inputImage = image.clone();
    if (!inputImage.empty() && inputImage.channels() == 3) {
        cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
    } else {
        grayImage = inputImage.clone();
    }
    dirty = true;
}

const cv::Mat& EdgeDetectionNode::getOutputImage() const {
    return outputImage;
}

void EdgeDetectionNode::process() {
    if (!dirty) return;

    if (grayImage.empty()) {
        std::cerr << "EdgeDetectionNode::process() - input is empty\n";
        outputImage = cv::Mat();
        dirty = false;
        return;
    }

    if (method == EdgeMethod::Sobel) {
        cv::Mat gradX, gradY;
        cv::Sobel(grayImage, gradX, CV_16S, 1, 0, sobelKernelSize);
        cv::Sobel(grayImage, gradY, CV_16S, 0, 1, sobelKernelSize);

        cv::Mat absX, absY;
        cv::convertScaleAbs(gradX, absX);
        cv::convertScaleAbs(gradY, absY);

        cv::addWeighted(absX, 0.5, absY, 0.5, 0, edgeImage);
    } else if (method == EdgeMethod::Canny) {
        cv::Canny(grayImage, edgeImage, cannyThreshold1, cannyThreshold2);
    }

    if (overlayEdges && inputImage.channels() == 3) {
        cv::Mat colorEdges;
        cv::cvtColor(edgeImage, colorEdges, cv::COLOR_GRAY2BGR);
        outputImage = inputImage.clone();
        cv::addWeighted(colorEdges, 1.0, outputImage, 1.0, 0.0, outputImage);
    } else {
        cv::cvtColor(edgeImage, outputImage, cv::COLOR_GRAY2BGR);
    }

    dirty = false;
}

void EdgeDetectionNode::drawUI() {
    ImGui::Text("Edge Detection Node");
    bool changed = false;

    const char* methods[] = { "Sobel", "Canny" };
    int methodIdx = static_cast<int>(method);
    if (ImGui::Combo("Method", &methodIdx, methods, IM_ARRAYSIZE(methods))) {
        method = static_cast<EdgeMethod>(methodIdx);
        changed = true;
    }

    if (method == EdgeMethod::Sobel) {
        changed |= ImGui::SliderInt("Sobel Kernel Size", &sobelKernelSize, 1, 7);
        if (sobelKernelSize % 2 == 0) sobelKernelSize += 1; // Ensure it's odd
    } else if (method == EdgeMethod::Canny) {
        changed |= ImGui::SliderInt("Canny Threshold 1", &cannyThreshold1, 0, 255);
        changed |= ImGui::SliderInt("Canny Threshold 2", &cannyThreshold2, 0, 255);
    }

    changed |= ImGui::Checkbox("Overlay on Original", &overlayEdges);

    if (changed) {
        dirty = true;
    }

    if (dirty) {
        process();
    }
}
