#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>

#include <libEasyPlayerAPI.h>

using namespace std;
using namespace cv;



int CALLBACK EasyPlayerCallBack( int _channelId, int *_channelPtr, int _frameType, char *pBuf, RTSP_FRAME_INFO* _frameInfo)
{
	
	printf("_channelId %d _frameType %d pBuf %c\n",_channelId,_frameType,pBuf);\
	if(_frameType==4)
	{
		printf("w h:%d %d\n",_frameInfo->width,_frameInfo->height);
	}
	
	return 0;
}

int main()
{

	int end_init = EasyPlayer_Init();
	int chanelid = EasyPlayer_OpenStream("rtsp://:8554/aa",NULL,DISPLAY_FORMAT_RGB565,1,NULL,NULL,&EasyPlayerCallBack);
	printf("%d\n",end_init);

	getchar();
	return 0;
}
