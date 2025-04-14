#pragma once
#include "NodeBase.h"
#include <opencv2/core.hpp>

// Enum to specify different types of noise
enum class NoiseType { Perlin, Simplex, Worley };

// Enum to specify different output modes for the noise (Color or Displacement)
enum class NoiseOutputMode { Color, Displacement };

class NoiseGenerationNode : public NodeBase {
public:
    // Constructor to initialize default values
    NoiseGenerationNode();

    // Function to set the input image
    void setInputImage(const cv::Mat& image);
    
    // Function to get the output image
    const cv::Mat& getOutputImage() const;
    
    // Function to process the noise generation
    void process() override;
    
    // Function to draw the UI controls for the node
    void drawUI() override;
    
    // Reset the node's parameters to default
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

    // Public member variables (as per your request)
    cv::Mat outputImage;      // Output image after processing
    cv::Mat inputImage;       // Input image for processing or displacement

    bool useNoise;            // Flag to toggle noise generation (if false, passes input image)
    
    // Noise type (Perlin, Simplex, or Worley)
    NoiseType noiseType;      
    
    // Output mode (Color output or Displacement map)
    NoiseOutputMode outputMode;
    
    // Parameters for noise generation
    float scale;              // Scale factor for noise
    int octaves;              // Number of octaves for Perlin-like noise
    float persistence;        // Persistence (influence of each octave)
    int width;                // Width of the generated noise image
    int height;               // Height of the generated noise image

private:
    // Helper function to generate noise value based on Perlin-like function
    float generateNoiseValue(float x, float y);
};
