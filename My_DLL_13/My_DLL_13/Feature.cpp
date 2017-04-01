#include "stdafx.h"
#include "Feature.h"


CFeature::CFeature()
{
	m_detectType = "ORB";
	m_extractType= "ORB";
	m_matchType = "BruteForce";

	//m_detector = new ORB(1000);
	//m_extractor = new FREAK(true, false);
	//m_matcher = new BFMatcher(cv::NORM_HAMMING,true);

}


CFeature::~CFeature()
{

}

void CFeature::detectKeypoints(const cv::Mat& image, vector<KeyPoint>& keypoints)
{
	assert(image.type() == CV_8UC1);
	assert(!m_detectType.empty());

	keypoints.clear();
	m_detector = FeatureDetector::create(m_detectType);

   // m_detector = new ORB(1000);
	m_detector->detect(image, keypoints);
}


void CFeature::extractDescriptors(const cv::Mat& image, vector<KeyPoint>& keypoints, cv::Mat& descriptors)
{
	assert(image.type() == CV_8UC1);
	assert(!m_extractor.empty());
	
	m_extractor = DescriptorExtractor::create(m_extractType);
	//m_extractor = new FREAK(true, false);
	m_extractor->compute(image, keypoints, descriptors);
}

bool CFeature::bestMatch(const cv::Mat& queryDescriptor, cv::Mat& trainDescriptor, vector<DMatch>& matches)
{
	assert(!queryDescriptor.empty());
	assert(!trainDescriptor.empty());
	assert(!m_matcher.empty());

	matches.clear();

	m_matcher = DescriptorMatcher::create(m_matchType);
	//m_matcher = new BFMatcher(cv::NORM_HAMMING,true);
	//m_matcher = new FlannBasedMatcher();
	m_matcher->add(std::vector<Mat>(1, queryDescriptor));
	m_matcher->train();
	m_matcher->match(trainDescriptor, matches);

	if (matches.empty())
		return false;

	return true;
}

void CFeature::knnMatch(const cv::Mat& queryDesriptor, cv::Mat& trainDescriptor, vector<vector<DMatch>>& matches, int k)
{
	assert(k > 0);
	assert(!queryDesriptor.empty());
	assert(!trainDescriptor.empty());
	assert(!m_matchType.empty);

	matches.clear();

	m_matcher = DescriptorMatcher::create(m_matchType);
	m_matcher->add(std::vector<Mat> (1,trainDescriptor));
	m_matcher->train();
	m_matcher->knnMatch(queryDesriptor,matches,k);
}

bool CFeature::homography(const vector<KeyPoint> & F_keypoints, const vector<KeyPoint>& I_keypoints, cv::Mat& homography)
{
	/*if (matches.size() < 25)
	{
		return ;
	}
	vector<Point2f> src();*/
}