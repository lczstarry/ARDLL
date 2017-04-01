#pragma once
#include "opencv2/opencv.hpp"


using namespace cv;

class CCamera
{
public:	
	CCamera();
	~CCamera();
	bool OpenCamera();  //打开摄像机
	bool CloseCamera();  //关闭摄像机
	bool GetCameraSize(int *width, int *height);  //获取摄像机的长宽大小

	bool GetCameraFrame(cv::Mat &frame); //读取摄像机每帧的数据

	int count();
	/*static CCamera *GetInstance()
	{
		if (Instance == NULL)
		{
			Instance = new CCamera();	
		}
		return Instance;
	}*/
	
private:
	cv::Ptr<cv::VideoCapture> video_capture;
	bool is_opencamera;

	/*CCamera()
	{
		video_capture = cv::Ptr<cv::VideoCapture>();
		is_opencamera = false;		
	}*/
	//static CCamera *Instance; //摄像机类的单例
	 
	int c_width;
	int c_height;
	
};

