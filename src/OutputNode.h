#pragma once
#include "NodeBase.h"
#include <imgui.h>
#include <opencv2/opencv.hpp>
#include <string>

class OutputNode : public NodeBase {
public:
    OutputNode();
    void process() override;
    void drawUI() override;
    
    // Use the inherited inputImage/outputImage from NodeBase.
    void setInputImage(const cv::Mat& image);
    const cv::Mat& getOutputImage() const;

    void saveImage(const std::string& filePath);
};

