#pragma once
#include "NodeBase.h"
#include <opencv2/opencv.hpp>
#include <imgui.h>

class BlurNode : public NodeBase {
public:
    BlurNode();
    void process() override;
    void drawUI() override;
    void setInputImage(const cv::Mat& image);
    const cv::Mat& getOutputImage() const;
    // Optionally add setters if needed.
    
    // Configurable parameters:
    int blurRadius;       // radius (1-20); kernel size = 2*blurRadius+1
    bool uniformBlur;     // if true, 2D Gaussian
    bool directionHorizontal; // used if uniformBlur is false
    
    // Preview of the Gaussian kernel, for educational display.
    cv::Mat kernelPreview;
    bool isDirectional; // if true, 1D Gaussian in horizontal direction
    bool processed;
};