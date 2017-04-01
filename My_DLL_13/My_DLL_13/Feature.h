#pragma once
#include <iostream>
#include <vector>
#include <opencv2\opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

class CFeature
{
public:
	CFeature();
	~CFeature();

	void extractDescriptors(const cv::Mat& image, vector<KeyPoint>& keypoints, cv::Mat& descriptors); //提取特征点
	void detectKeypoints(const cv::Mat& image, vector<KeyPoint>& keypoints); // 检查特征点
	bool bestMatch(const cv::Mat& queryDescriptor, cv::Mat& trainDescriptor, vector<DMatch>& matches);
	void knnMatch(const cv::Mat& queryDesriptor, cv::Mat& trainDescriptor, vector<vector<DMatch>>& matches, int k);
	bool homography(const vector<KeyPoint> & F_keypoints, const vector<KeyPoint>& I_keypoints,cv::Mat& homography);
private:
	Ptr<FeatureDetector> m_detector;
	Ptr<DescriptorExtractor> m_extractor;
	Ptr<DescriptorMatcher> m_matcher;

	string m_detectType;
	string m_extractType;
	string m_matchType;
};

