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
    void reset() override {
        NodeBase::reset(); // Call base class reset
        useNoise = false; // Default to no noise
        noiseType = NoiseType::Perlin; // Default noise type
        outputMode = NoiseOutputMode::Color; // Default output mode
        scale = 0.05f; // Default scale
        octaves = 3; // Default octaves
        persistence = 0.5f; // Default persistence
        width = 512; // Default width
        height = 512; // Default height
    }
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
