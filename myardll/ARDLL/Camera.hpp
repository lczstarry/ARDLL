#pragma once
#include <opencv2/opencv.hpp>


using namespace cv;

class CCamera
{
public:
	CCamera();
	~CCamera();
	bool OpenCamera();  //�������
	bool CloseCamera();  //�ر������
	bool GetCameraSize(int *width, int *height);  //��ȡ������ĳ����С

	bool GetCameraFrame(cv::Mat &frame); //��ȡ�����ÿ֡������

	int count();


private:
	cv::VideoCapture video_capture;
	bool is_opencamera;

	

	int c_width;
	int c_height;

};

