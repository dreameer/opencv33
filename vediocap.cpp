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


int main4(){
	VideoCapture cap(1);
	cap.set(CAP_PROP_FRAME_WIDTH,1280);
	cap.set(CAP_PROP_FRAME_HEIGHT,760);
	VideoWriter outputVideo;
	if (!cap.isOpened()) {
		printf("cant open camera!\n");
	} else {
		for(;;)
		{
			int fps = cap.get(CAP_PROP_FPS);
			printf("fps:%d\n",fps);
			Mat src; 

			cap >> src;
			imshow("frame", src);
			char cmd = waitKey(1);
			if (cmd == 'b')
				break;
			if (cmd == 'c'){
				Size S = Size((int) cap.get(CAP_PROP_FRAME_WIDTH),(int) cap.get(CAP_PROP_FRAME_HEIGHT));
				outputVideo.open("output.avi", CV_FOURCC('D','I','V','X'), cap.get(CAP_PROP_FPS), S, true);
				if (!outputVideo.isOpened())
				{
					cout  << "Could not open the output video for write: " << endl;
				}
				else{
					cout << "open camera success"<<endl;
				}
			}
		}
	}
	return 0;
}
