#ifndef BRIGHTNESS_CONTRAST_NODE_H
#define BRIGHTNESS_CONTRAST_NODE_H

#include "NodeBase.h"
#include <opencv2/opencv.hpp>
#include <imgui.h>

class BrightnessContrastNode : public NodeBase {
public:
    BrightnessContrastNode();
    
    void process() override;
    void drawUI() override;
    bool isImageProcessed() const;

    void setInputImage(const cv::Mat& image);
    const cv::Mat& getOutputImage() const;
   

private:
    float brightness;  
    float contrast;
    bool processed;
    
};

#endif // BRIGHTNESS_CONTRAST_NODE_H