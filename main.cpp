#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#include "detector.hpp"
#include "solver.hpp"
#include "predictor.hpp"

using namespace cv;
using namespace std;

int main() {
    ArmorDetector detector;
    PoseSolver solver;
    KalmanPredictor predictor;
    
    VideoCapture cap("robomaster_test.mp4");

    if (!cap.isOpened()) return -1;

    Mat frame;

    while (true) {
        cap >> frame;

        if (frame.empty()) break;

        double t = (double)getTickCount() / getTickFrequency();
        vector<ArmorPlate> armors = detector.detect(frame);
        Mat debug_img = frame.clone();

        if (!armors.empty()) {
            ArmorPlate& target = armors[0];
            solver.solve(target);
            predictor.correct(target.center, t);
            line(debug_img, target.left.center, target.right.center, Scalar(0, 0, 255), 2);
            string info = "Z: " + to_string(target.z_3d).substr(0, 4) + "m";
            putText(debug_img, info, target.center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0), 2);
        }
        Point2f pred = predictor.predict(t + 0.033);
        circle(debug_img, pred, 5, Scalar(0, 255, 255), 2);
        imshow("RM Vision Modular", debug_img);

        if (waitKey(1) == 'q') break;
       
    }

    return 0;
}