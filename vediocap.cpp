/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/
#include <opencv2/core/utility.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cstring>



using namespace std;
using namespace cv;


int main(){
	VideoCapture cap("rtsp://admin:Qwer1234@192.168.1.64/Streaming/Channels/1");
	//cap.open(0,0);
	//cap.set(CAP_PROP_FRAME_WIDTH,640);
	
	//cap.set(CAP_PROP_FRAME_HEIGHT,480);
	//cout<<cap.set(CAP_PROP_EXPOSURE,-6)<<endl;
	//cap.set(CAP_PROP_AUTO_EXPOSURE,-1);
	//cout<<"exposure"<<cap.get(CAP_PROP_EXPOSURE);
	//cout<<"auto exposure"<<cap.get(CAP_PROP_AUTO_EXPOSURE);
	//cout<<"api"<<cap.get(CAP_PROP_MODE);
	if (!cap.isOpened()) {
		printf("cant open camera!\n");
	} else {
		Mat frame; 
		namedWindow("frame",0);
		for(;;)
		{
			int64 start = cv::getTickCount();
			//int fps = cap.get(CAP_PROP_FPS);
			//printf("fps:%d\n",fps);
			
			//cout<<"exposure"<<cap.get(CAP_PROP_EXPOSURE);
			cap >> frame;
		double fps = cv::getTickFrequency() / (cv::getTickCount()-start);
		//cout<<"fps:"<<fps<<endl;
		std::ostringstream stm;
		stm << fps;
		string frameinfo = " fps:" + stm.str();
		putText(frame, frameinfo, Point(10, 40),FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1, 8);
		imshow("frame",frame);
		if((char)waitKey(1)=='b')break;
		}
	}
	getchar();
	return 0;
}
