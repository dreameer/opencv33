

#include <opencv2/opencv.hpp>
#include <iostream>
 
using namespace std;
using namespace cv;
 
int main()
{
        VideoCapture cap;
         cap.open("rtsp://192.168.1.100:554/stream0",CAP_ANY );
 
        Mat Camera_CImg;
        Mat Camera_GImg;
        //cap.set(CV_CAP_PROP_FRAME_HEIGHT,768);
        //cap.set(CV_CAP_PROP_FRAME_WIDTH,1024);
        if (!cap.isOpened())
			return 1024;
       
        while (1)
        {
                  cap>> Camera_CImg;
                  if(Camera_CImg.empty())
                  break;
                  cvtColor(Camera_CImg,Camera_GImg, CV_RGB2GRAY);
                  imshow("input", Camera_GImg);
                  if (cvWaitKey(10) == 'q')
                  break;
        }
        system("pause");
return 0;
}