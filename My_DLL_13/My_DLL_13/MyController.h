#pragma once
#include <memory>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "Camera.h"
#include "Feature.h"

using namespace std;
using namespace cv;

class MyController
{
public:
	MyController();
	~MyController();

	bool start_camera();
	bool stop_camera();
	bool read_frame(unsigned int *buffer);
	bool get_camera_size(int *width, int *height);
	//void draw_result();
   // bool get_projection(float *m, float width, float height, float near_plane, float far_plane);
	bool found_texture();
	void get_texture_filepath_detectkeypoint(const string str);
	void cameraframe_detekeypoints();
	bool best_Match();
	int Homography();
	bool mark;
private:
	std::shared_ptr<CCamera> camera;
	float projection[16];

	std::shared_ptr<CFeature> feature;
	//CFeature *feature;
	string file_path;
	cv::Mat Image;
	cv::Mat frame; // ÉãÏñ»úµÄ»­Ãæ
	cv::Mat frame_GARY;
	cv::Mat queryDescriptor, trainDescriptor;
	vector<KeyPoint> querKeypoints,trainKeyPoints;
	vector<DMatch> matches;
};

