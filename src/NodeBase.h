#pragma once

#include <opencv2/core.hpp>
#include <string>

// Base class for all image processing nodes
class NodeBase {
public:
    NodeBase(const std::string& name = "UnnamedNode") : nodeName(name) {}
    virtual ~NodeBase() = default;

    // Sets the input image and marks the node as needing reprocessing
    virtual void setInputImage(const cv::Mat& image) {
        inputImage = image.clone();
        dirty = true;
    }

    // Returns the processed output image
    virtual const cv::Mat& getOutputImage() const {
        return outputImage;
    }

    // Clears the output and marks the node as dirty
    virtual void clearOutput() {
        outputImage.release();
        dirty = true;
    }

    // Getter for the dirty flag
    bool isDirty() const {
        return dirty;
    }

    // Setter for the dirty flag
    void setDirty(bool value) {
        dirty = value;
    }

    // Pure virtual methods for processing and UI rendering
    virtual void process() = 0;
    virtual void drawUI() = 0;

    // Node name (for display/debugging)
    std::string getNodeName() const {
        return nodeName;
    }


    cv::Mat inputImage;
    cv::Mat outputImage;
    virtual void reset(){
        clearOutput();
        dirty = true;
    }

    bool dirty = true; // Indicates whether the node needs reprocessing
    std::string nodeName;
};