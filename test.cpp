// g++ test.cpp -o test `pkg-config --cflags --libs opencv`

#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

int main()  
{  
    // VideoCapture video("./TB.mp4");  
    VideoCapture video(0);
    Mat frame, mask, thresholdImage, output;  
    int frameNum = 1;  
    if (!video.isOpened())  
        cout << "fail to open!" << endl;  
    //cout<<video.isOpened();  
    long totalFrameNumber = video.get(CV_CAP_PROP_FRAME_COUNT);  
    video>>frame;  
    //cout<<frame.size;  
    BackgroundSubtractorMOG2 bgSubtractor(25, 10, true);
    //imshow("video", frame);  
    //waitKey(10000);  
      
    while (true){  
        if (totalFrameNumber == frameNum)  
            break;  
        video >> frame;  
        ++frameNum;  
        bgSubtractor(frame, mask, -1);  
        //cout << frameNum << endl;  
        imshow("mask",mask);    
        waitKey(10);    
    }  
      
    return 0;  
}  