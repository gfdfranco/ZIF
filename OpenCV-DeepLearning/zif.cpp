









// sudo modprobe bcm2835-v4l2
//g++ -o zif zif.cpp $(pkg-config --cflags --libs opencv)

#include "opencv2/opencv.hpp"
using namespace cv;
int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    namedWindow("img",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        imshow("img", frame);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}