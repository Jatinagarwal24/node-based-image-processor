#pragma once
#include "NodeBase.h"
#include <opencv2/opencv.hpp>
#include <vector>

enum class EdgeMethod { Sobel, Canny };

class EdgeDetectionNode : public NodeBase {
public:
    EdgeDetectionNode();

    void setInputImage(const cv::Mat& image) ;
    const cv::Mat& getOutputImage() const;
    void process() override;
    void drawUI() override;

private:
    cv::Mat inputImage;
    cv::Mat grayImage;
    cv::Mat edgeImage;
    bool overlayEdges;

    // Parameters
    EdgeMethod method;
    int sobelKernelSize;
    int cannyThreshold1;
    int cannyThreshold2;

    // Output
    cv::Mat outputImage;
};
