#pragma once
#include "NodeBase.h"
#include <opencv2/opencv.hpp>
#include <vector>

enum class ThresholdMethod {
    Binary,
    Otsu,
    Adaptive
};

class ThresholdNode : public NodeBase {
    public:
        ThresholdNode();
    
        void setInputImage(const cv::Mat& image) ;
        const cv::Mat& getOutputImage() const ;
        void process() override;
        void drawUI() override;
    
    private:
        bool useThreshold;
        int thresholdValue;
        ThresholdMethod method;
    
        // Adaptive threshold parameters
        int adaptiveBlockSize;
        int adaptiveC;
    
        // Otsu threshold result
        double computedOtsuThresh;
    
        cv::Mat colorInput, grayInput, outputImage;
        std::vector<float> histogramData; // Histogram data for UI display
};