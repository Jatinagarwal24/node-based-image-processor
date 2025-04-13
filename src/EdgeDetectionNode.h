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
    void reset() override {
        NodeBase::reset(); // Call base class reset
        overlayEdges = true;
        method = EdgeMethod::Sobel;
        sobelKernelSize = 1;
        cannyThreshold1 = 100;
        cannyThreshold2 = 200;
    }

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
