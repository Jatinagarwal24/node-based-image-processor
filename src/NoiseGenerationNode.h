#pragma once
#include "NodeBase.h"
#include <opencv2/core.hpp>

enum class NoiseType { Perlin, Simplex, Worley };
enum class NoiseOutputMode { Color, Displacement };

class NoiseGenerationNode : public NodeBase {
public:
    NoiseGenerationNode();

    void setInputImage(const cv::Mat& image);
    const cv::Mat& getOutputImage() const ;
    void process() override;
    void drawUI() override;
    void reset() override {
        NodeBase::reset();
        useNoise = false; // Reset to default state
        noiseType = NoiseType::Perlin;
        outputMode = NoiseOutputMode::Color;
        scale = 0.01f;
        octaves = 4;
        persistence = 0.5f;
        width = 512; // Default width
        height = 512; // Default height
    }

    // All members are public per your earlier request
    cv::Mat outputImage;
    cv::Mat inputImage;

    bool useNoise;  // New toggle flag: if false, passes input image through

    NoiseType noiseType;
    NoiseOutputMode outputMode;

    float scale;
    int octaves;
    float persistence;
    int width;
    int height;

    float generateNoiseValue(float x, float y); // Simulated Perlin-style noise
};