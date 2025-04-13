#pragma once
#include "NodeBase.h"
#include <opencv2/core.hpp>
#include <vector>
#include <string>

enum class KernelPreset {
    Custom,
    Sharpen,
    Emboss,
    EdgeEnhance
};

class ConvolutionFilterNode : public NodeBase {
public:
    ConvolutionFilterNode();

    void setInputImage(const cv::Mat& image) ;
    const cv::Mat& getOutputImage() const;
    void process() override;
    void drawUI() override;

    // All members are public for ease of access
    cv::Mat inputImage;
    cv::Mat outputImage;

    // Toggle for enabling/disabling filter processing
    bool useFilter;

    // Kernel configuration
    int kernelSize;             // Either 3 (3x3) or 5 (5x5)
    KernelPreset kernelPreset;  // Chosen preset
    std::vector<float> customKernel; // Kernel matrix (row-major order)

    // Utility function to update customKernel based on selected preset and kernel size
    void updateKernelPreset();
};
