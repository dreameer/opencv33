/**
 * @file SBM_Sample
 * @brief Get a disparity map of two images
 * @author A. Huaman
 */

#include <iostream>
#include <vector>
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"



using namespace cv;
using namespace std;
const char *windowDisparity = "Disparity";


/**
 * @function main
 * @brief Main function
 */

int main()
{
	char *leftname[14] = {"left01.jpg","left02.jpg","left03.jpg","left04.jpg","left05.jpg","left06.jpg","left07.jpg","left08.jpg","left09.jpg","left10.jpg","left11.jpg","left12.jpg","left13.jpg","left14.jpg"};
	char *rightname[14] = {"right01.jpg","right02.jpg","right03.jpg","right04.jpg","right05.jpg","right06.jpg","right07.jpg","right08.jpg","right09.jpg","right10.jpg","right11.jpg","right12.jpg","right13.jpg","right14.jpg"};
	for(int i=0;i<14;i++){
		//-- 1. Read the images
		Mat imgLeft = imread( leftname[i], IMREAD_GRAYSCALE );
		Mat imgRight = imread( rightname[i], IMREAD_GRAYSCALE );
		//-- And create the image in which we will save our disparities
		Mat imgDisparity16S = Mat( imgLeft.rows, imgLeft.cols, CV_16S );
		Mat imgDisparity8U = Mat( imgLeft.rows, imgLeft.cols, CV_8UC1 );

		if( imgLeft.empty() || imgRight.empty() )
		{ std::cout<< " --(!) Error reading images " << std::endl; 
		getchar();return -1; }

		//-- 2. Call the constructor for StereoBM
		int ndisparities = 16*5;   /**< Range of disparity */
		int SADWindowSize = 21; /**< Size of the block window. Must be odd */

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
		Mat R = (Mat_<double>(4,4) << 1., 0., 0., -2.5239142608642578e+002, 0., 1., 0.,
       -2.2994655036926270e+002, 0., 0., 0., 3.2947142980851788e+002, 0.,
       0., -9.5262563587360261e-001, 0. );
		Mat mat3d;
		reprojectImageTo3D(imgDisparity16S,mat3d,R);
		vector<Mat> mv;
		split(mat3d,mv);

		namedWindow( windowDisparity, WINDOW_NORMAL );
		imshow( windowDisparity, imgDisparity8U );
		imshow("left",imgLeft);
		imshow("3d2",mv[2]);
		//-- 5. Save the image
		imwrite("SBM_sample.png", imgDisparity16S);

		waitKey(0);
	}
	return 0;
}
