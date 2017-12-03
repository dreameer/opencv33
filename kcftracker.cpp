#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
using namespace std;
using namespace cv;

#pragma comment(lib,"opencv_tracking331d.lib")
#pragma comment(lib,"opencv_datasets331d.lib")

int main( int argc, char** argv ){

	/*default params:
	  TrackerKCF::Params::Params(){
      detect_thresh = 0.5f; //!<  detection confidence threshold
      sigma=0.2f;          //!<  gaussian kernel bandwidth
      lambda=0.0001f;       //!<  regularization
      interp_factor=0.075f;  //!<  linear interpolation factor for adaptation
      output_sigma_factor=1.0f / 16.0f;     //!<  spatial bandwidth (proportional to target)
      resize=true; //!<  activate the resize feature to improve the processing speed
      max_patch_size=80*80;      //!<  threshold for the ROI size
      split_coeff=true;    //!<  split the training coefficients into two matrices
      wrap_kernel=false;  //!<  wrap around the kernel values
      desc_npca = img;    //!<  non-compressed descriptors of TrackerKCF::MODE
      desc_pca = CN;      //!<  compressed descriptors of TrackerKCF::MODE

      //feature compression
      compress_feature=true;   //!<  activate the pca method to compress the features
      compressed_size=2;      //!<  feature size after compression
      pca_learning_rate=0.15f; //!<  compression learning rate
  }  
  */

  Rect2d roi;
  Mat img,roig,roiPro;
  TrackerKCF::Params params;
  params.pca_learning_rate = 0.1f;
  params.detect_thresh = 0.25f;
  Ptr<Tracker> tracker;
  


  VideoCapture cap;
  cap.open("rtsp://:8554/aa",CAP_ANY);
  
  if(!cap.isOpened()){
	  printf("cant open\n");
	  getchar();
	  return -1;
  }
   
  int skipnum = 1;
  char cmd='c';
  bool initroi = true;
  printf("input skipnum\n");
  scanf_s("%d",&skipnum);
  for(int i=0;i<skipnum;i++)
	  cap >> img;
  
  printf("img size:%d,%d\n",img.cols,img.rows);
  namedWindow("roig",0);
  namedWindow("roiPro",0);
  
  for ( ;; ){
    cap >> img;
	
	pyrDown(img,img,Size(img.cols/2,img.rows/2));
	pyrDown(img,img,Size(img.cols/2,img.rows/2));
	
	if(cmd=='b' || initroi){
		initroi = false;
		tracker = TrackerKCF::create();
		tracker->save("params.data");
		roi=selectROI("select roi",img);
		printf("tracker-init:%d\n",tracker->init(img,roi));
		tracker->update(img,roi);
	}
    if(img.rows==0 || img.cols==0)
      break;

	if(tracker->update(img,roi))
	{
		
		if( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= img.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= img.rows )
		{
			printf("roi:%4f %4f %4f %4f\n",roi.x,roi.y,roi.width,roi.height);
			img(roi).copyTo(roig);
			Canny(roig, roiPro, 150, 160, 3);
			vector<Vec4i> linesP; 


			HoughLinesP(roiPro, linesP, 1, CV_PI/180, 10, 10, 4 );
			for( size_t i = 0; i < linesP.size(); i++ )
			{
				Vec4i l = linesP[i];
				float tanofangle = abs((float)(l[3]-l[1])/(l[2]-l[0]));
				float distance2center = sqrt(pow(((l[3]-l[1])*0.5+l[1]-roi.height*0.5),2)+pow(((l[2]-l[0])*0.5+l[0]-roi.width*0.5),2));
				printf("tanofangle distance2center:%3f,%3f\n",tanofangle,distance2center);
				line( roig, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, LINE_AA);
			}
			printf("linesp size:%d\n",linesP.size());
			imshow("roig",roig);
			imshow("roiPro",roiPro);
		}
		else
		{
		}
		rectangle( img, roi, Scalar( 255, 0, 0 ), 2, 1 );
	}else
	{
		rectangle( img, roi, Scalar( 0, 255, 0 ), 2, 1 );
	}
    imshow("img",img);
    cmd = waitKey(1);
	if(cmd=='a')break;

  }
  return 0;
}