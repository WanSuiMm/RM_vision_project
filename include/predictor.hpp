#pragma once
#include <opencv2/opencv.hpp>

class KalmanPredictor {
public:
    KalmanPredictor();
    cv::Point2f predict(double current_time);
    void correct(cv::Point2f center, double current_time);

private:
    cv::KalmanFilter kf;
    cv::Mat meas;
    double last_time;
    bool is_initialized;
    void init_kf();
};