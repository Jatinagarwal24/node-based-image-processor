#include "ColorChannelSplitterNode.h"
#include <imgui.h>
#include <opencv2/imgproc.hpp>

void ColorChannelSplitterNode::process() {
    if (inputImage.empty()) {
        return; // No input image to process
    }
    if (dirty) {
        std::vector<cv::Mat> channels;
        cv::split(inputImage, channels);
    
        if (channels.size() >= 3) {
            // std::cout << "ColorChannelSplitterNode::process(): Splitting channels" << std::endl;
            redChannel = channels[2];
            greenChannel = channels[1];
            blueChannel = channels[0];
        }
    
        // Create a blank image for merging
        cv::Mat blank = cv::Mat::zeros(redChannel.size(), redChannel.type());
    
        // Merge selected channels into a 3-channel BGR image
        std::vector<cv::Mat> mergedChannels = {
            showBlue ? blueChannel : blank,
            showGreen ? greenChannel : blank,
            showRed ? redChannel : blank
        };
    
        cv::merge(mergedChannels, outputImage);
    
        dirty = false;
    }
    
}

void ColorChannelSplitterNode::drawUI() {
    ImGui::Text("Color Channel Splitter Node");
    bool updated = false;
    updated |= ImGui::Checkbox("Show Red Channel", &showRed);
    updated |= ImGui::Checkbox("Show Green Channel", &showGreen);
    updated |= ImGui::Checkbox("Show Blue Channel", &showBlue);
    
    if (updated) {
        dirty = true;
    }

    if (outputImage.empty()) {
        ImGui::Text("No output image available.");
    } else {
        ImGui::Text("Output image is ready.");
    }
    if (ImGui::Button("Reset")) {
        reset(); // Call the reset method
    }

}