#include "detector.hpp"
#include <cmath>

using namespace cv;
using namespace std;

LightBar::LightBar(const RotatedRect& r) : rect(r) {
    Size2f s = r.size;
    length = (s.width > s.height) ? s.width : s.height;
    center = r.center;
    angle = (s.width > s.height) ? (r.angle + 90.0f) : r.angle;
}

ArmorDetector::ArmorDetector() {}

vector<ArmorPlate> ArmorDetector::detect(const Mat& frame) {
    Mat mask = preprocess(frame);
    vector<LightBar> lights = findLights(mask);
    return matchArmors(lights);
}

Mat ArmorDetector::preprocess(const Mat& frame) {
    Mat hsv, mask;
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    
    if (DETECT_BLUE) {
        inRange(hsv, Scalar(100, 43, 46), Scalar(124, 255, 255), mask);
    } 
    else {
        Mat mask1, mask2;
        inRange(hsv, Scalar(0, 43, 46), Scalar(10, 255, 255), mask1);
        inRange(hsv, Scalar(160, 43, 46), Scalar(180, 255, 255), mask2);
        mask = mask1 | mask2;
    }
    
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(mask, mask, MORPH_CLOSE, kernel);
    return mask;
}

vector<LightBar> ArmorDetector::findLights(const Mat& mask) {
    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<LightBar> lights;

    for (const auto& cnt : contours) {

        if (contourArea(cnt) < 20) continue;
        
        RotatedRect r = minAreaRect(cnt);
        LightBar lb(r);
        
        float w = (r.size.width < r.size.height) ? r.size.width : r.size.height;

        if (lb.length / w < 1.5f) continue;

        if (abs(lb.angle) > 45.0f) continue;

        lights.push_back(lb);
    }
    return lights;
}

vector<ArmorPlate> ArmorDetector::matchArmors(const vector<LightBar>& lights) {
    vector<ArmorPlate> armors;
    for (size_t i = 0; i < lights.size(); i++) {

        for (size_t j = i + 1; j < lights.size(); j++) {
            const LightBar& l1 = lights[i];
            const LightBar& l2 = lights[j];

            if (abs(l1.angle - l2.angle) > 8.0f) continue;

            if (abs(l1.length - l2.length) / max(l1.length, l2.length) > 0.4f) continue;

            const LightBar& left = (l1.center.x < l2.center.x) ? l1 : l2;
            const LightBar& right = (l1.center.x < l2.center.x) ? l2 : l1;
            float y_diff = abs(left.center.y - right.center.y);
            float avg_len = (left.length + right.length) / 2.0f;

            if (y_diff / avg_len > 0.5f) continue;

            float dis = norm(left.center - right.center);
            float ratio = dis / avg_len;

            if (ratio < 1.5f || ratio > 5.5f) continue;

            ArmorPlate armor;
            armor.left = left;
            armor.right = right;
            armor.center = (left.center + right.center) / 2.0f;
            armors.push_back(armor);
        }

    }

    return armors;
}