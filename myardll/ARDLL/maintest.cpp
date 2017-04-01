#define EXPORTBUILD
#include "ARPipeline.hpp"
#include"maintest.hpp"
#include <opencv2/opencv.hpp>
#include <fstream> 
#include"MyController.hpp"
using namespace std;

static  MyController controller;


bool open_camera()
{
	
	return controller.start_camera();

}
bool open_camera1()
{

	cv::VideoCapture capture;
	capture.open(0);
	return true;

}
int Imagerows()
{
	Mat img = imread("G:/Pattern.jpg");
	
	return img.cols;
}
bool get_camera_size(int *width, int *height)
{

	return controller.get_camera_size(width, height);
}
 bool update_camera_frame(unsigned int *buffer)
{


	return controller.read_frame(buffer);

}
 int count()
 {
	 CCamera *camera = new CCamera();
	 return camera->count();
 }

  bool close_camera()
 {
	 return controller.stop_camera();
 }
 bool get_projection(float *m, float width, float height, float near_plane, float far_plane)
 {
	 return true;
 }

 bool getPatternLocation(char *filepath, char*rtpath)
 {
	

	 return controller.getPatternLocation(filepath, rtpath);
	  
 }

 