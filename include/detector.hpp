#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "types.hpp"
#include "settings.hpp"

class ArmorDetector {
public:
    ArmorDetector();
    std::vector<ArmorPlate> detect(const cv::Mat& frame);

private:
    cv::Mat preprocess(const cv::Mat& frame);
    std::vector<LightBar> findLights(const cv::Mat& mask);
    std::vector<ArmorPlate> matchArmors(const std::vector<LightBar>& lights);
    cv::Mat debug_img;
};