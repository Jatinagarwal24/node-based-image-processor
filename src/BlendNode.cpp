#include "BlendNode.h"
#include <opencv2/imgproc.hpp>
#include <imgui.h>
#include <iostream>

BlendNode::BlendNode()
    : NodeBase("Blend Node"), blendMode(BlendMode::Normal), opacity(0.5f), processed(false)
{
}

void BlendNode::setBlendImage(const cv::Mat& imageA, const cv::Mat& imageB) {
    inputImage = imageA;       // Primary image
    blendImage = imageB;       // The image to blend with
    dirty = true;              // Mark dirty so that process() knows it must update
}

const cv::Mat& BlendNode::getOutputImage() const {
    return outputImage;
}

void BlendNode::process() {
    // Only process when dirty is true
    if (!dirty)
        return;
        
    if (inputImage.empty() || blendImage.empty()) {
        std::cerr << "BlendNode::process(): One or both input images are empty." << std::endl;
        return;
    }
    
    cv::Mat imgA, imgB;
    inputImage.copyTo(imgA);
    
    // Ensure the blend image is resized to match imgA if needed
    if (inputImage.size() != blendImage.size())
        cv::resize(blendImage, imgB, inputImage.size());
    else
        blendImage.copyTo(imgB);
    
    // Convert blend image to the same type if different
    if (imgA.type() != imgB.type()) {
        imgB.convertTo(imgB, imgA.type());
    }
    
    // Perform blending based on the selected blend mode
    switch (blendMode) {
        case BlendMode::Normal:
            cv::addWeighted(imgA, opacity, imgB, 1.0f - opacity, 0.0, outputImage);
            break;
            
        case BlendMode::Multiply:
            cv::multiply(imgA, imgB, outputImage, opacity / 255.0);
            break;
            
        case BlendMode::Screen: {
            cv::Mat invA, invB, blended;
            cv::subtract(cv::Scalar::all(255), imgA, invA);
            cv::subtract(cv::Scalar::all(255), imgB, invB);
            cv::multiply(invA, invB, blended, 1.0 / 255.0);
            cv::subtract(cv::Scalar::all(255), blended, blended);
            cv::addWeighted(imgA, 1.0f - opacity, blended, opacity, 0.0, outputImage);
            break;
        }
        
        case BlendMode::Overlay: {
            outputImage = imgA.clone();
            for (int y = 0; y < imgA.rows; y++) {
                for (int x = 0; x < imgA.cols; x++) {
                    for (int c = 0; c < imgA.channels(); c++) {
                        uchar A = imgA.at<cv::Vec3b>(y, x)[c];
                        uchar B = imgB.at<cv::Vec3b>(y, x)[c];
                        float result = (A < 128)
                            ? (2.0f * A * B / 255.0f)
                            : (255.0f - 2.0f * (255.0f - A) * (255.0f - B) / 255.0f);
                        outputImage.at<cv::Vec3b>(y, x)[c] =
                            cv::saturate_cast<uchar>(result * opacity + A * (1.0f - opacity));
                    }
                }
            }
            break;
        }
        
        case BlendMode::Difference:
            cv::absdiff(imgA, imgB, outputImage);
            break;
        
        default:
            std::cerr << "BlendNode::process(): Unknown blend mode!" << std::endl;
            break;
    }
    
    processed = true;
    dirty = false;  // Processing complete
}

void BlendNode::drawUI() {
    ImGui::Text("Blend Node");

    const char* modes[] = { "Normal", "Multiply", "Screen", "Overlay", "Difference" };
    int currentMode = static_cast<int>(blendMode);
    bool changed = false;
    
    if (ImGui::Combo("Blend Mode", &currentMode, modes, IM_ARRAYSIZE(modes))) {
        blendMode = static_cast<BlendMode>(currentMode);
        changed = true;
    }

    if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f)) {
        changed = true;
    }

    if (changed) {
        dirty = true;
    }

    if (!outputImage.empty()) {
        ImGui::Text("Output image ready.");
    } else {
        ImGui::Text("No output image.");
    }
}