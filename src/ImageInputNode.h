#pragma once
#include "NodeBase.h"
#include <opencv2/opencv.hpp>
#include "OpenGLHelper.h"  // make sure this path is correct relative to your file structure

class ImageInputNode : public NodeBase {
public:
    ImageInputNode();
    void process() override;
    void drawUI() override;

    // Use the inherited inputImage/outputImage from NodeBase
    void setInputImage(const cv::Mat& image);
    const cv::Mat& getOutputImage() const ;

    // Load an image from a file
    void loadImage(const std::string& filePath);
    GLuint textureID = 0; 
    bool dirty = false; // Flag to indicate if the image is dirty (needs processing)
};

