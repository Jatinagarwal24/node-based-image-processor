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
        void reset() override {
            NodeBase::reset(); // Call base class reset
            useThreshold = false;
            thresholdValue = 128;
            adaptiveBlockSize = 11;
            adaptiveC = 2;
            computedOtsuThresh = 0.0;
            dirty= true; // Mark as dirty
        }
    
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