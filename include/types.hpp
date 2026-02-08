#pragma once
#include <opencv2/opencv.hpp>

// 灯条
struct LightBar {
    cv::RotatedRect rect;
    float length;
    float angle;
    cv::Point2f center;

    LightBar() = default;
    LightBar(const cv::RotatedRect& r); 
};

// 装甲板
struct ArmorPlate {
    LightBar left, right;
    cv::Point2f center;
    float x_3d = 0;
    float y_3d = 0;
    float z_3d = 0;
};