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

private:
    int blurRadius;
    bool uniformBlur;
    bool directionHorizontal;
    bool useBlurNode;
    bool processed;

    cv::Mat kernelPreview;  // For displaying the kernel
};
