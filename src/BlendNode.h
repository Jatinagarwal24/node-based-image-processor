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


private:
    cv::Mat inputImage;    // Primary image
    cv::Mat blendImage;    // Image to blend with
    cv::Mat outputImage;   // Result image 
    BlendMode blendMode;
    float opacity;        // Blend strength [0.0 - 1.0]
    bool processed;     // Flag to indicate if the image has been processed
    bool useBlend;    // Flag to indicate if blending is enabled
};
