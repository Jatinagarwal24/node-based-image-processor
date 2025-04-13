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
