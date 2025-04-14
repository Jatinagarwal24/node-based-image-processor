#pragma once

#include "NodeBase.h"
#include <opencv2/core.hpp>

class BlurNode : public NodeBase {
public:
    BlurNode();

    void setInputImage(const cv::Mat& image);
    const cv::Mat& getOutputImage() const ;
    void process() override;
    void drawUI() override;
    void reset() override {
        NodeBase::reset(); // Call base class reset
        blurRadius = 5; // Default radius
        uniformBlur = true; // Default to uniform blur
        directionHorizontal = true; // Default to horizontal direction
        useBlurNode = false; // Default to disabled
    }
private:
    int blurRadius;
    bool uniformBlur;
    bool directionHorizontal;
    bool useBlurNode;
    bool processed;

    cv::Mat kernelPreview;  // For displaying the kernel
};
