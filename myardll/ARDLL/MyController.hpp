#pragma once
#include <memory>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "Camera.hpp"
#include "PatternDetector.hpp"

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

	bool found_texture();
	bool getPatternLocation(char *filepath, char*rtpath);

private:
	std::shared_ptr<CCamera> camera;
	float projection[16];

	

	cv::Mat Image;
	cv::Mat frame; // ÉãÏñ»úµÄ»­Ãæ

};

