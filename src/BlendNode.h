#pragma once
#include "NodeBase.h"
#include <opencv2/opencv.hpp>

enum class BlendMode {
    Normal,
    Multiply,
    Screen,
    Overlay,
    Difference
};

class BlendNode : public NodeBase {
public:
    BlendNode();

    void process() override;
    void drawUI() override;

    // Set the two input images (A and B)
    void setBlendImage(const cv::Mat& imageA, const cv::Mat& imageB);
    const cv::Mat& getOutputImage() const;
    void reset() override {
        NodeBase::reset(); // Call base class reset
        blendMode = BlendMode::Normal; // Default blend mode
        opacity = 0.5f; // Default opacity
        useBlend = false; // Default to no blending
    }

private:
    cv::Mat inputImage;    // Primary image
    cv::Mat blendImage;    // Image to blend with
    cv::Mat outputImage;   // Result image 
    BlendMode blendMode;
    float opacity;        // Blend strength [0.0 - 1.0]
    bool processed;     // Flag to indicate if the image has been processed
    bool useBlend;    // Flag to indicate if blending is enabled
};
