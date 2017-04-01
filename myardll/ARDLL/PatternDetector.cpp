
#include "PatternDetector.hpp"
#include <cmath>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>

PatternDetector::PatternDetector(cv::Ptr<cv::FeatureDetector> detector,
	cv::Ptr<cv::DescriptorExtractor> extractor,
	cv::Ptr<cv::DescriptorMatcher> matcher,
	bool ratioTest)
	: m_detector(detector)
	, m_extractor(extractor)
	, m_matcher(matcher)
	, enableRatioTest(ratioTest)
	, enableHomographyRefinement(true)
	, homographyReprojectionThreshold(3)
{
}

void PatternDetector::getGray(const cv::Mat& image, cv::Mat& gray)
{
	if (image.channels() == 3)
		cv::cvtColor(image, gray, CV_BGR2GRAY);
	else if (image.channels() == 4)
		cv::cvtColor(image, gray, CV_BGRA2GRAY);
	else if (image.channels() == 1)
		gray = image;
}
bool PatternDetector::extractFeatures(const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors) const
{
	//assert(!image.empty());
	//assert(image.channels() == 1);

	m_detector->detect(image, keypoints); //特征点检测
	if (keypoints.empty())
		return false;

	m_extractor->compute(image, keypoints, descriptors);// 用Freak特征来描述特征点 ，提取描述符
	if (keypoints.empty())
		return false;

	return true;
}
void PatternDetector::buildPatternFromImage(const cv::Mat& image, Pattern& pattern) const
{
	int numImages = 4;
	float step = sqrtf(2.0f);//求平方根

	// Store original image in pattern structure
	pattern.size = cv::Size(image.cols, image.rows);
	pattern.frame = image.clone();
	getGray(image, pattern.grayImg);

	// Build 2d and 3d contours (3d contour lie in XY plane since it's planar)
	pattern.points2d.resize(4); //改变vector中元素的数目为4
	pattern.points3d.resize(4);

	// Image dimensions
	const float w = image.cols;
	const float h = image.rows;

	// Normalized dimensions:
	const float maxSize = std::max(w, h);
	const float unitW = w / maxSize;
	const float unitH = h / maxSize;

	pattern.points2d[0] = cv::Point2f(0, 0);
	pattern.points2d[1] = cv::Point2f(w, 0);
	pattern.points2d[2] = cv::Point2f(w, h);
	pattern.points2d[3] = cv::Point2f(0, h);

	pattern.points3d[0] = cv::Point3f(-unitW, -unitH, 0);
	pattern.points3d[1] = cv::Point3f(unitW, -unitH, 0);
	pattern.points3d[2] = cv::Point3f(unitW, unitH, 0);
	pattern.points3d[3] = cv::Point3f(-unitW, unitH, 0);

	extractFeatures(pattern.grayImg, pattern.keypoints, pattern.descriptors);
}
void PatternDetector::train(const Pattern& pattern)
{

	m_pattern = pattern;
	m_matcher->clear(); //清除旧的训练数据
	//我们增加描述子容器  
	//每一个描述子矩阵描述一副图像   
	//这使得我们可以执行多图像搜索
	std::vector<cv::Mat> descriptors(1);
	descriptors[0] = pattern.descriptors.clone();
	m_matcher->add(descriptors);
	//增加训练数据之后，执行实际训练
	m_matcher->train();

}
void PatternDetector::getMatches(const cv::Mat& queryDescriptors, std::vector<cv::DMatch>& matches)
{
	matches.clear();

	if (enableRatioTest)
	{
		//交叉匹配
		//当最好的匹配距离是0时，为了避免出现NaN（除数为0），我们将使用反比例
		const float minRatio = 1.f / 1.5f;

		//KNN匹配对于每一个查询描述子返回两个最邻近的匹配
		m_matcher->knnMatch(queryDescriptors, m_knnMatches, 2);

		for (size_t i = 0; i<m_knnMatches.size(); i++)
		{
			const cv::DMatch& bestMatch = m_knnMatches[i][0];
			const cv::DMatch& betterMatch = m_knnMatches[i][1];

			float distanceRatio = bestMatch.distance / betterMatch.distance;

			//仅通过最邻近匹配距离比例大于1.5的匹配 
			if (distanceRatio < minRatio)
			{
				matches.push_back(bestMatch);
			}
		}
	}
	else
	{
		// 常规匹配
		m_matcher->match(queryDescriptors, matches);
	}
}
bool PatternDetector::refineMatchesWithHomography(const std::vector<cv::KeyPoint>& queryKeypoints, const std::vector<cv::KeyPoint>& trainKeypoints, float reprojectionThreshold, std::vector<cv::DMatch>& matches, cv::Mat& homography)
{
	const int minNumberMatchesAllowed = 8;

	if (matches.size() < minNumberMatchesAllowed)
		return false;

	//为cv::findHomograph准备数据
	std::vector<cv::Point2f> srcPoints(matches.size());
	std::vector<cv::Point2f> dstPoints(matches.size());

	for (size_t i = 0; i < matches.size(); i++)
	{
		srcPoints[i] = trainKeypoints[matches[i].trainIdx].pt;//trainIdx  训练特征描述子索引 ；pt 坐标
		dstPoints[i] = queryKeypoints[matches[i].queryIdx].pt;
	}

	// 找到单应性矩阵并且获取内围层掩码 （inliers mask）
	std::vector<unsigned char> inliersMask(srcPoints.size());
	homography = cv::findHomography(srcPoints,
		dstPoints,
		CV_FM_RANSAC,
		reprojectionThreshold,
		inliersMask);

	std::vector<cv::DMatch> inliers;
	for (size_t i = 0; i<inliersMask.size(); i++)
	{
		if (inliersMask[i])
			inliers.push_back(matches[i]);
	}

	matches.swap(inliers);//交换
	return matches.size() > minNumberMatchesAllowed;
}
bool PatternDetector::findPattern(const cv::Mat& image, PatternTrackingInfo& info)
{
	//转换输入图像到灰度  
	getGray(image, m_grayImg);
	//从灰度图像中提取特征点  
	extractFeatures(m_grayImg, m_queryKeypoints,
		m_queryDescriptors);
	//使用当前模式获取匹配  
	getMatches(m_queryDescriptors, m_matches);
	//寻找单应性变换和检测好的匹配  
	bool homographyFound = refineMatchesWithHomography(
		m_queryKeypoints,
		m_pattern.keypoints,
		homographyReprojectionThreshold,
		m_matches,
		m_roughHomography);
	if (homographyFound)
	{
		//如果启用单应性提纯，则改善找到的单应性变换  

		if (enableHomographyRefinement)
		{
			//使用找到的单应性转换图像 透视变换（变换后是图像）
			cv::warpPerspective(m_grayImg, m_warpedImg, m_roughHomography, m_pattern.size, cv::WARP_INVERSE_MAP | cv::INTER_CUBIC);
			// Get refined matches获取提纯的匹配  
			std::vector<cv::KeyPoint> warpedKeypoints;
			std::vector<cv::DMatch> refinedMatches;
			//在转换后的图像上检测特征  
			extractFeatures(m_warpedImg, warpedKeypoints, m_queryDescriptors);
			// Match with pattern使用模式进行匹配  
			getMatches(m_queryDescriptors, refinedMatches);
			// Estimate new refinement homography估计新的提纯单应性  
			homographyFound = refineMatchesWithHomography(warpedKeypoints, m_pattern.keypoints, homographyReprojectionThreshold, refinedMatches, m_refinedHomography);
			//得到一个结果单应性作为提纯和粗糙单应性矩阵乘积的结果。  
			info.homography = m_roughHomography * m_refinedHomography;
			// Transform contour with precise homography使用精确的单应性转换轮廓（点） 输出info.points2d 经过变换后的2d点
			cv::perspectiveTransform(m_pattern.points2d,
				info.points2d, info.homography);
		}
		else
		{
			info.homography = m_roughHomography;
			//使用粗略的单应性转换轮廓  输出info.points2d 经过变换后的2d点
			cv::perspectiveTransform(m_pattern.points2d, info.points2d, m_roughHomography);
		}
	}
	//匹配大于25 返回真
	//if (m_matches.size() >25)
	//{
	//	return true;
	//}
	//else{
	//	return false;
	//}

	
	return homographyFound;
}