#include "NoiseGenerationNode.h"
#include <imgui.h>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <random>


void NoiseGenerationNode::setInputImage(const cv::Mat& image) {
    inputImage = image.clone(); // Used if displacement is needed, or for passing through
    dirty = true;
}

const cv::Mat& NoiseGenerationNode::getOutputImage() const {
    return outputImage;
}

// Basic Perlin-like pseudo noise for demo purposes
float NoiseGenerationNode::generateNoiseValue(float x, float y) {
    float value = 0.0f;
    float amplitude = 1.0f;
    float frequency = scale;
    float maxValue = 0.0f;

    for (int i = 0; i < octaves; ++i) {
        float nx = x * frequency;
        float ny = y * frequency;
        float noise = static_cast<float>(std::sin(nx * 12.9898f + ny * 78.233f) * 43758.5453f);
        noise = noise - std::floor(noise); // fractional part
        value += noise * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.0f;
    }
    return value / maxValue;
}

void NoiseGenerationNode::process() {
    // If noise generation is not enabled, pass through the original image.
    if (!useNoise) {
        // Ensure that if an input image is available, it is passed through.
        if (!inputImage.empty()) {
            outputImage = inputImage.clone();
        }
        dirty = false;
        return;
    }

    // Otherwise generate the noise image.
    outputImage = cv::Mat(height, width, CV_8UC1);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float noiseVal = generateNoiseValue(static_cast<float>(x), static_cast<float>(y));
            outputImage.at<uchar>(y, x) = static_cast<uchar>(noiseVal * 255);
        }
    }

    if (outputMode == NoiseOutputMode::Color) {
        cv::cvtColor(outputImage, outputImage, cv::COLOR_GRAY2BGR);
    }
    dirty = false;
}

void NoiseGenerationNode::drawUI() {
    ImGui::Text("Noise Generation Node");
    bool changed = false;

    // Checkbox to enable or disable noise generation
    changed |= ImGui::Checkbox("Enable Noise", &useNoise);
    
    // Noise type selection
    const char* types[] = { "Perlin", "Simplex", "Worley" };
    int noiseIdx = static_cast<int>(noiseType);
    if (ImGui::Combo("Noise Type", &noiseIdx, types, IM_ARRAYSIZE(types))) {
        noiseType = static_cast<NoiseType>(noiseIdx);
        changed = true;
    }

    // Output mode
    const char* modes[] = { "Color Output", "Displacement Map" };
    int outputIdx = static_cast<int>(outputMode);
    if (ImGui::Combo("Output Mode", &outputIdx, modes, IM_ARRAYSIZE(modes))) {
        outputMode = static_cast<NoiseOutputMode>(outputIdx);
        changed = true;
    }

    changed |= ImGui::SliderFloat("Scale", &scale, 0.001f, 0.1f);
    changed |= ImGui::SliderInt("Octaves", &octaves, 1, 8);
    changed |= ImGui::SliderFloat("Persistence", &persistence, 0.1f, 1.0f);

    if (changed) {
        dirty = true;
    }

    if (dirty) {
        process();
    }
    if (ImGui::Button("Reset")) {
        reset(); // Call the reset method
    }
}
