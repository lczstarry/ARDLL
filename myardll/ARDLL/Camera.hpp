#pragma once
#include <opencv2/opencv.hpp>


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


private:
	cv::VideoCapture video_capture;
	bool is_opencamera;

	

	int c_width;
	int c_height;

};

