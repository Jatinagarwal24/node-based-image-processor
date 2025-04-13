#include "ConvolutionFilterNode.h"
#include <imgui.h>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <algorithm>

ConvolutionFilterNode::ConvolutionFilterNode()
    : NodeBase("Convolution Filter Node"),
      useFilter(false),        // Default: filter is disabled, so original image will pass through
      kernelSize(3),
      kernelPreset(KernelPreset::Sharpen),
      customKernel(9, 0.0f)      // Initialize for a 3x3 kernel
{
    updateKernelPreset();
}

void ConvolutionFilterNode::setInputImage(const cv::Mat& image) {
    inputImage = image.clone();
    dirty = true;
}

const cv::Mat& ConvolutionFilterNode::getOutputImage() const {
    return outputImage;
}

void ConvolutionFilterNode::updateKernelPreset() {
    // Update the customKernel vector based on current preset and kernelSize
    if (kernelSize == 3) {
        customKernel.resize(9);
        switch (kernelPreset) {
            case KernelPreset::Sharpen:
                // Sharpen 3x3 kernel
                customKernel = {  0, -1,  0,
                                 -1,  5, -1,
                                  0, -1,  0 };
                break;
            case KernelPreset::Emboss:
                // Emboss 3x3 kernel
                customKernel = { -2, -1,  0,
                                 -1,  1,  1,
                                  0,  1,  2 };
                break;
            case KernelPreset::EdgeEnhance:
                // Edge enhance 3x3 kernel
                customKernel = {  1,  1,  1,
                                  1, -7,  1,
                                  1,  1,  1 };
                break;
            case KernelPreset::Custom:
            default:
                // Identity kernel for custom mode by default
                customKernel = {  0, 0, 0,
                                  0, 1, 0,
                                  0, 0, 0 };
                break;
        }
    } else if (kernelSize == 5) {
        customKernel.resize(25);
        switch (kernelPreset) {
            case KernelPreset::Sharpen:
                // Example 5x5 sharpen kernel
                customKernel = {
                    -1, -1, -1, -1, -1,
                    -1,  2,  2,  2, -1,
                    -1,  2,  8,  2, -1,
                    -1,  2,  2,  2, -1,
                    -1, -1, -1, -1, -1
                };
                break;
            case KernelPreset::Emboss:
                // Simple 5x5 emboss kernel
                customKernel = {
                    -2, -1,  0,  1,  2,
                    -2, -1,  0,  1,  2,
                    -2, -1,  0,  1,  2,
                    -2, -1,  0,  1,  2,
                    -2, -1,  0,  1,  2
                };
                break;
            case KernelPreset::EdgeEnhance:
                // Example 5x5 edge enhance kernel
                customKernel = {
                     1,  1,   1,  1,  1,
                     1,  1,   1,  1,  1,
                     1,  1, -24,  1,  1,
                     1,  1,   1,  1,  1,
                     1,  1,   1,  1,  1
                };
                break;
            case KernelPreset::Custom:
            default:
                // Identity kernel for custom 5x5
                customKernel = std::vector<float>(25, 0.0f);
                customKernel[12] = 1.0f;
                break;
        }
    }
}

void ConvolutionFilterNode::process() {
    if (!dirty) return;
    if (inputImage.empty()) {
        std::cerr << "ConvolutionFilterNode::process() - input image is empty\n";
        dirty = false;
        return;
    }
    
    // If filtering is not enabled, pass the input image through unchanged.
    if (!useFilter) {
        outputImage = inputImage.clone();
        dirty = false;
        return;
    }
    
    // Ensure the customKernel has the correct size.
    if (customKernel.size() != static_cast<size_t>(kernelSize * kernelSize)) {
        updateKernelPreset();
    }
    
    // Create an OpenCV kernel from customKernel data.
    cv::Mat kernelMat(kernelSize, kernelSize, CV_32F, customKernel.data());
    
    // Apply the convolution filter using OpenCV's filter2D.
    cv::filter2D(inputImage, outputImage, -1, kernelMat);
    dirty = false;
}

void ConvolutionFilterNode::drawUI() {
    ImGui::Text("Convolution Filter Node");
    bool changed = false;
    
    // Checkbox for enabling/disabling filter (if disabled, original image shows)
    changed |= ImGui::Checkbox("Enable Filter", &useFilter);
    
    // Kernel size selection with radio buttons (3x3 vs 5x5)
    int radioChoice = (kernelSize == 3) ? 0 : 1;
    if (ImGui::RadioButton("3x3", &radioChoice, 0)) {
        kernelSize = 3;
        changed = true;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("5x5", &radioChoice, 1)) {
        kernelSize = 5;
        changed = true;
    }
    
    // Kernel preset selection combo
    const char* presets[] = { "Custom", "Sharpen", "Emboss", "Edge Enhance" };
    int presetIdx = static_cast<int>(kernelPreset);
    if (ImGui::Combo("Preset", &presetIdx, presets, IM_ARRAYSIZE(presets))) {
        kernelPreset = static_cast<KernelPreset>(presetIdx);
        updateKernelPreset();
        changed = true;
    }
    
    // Allow manual editing only in Custom preset mode
    if (kernelPreset == KernelPreset::Custom) {
        int totalElements = kernelSize * kernelSize;
        for (int i = 0; i < totalElements; ++i) {
            std::string label = "K" + std::to_string(i);
            changed |= ImGui::InputFloat(label.c_str(), &customKernel[i], 0.1f, 1.0f, "%.2f");
            if ((i + 1) % kernelSize != 0)
                ImGui::SameLine();
        }
    } else {
        ImGui::Text("Switch to 'Custom' preset to edit kernel values manually.");
    }
    
    if (changed) {
        dirty = true;
    }
    
    if (!inputImage.empty()) {
        process();
        ImGui::Text("Kernel Effect Preview:");
        // Note: Typically, here you would convert outputImage to a texture and display it.
        // This framework-specific code is assumed to be handled elsewhere in your system.
    }
    if (ImGui::Button("Reset")) {
        reset(); // Call the reset method
    }
}
