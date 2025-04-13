// filepath: c:\Users\jatin\node-image-processor\src\OpenGLHelper.h
#pragma once
#include <GL/glew.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

namespace OpenGLHelper {

// Converts a cv::Mat to an OpenGL texture.
// If existingTextureID is zero, a new texture is generated.
inline GLuint cvMatToTexture(const cv::Mat &mat, GLuint &existingTextureID) {
    if (mat.empty())
        return 0;

    cv::Mat matRGBA;
    if (mat.channels() == 3) {
        cv::cvtColor(mat, matRGBA, cv::COLOR_BGR2RGBA);
    } else if (mat.channels() == 4) {
        matRGBA = mat.clone(); // Already 4 channels
    } else {
        cv::cvtColor(mat, matRGBA, cv::COLOR_GRAY2RGBA);
    }

    if (existingTextureID == 0) {
        glGenTextures(1, &existingTextureID);
    }

    glBindTexture(GL_TEXTURE_2D, existingTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        matRGBA.cols, matRGBA.rows, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, matRGBA.data
    );
    glBindTexture(GL_TEXTURE_2D, 0);

    return existingTextureID;
}

} 