#pragma once
#include "opencv2/opencv.hpp"


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
	//static CCamera *Instance; //�������ĵ���
	 
	int c_width;
	int c_height;
	
};

