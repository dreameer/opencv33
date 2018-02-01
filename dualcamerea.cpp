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
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <cstring>
#include <Windows.h>

#define w 320
#define h 160

using namespace std;
using namespace cv;

int main1(){

	VideoCapture left,right;
	char *leftname[14] = {"left01.jpg","left02.jpg","left03.jpg","left04.jpg","left05.jpg","left06.jpg","left07.jpg","left08.jpg","left09.jpg","left10.jpg","left11.jpg","left12.jpg","left13.jpg","left14.jpg"};
	char *rightname[14] = {"right01.jpg","right02.jpg","right03.jpg","right04.jpg","right05.jpg","right06.jpg","right07.jpg","right08.jpg","right09.jpg","right10.jpg","right11.jpg","right12.jpg","right13.jpg","right14.jpg"};
	bool isopen = false;
	if (left.open(0)) {
		printf("open left camera!\n");
		left.set(CAP_PROP_FRAME_WIDTH,w);
		left.set(CAP_PROP_FRAME_HEIGHT,h);
		Sleep(1000);
		if(right.open(1)){
			printf("open right camera!\n");
			right.set(CAP_PROP_FRAME_WIDTH,w);
			right.set(CAP_PROP_FRAME_HEIGHT,h);
			isopen = true;
		}else{
			printf("open right camera false\n");
		}
	}
	else{
		printf("open left camera false\n");
	}
	if(false){
		getchar();
	}
	else {
		int imageserial = 0;
		for(;;){
			//float fps = left.get(CAP_PROP_FPS);
			//printf("fps:%4f\n",fps);
			Mat leftframe,rightframe;
			Mat leftgray,rightgray;
			left >> leftframe;
			right >> rightframe;
			imshow("left",leftframe);
			imshow("right",rightframe);
			char cmd = waitKey(1);
			if(cmd=='c'){
				if(imageserial>=14)break;
				imwrite(leftname[imageserial],leftframe);
				imwrite(rightname[imageserial],rightframe);
				imageserial++;
				printf("imageserial:%d\n",imageserial);
			}
			if(cmd=='a'){
				imwrite("left.jpg",leftframe);
				imwrite("right.jpg",rightframe);
			}
			if(cmd=='b'){break;
			}
		}
		for(;;){
			Mat leftframe,rightframe;
			Mat imgLeft,imgRight;
			left >> leftframe;
			right >> rightframe;
			cvtColor(leftframe,imgLeft,CV_BGR2GRAY);
			cvtColor(rightframe,imgRight,CV_BGR2GRAY);
			//-- And create the image in which we will save our disparities
			Mat imgDisparity16S = Mat( imgLeft.rows, imgLeft.cols, CV_16S );
			Mat imgDisparity8U = Mat( imgLeft.rows, imgLeft.cols, CV_8UC1 );

			if( imgLeft.empty() || imgRight.empty() )
			{ std::cout<< " --(!) Error reading images " << std::endl; 
			getchar();return -1; }

			//-- 2. Call the constructor for StereoBM
			int ndisparities = 0;   /**< Range of disparity */
			int SADWindowSize = 5; /**< Size of the block window. Must be odd */

			Ptr<StereoBM> sbm = StereoBM::create( ndisparities, SADWindowSize );

			//-- 3. Calculate the disparity image
			sbm->compute( imgLeft, imgRight, imgDisparity16S );

			//-- Check its extreme values
			double minVal; double maxVal;

			minMaxLoc( imgDisparity16S, &minVal, &maxVal );

			printf("Min disp: %f Max value: %f \n", minVal, maxVal);

			//-- 4. Display it as a CV_8UC1 image
			imgDisparity16S.convertTo( imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));



			//--5. reprojectImageTo3D
			Mat R = (Mat_<double>(4,4) <<1., 0., 0., -1.5309913635253906e+002, 0., 1., 0.,
				-1.1647804450988770e+002, 0., 0., 0., 2.0915136830804386e+002, 0.,
				0., 5.8362550337003383e-001, 0. );
			Mat mat3d;
			reprojectImageTo3D(imgDisparity16S,mat3d,R);
			vector<Mat> mv;
			split(mat3d,mv);
			Mat deepimg;
			mv[2].convertTo(deepimg, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

			imshow("deepimg",deepimg);
			waitKey(10);
		}

	}
	return 0;
}




