#include "predictor.hpp"

using namespace cv;

KalmanPredictor::KalmanPredictor() {
    is_initialized = false;
    init_kf();
}

void KalmanPredictor::init_kf() {
    kf.init(4, 2, 0); 
    kf.transitionMatrix = (Mat_<float>(4, 4) << 
        1, 0, 1, 0, 
        0, 1, 0, 1, 
        0, 0, 1, 0, 
        0, 0, 0, 1);
    setIdentity(kf.measurementMatrix);
    setIdentity(kf.processNoiseCov, Scalar::all(1e-4));
    setIdentity(kf.measurementNoiseCov, Scalar::all(1e-1));
    setIdentity(kf.errorCovPost, Scalar::all(1));
    meas = Mat::zeros(2, 1, CV_32F);
}

Point2f KalmanPredictor::predict(double current_time) {

    if (!is_initialized) return Point2f(0, 0);
    
    double dt = current_time - last_time;

    if (dt <= 0) dt = 0.01;

    last_time = current_time;

    kf.transitionMatrix.at<float>(0, 2) = (float)dt;
    kf.transitionMatrix.at<float>(1, 3) = (float)dt;

    Mat p = kf.predict();
    return Point2f(p.at<float>(0), p.at<float>(1));
}

void KalmanPredictor::correct(Point2f center, double current_time) {

    if (!is_initialized) {
        is_initialized = true;
        kf.statePost.at<float>(0) = center.x;
        kf.statePost.at<float>(1) = center.y;
        kf.statePost.at<float>(2) = 0;
        kf.statePost.at<float>(3) = 0;
        last_time = current_time;
        return;
    }
    
    meas.at<float>(0) = center.x;
    meas.at<float>(1) = center.y;
    kf.correct(meas);
}