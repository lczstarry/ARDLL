#include "stdafx.h"

#include <cstdarg>
#include <memory>
#include <unordered_map>

#include "opencv2/opencv.hpp"
#include "My_Function.h"
#include "Camera.h"
#include "MyController.h"

using namespace cv;
//static CCamera camera;
static MyController controller;
//CCamera *camera = CCamera::GetInstance();

extern "C"
{

	_DLLExport int Imagerows()
	{
		Mat img = imread("D:/OpenCV/OpencvSharp/OpenCvSharp-2.4.10-x64-20161106/My_CS_OpenCV/RZ.jpg");	
		//imshow("src", img);
		//waitKey(0);
		return img.cols;
	}

	//_DLLExport bool test_update_camera_frame(unsigned int *buffer)
	//{
	//	update_camera_frame(buffer);
	//	int width;
	//	int height;
	//	if (!get_camera_size(&width, &height))
	//		return false;

	//	cv::Mat frame(cv::Size(width, height), CV_8UC4, buffer);
	//	cv::cvtColor(frame, frame, CV_RGBA2BGR);

	//	int key = cvWaitKey(30);

	//	if (key == 32)
	//		return false;

	//	return true;
	//}

	_DLLExport bool get_camera_size(int *width, int *height)
	{
		
		return controller.get_camera_size(width,height);
	}

	_DLLExport bool update_camera_frame(unsigned int *buffer)
	{ 
		//cv::Mat frame;
		//int width;
		//int height;
		//if (!get_camera_size(&width, &height))
		//{
		//	return false;
		//}

		//cv::Mat out(cv::Size(240, 320), CV_8UC4, buffer);
		////camera.GetCameraFrame(frame);
		//cv::cvtColor(frame, out, CV_RGBA2BGR);
		//return true;

		return controller.read_frame(buffer);
		
	}


	_DLLExport bool open_camera()
	{
		//CCamera *camera = new CCamera();
		return controller.start_camera();
		 
	}

	_DLLExport int count()
	{
		CCamera *camera = new CCamera();
		return camera->count();
	}

	_DLLExport bool close_camera()
	{
		return controller.stop_camera();
	}

    _DLLExport bool get_projection(float *m, float width, float height, float near_plane, float far_plane)
	{
		return true;
	}

	_DLLExport bool get_texture_filepath(char *filepath)
	{
		//Mat img = imread(filepath);
		
		//waitKey(0);

		controller.get_texture_filepath_detectkeypoint(filepath);
		return true;
	}

	_DLLExport void cameraframe_detekeypoints()
	{
		controller.cameraframe_detekeypoints();
	}

	_DLLExport bool best_Match()
	{
		return controller.best_Match();
    }

	_DLLExport int Homography()
	{
		return controller.Homography();
	}
}