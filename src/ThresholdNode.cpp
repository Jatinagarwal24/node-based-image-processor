// #include "ThresholdNode.h"
// #include <imgui.h>
// #include <opencv2/imgproc.hpp>
// #include <algorithm>
// #include <iostream>

// ThresholdNode::ThresholdNode()
//     : NodeBase("Threshold Node"),
//       useThreshold(false),     // Disabled by default
//       thresholdValue(128),
//       method(ThresholdMethod::Binary)
// {
// }

// void ThresholdNode::setInputImage(const cv::Mat& image) {
//     // Store the color input directly
//     colorInput = image.clone();

//     // If the image has 3 channels, convert to grayscale for thresholding
//     if (!image.empty() && image.channels() == 3) {
//         cv::cvtColor(image, grayInput, cv::COLOR_BGR2GRAY);
//     } else {
//         grayInput = image.clone();
//     }

//     dirty = true; // Mark as needing processing
// }

// const cv::Mat& ThresholdNode::getOutputImage() const {
//     return outputImage;
// }

// void ThresholdNode::process() {
//     if (!dirty) return;
//     if (colorInput.empty()) {
//         std::cerr << "ThresholdNode::process() - input is empty\n";
//         dirty = false;
//         return;
//     }

//     // If thresholding is disabled, pass through the original color image
//     if (!useThreshold) {
//         outputImage = colorInput.clone();
//     } else {
//         // Use the appropriate method for thresholding
//         if (method == ThresholdMethod::Binary) {
//             if (grayInput.empty()) {
//                 std::cerr << "ThresholdNode::process() - grayscale is empty, cannot threshold.\n";
//                 outputImage = colorInput.clone();
//             } else {
//                 cv::threshold(grayInput, outputImage, thresholdValue, 255, cv::THRESH_BINARY);
//             }
//         } else if (method == ThresholdMethod::Otsu) {
//             if (grayInput.empty()) {
//                 std::cerr << "ThresholdNode::process() - grayscale is empty, cannot threshold.\n";
//                 outputImage = colorInput.clone();
//             } else {
//                 cv::threshold(grayInput, outputImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
//             }
//         } else if (method == ThresholdMethod::Adaptive) {
//             if (grayInput.empty()) {
//                 std::cerr << "ThresholdNode::process() - grayscale is empty, cannot threshold.\n";
//                 outputImage = colorInput.clone();
//             } else {
//                 cv::adaptiveThreshold(grayInput, outputImage, 255,
//                                       cv::ADAPTIVE_THRESH_GAUSSIAN_C,
//                                       cv::THRESH_BINARY, 11, 2);
//             }
//         }
//     }

//     // Compute a histogram of the grayscale input for preview
//     {
//         cv::Mat histSource = grayInput.empty() ? colorInput.clone() : grayInput.clone();
//         if (!histSource.empty() && histSource.channels() == 3) {
//             cv::cvtColor(histSource, histSource, cv::COLOR_BGR2GRAY);
//         }

//         int histSize = 256;
//         float range[] = { 0, 256 };
//         const float* histRange = { range };
//         cv::Mat hist;
//         cv::calcHist(&histSource, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);

//         histogramData.resize(histSize);
//         for (int i = 0; i < histSize; i++) {
//             histogramData[i] = hist.at<float>(i);
//         }
//     }

//     dirty = false;
// }

// void ThresholdNode::drawUI() {
//     ImGui::Text("Threshold Node");
//     bool changed = false;

//     // Checkbox to enable/disable thresholding
//     changed |= ImGui::Checkbox("Enable Thresholding", &useThreshold);

//     // Show threshold slider only for Binary method
//     if (method == ThresholdMethod::Binary) {
//         changed |= ImGui::SliderInt("Threshold Value", &thresholdValue, 0, 255);
//     } else {
//         ImGui::Text("Threshold value not used for Otsu/Adaptive");
//     }

//     // Combo box for selecting the threshold method
//     const char* methods[] = { "Binary", "Otsu", "Adaptive" };
//     int methodIdx = static_cast<int>(method);
//     if (ImGui::Combo("Method", &methodIdx, methods, IM_ARRAYSIZE(methods))) {
//         method = static_cast<ThresholdMethod>(methodIdx);
//         changed = true;
//     }

//     if (changed) {
//         dirty = true;
//     }

//     // Automatically process if dirty
//     if (dirty) {
//         process();
//     }

//     // Display histogram preview if available
//     if (!histogramData.empty()) {
//         ImGui::Text("Histogram:");
//         float maxVal = *std::max_element(histogramData.begin(), histogramData.end());
//         ImGui::PlotHistogram("##histogram", histogramData.data(),
//                              static_cast<int>(histogramData.size()), 0,
//                              nullptr, 0.0f, maxVal, ImVec2(0, 80));
//     }
// }
#include "ThresholdNode.h"
#include <imgui.h>
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <iostream>

ThresholdNode::ThresholdNode()
    : NodeBase("Threshold Node"),
      useThreshold(false),
      thresholdValue(128),
      method(ThresholdMethod::Binary),
      adaptiveBlockSize(11),
      adaptiveC(2),
      computedOtsuThresh(0.0)
{
}

void ThresholdNode::setInputImage(const cv::Mat& image) {
    colorInput = image.clone();
    if (!image.empty() && image.channels() == 3) {
        cv::cvtColor(image, grayInput, cv::COLOR_BGR2GRAY);
    } else {
        grayInput = image.clone();
    }
    dirty = true;
}

const cv::Mat& ThresholdNode::getOutputImage() const {
    return outputImage;
}

void ThresholdNode::process() {
    if (!dirty) return;
    if (colorInput.empty()) {
        std::cerr << "ThresholdNode::process() - input is empty\n";
        dirty = false;
        return;
    }

    if (!useThreshold) {
        outputImage = colorInput.clone();
    } else {
        if (grayInput.empty()) {
            std::cerr << "ThresholdNode::process() - grayscale is empty\n";
            outputImage = colorInput.clone();
        } else {
            switch (method) {
                case ThresholdMethod::Binary:
                    cv::threshold(grayInput, outputImage, thresholdValue, 255, cv::THRESH_BINARY);
                    break;
                case ThresholdMethod::Otsu:
                    computedOtsuThresh = cv::threshold(grayInput, outputImage, 0, 255,
                                                       cv::THRESH_BINARY | cv::THRESH_OTSU);
                    break;
                case ThresholdMethod::Adaptive:
                    if (adaptiveBlockSize % 2 == 0) adaptiveBlockSize++;  // Must be odd
                    if (adaptiveBlockSize < 3) adaptiveBlockSize = 3;
                    cv::adaptiveThreshold(grayInput, outputImage, 255,
                                          cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                                          cv::THRESH_BINARY,
                                          adaptiveBlockSize, adaptiveC);
                    break;
            }
        }
    }

    // Histogram generation
    {
        cv::Mat histSource = grayInput.empty() ? colorInput.clone() : grayInput.clone();
        if (!histSource.empty() && histSource.channels() == 3) {
            cv::cvtColor(histSource, histSource, cv::COLOR_BGR2GRAY);
        }

        int histSize = 256;
        float range[] = { 0, 256 };
        const float* histRange = { range };
        cv::Mat hist;
        cv::calcHist(&histSource, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);

        histogramData.resize(histSize);
        for (int i = 0; i < histSize; ++i) {
            histogramData[i] = hist.at<float>(i);
        }
    }

    dirty = false;
}

void ThresholdNode::drawUI() {
    ImGui::Text("Threshold Node");
    bool changed = false;

    changed |= ImGui::Checkbox("Enable Thresholding", &useThreshold);

    const char* methods[] = { "Binary", "Otsu", "Adaptive" };
    int methodIdx = static_cast<int>(method);
    if (ImGui::Combo("Method", &methodIdx, methods, IM_ARRAYSIZE(methods))) {
        method = static_cast<ThresholdMethod>(methodIdx);
        changed = true;
    }

    if (method == ThresholdMethod::Binary) {
        changed |= ImGui::SliderInt("Threshold Value", &thresholdValue, 0, 255);
    } else if (method == ThresholdMethod::Otsu) {
        ImGui::Text("Otsu determines threshold automatically.");
        if (!grayInput.empty()) {
            ImGui::Text("Computed Threshold: %.2f", computedOtsuThresh);
        }
    } else if (method == ThresholdMethod::Adaptive) {
        changed |= ImGui::SliderInt("Block Size (odd)", &adaptiveBlockSize, 3, 31);
        changed |= ImGui::SliderInt("C Value", &adaptiveC, -20, 20);
    }

    if (changed) dirty = true;

    if (dirty) process();

    if (!histogramData.empty()) {
        ImGui::Text("Histogram:");
        float maxVal = *std::max_element(histogramData.begin(), histogramData.end());
        ImGui::PlotHistogram("##histogram", histogramData.data(),
                             static_cast<int>(histogramData.size()), 0,
                             nullptr, 0.0f, maxVal, ImVec2(0, 80));
    }
}
