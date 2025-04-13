#pragma once
#include "NodeBase.h"
#include "BrightnessContrastNode.h"
#include <opencv2/opencv.hpp>

class ColorChannelSplitterNode : public NodeBase {
public:
    ColorChannelSplitterNode() : NodeBase("ColorChannelSplitter") {}

    // Constructor to accept BrightnessContrastNode
    ColorChannelSplitterNode(BrightnessContrastNode& bcNode)
        : NodeBase("ColorChannelSplitter"), brightnessContrastNode(&bcNode) {}

    void process();
    void drawUI();

    void setInputImage(const cv::Mat& image) {
        inputImage = image;
        dirty = true; // Mark the node as needing processing
    }

    const cv::Mat& getOutputImage() const {
        return outputImage;
    }

    cv::Mat inputImage;
    cv::Mat outputImage;
    cv::Mat redChannel;
    cv::Mat greenChannel;
    cv::Mat blueChannel;
    bool showRed = true;  // Default to true
    bool showGreen = true; // Default to true
    bool showBlue = true;  // Default to true
    bool dirty = false;
    void reset() override {
        NodeBase::reset(); // Call base class reset
        showRed = true;
        showGreen = true;
        showBlue = true;
        redChannel.release();
        greenChannel.release();
        blueChannel.release();
    }


    BrightnessContrastNode* brightnessContrastNode = nullptr; // Pointer to the connected node
};