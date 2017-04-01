#include "stdafx.h"
#include "MyController.h"

MyController::MyController()
{
	camera = std::make_shared<CCamera>();
	file_path = "";
	//feature = new CFeature();
	feature = std::make_shared<CFeature>();

}


MyController::~MyController()
{

}

bool MyController::start_camera()
{
	return camera->OpenCamera();
}

bool MyController::stop_camera()
{
	return camera->CloseCamera();
}

bool MyController::read_frame(unsigned int *buffer)
{
	cv::Mat out(cv::Size(320, 240), CV_8UC4, buffer); //CV_8UC4 8-bit unsigned integers ( 0..255 ) C4代表四个通道(即 r、g、b、a)
	cv::Mat result;
	//imshow("当前图片",out);
	//Mat img = imread("D:/OpenCV/OpencvSharp/OpenCvSharp-2.4.10-x64-20161106/My_CS_OpenCV/RZ.jpg");
	//cv::Mat result;

	bool temp = false;
	
	temp = camera->GetCameraFrame(frame);

	cv::resize(frame, result, Size(320, 240), 0, 0, CV_INTER_LINEAR);
	//cv::cvtColor(result, out, CV_8UC4);
	
	cv::cvtColor(result, out, COLOR_BGR2BGRA);

	//imshow("当前视频", frame);
	//cv::Mat dealframe;
	//result.copyTo(dealframe);
	//string msg = "Calibration";
	//int baseLine = 0;
	////返回一个包含文字的方框的大小
	//Size textSize = getTextSize(msg, 1, 1, 1, &baseLine);
	////格式标准
	//msg = format("%d Index", buffer);
	////图片起点的位置
	//Point text(dealframe.cols - 2 * textSize.width - 10, dealframe.rows - 2 * baseLine - 10);
	////将文字显示在图片
	//putText(dealframe, msg, text, 1, 1, Scalar(0, 0, 255));
	//imshow("Chessboard detect ", dealframe);
	////imshow("当前视频2",out);

	return temp;
}

bool MyController::get_camera_size(int *width, int *height)
{
	return camera->GetCameraSize(width,height);
}

//bool get_projection(float *m, float width, float height, float near_plane, float far_plane)
//{
//
//}

bool MyController::found_texture()
{

}

void MyController::get_texture_filepath_detectkeypoint(const string str)
{
	file_path = str;
	
	if (file_path != "")
	{
		Image = imread(file_path); //得到灰度图
		cv::cvtColor(Image, Image, COLOR_BGRA2GRAY);
		imshow("src", Image);

		if (Image.empty())
		{
			return;
		}

		feature->detectKeypoints(Image,querKeypoints);

		feature->extractDescriptors(Image, querKeypoints,queryDescriptor);		
	}
}

void MyController::cameraframe_detekeypoints()
{
    cv::cvtColor(frame, frame_GARY, COLOR_BGRA2GRAY);//将视频影像转换为灰度图
	
	if (!frame_GARY.empty())
	{
		feature->detectKeypoints(frame_GARY, trainKeyPoints);	
		feature->extractDescriptors(frame_GARY, trainKeyPoints, trainDescriptor);
	}
	
	cv::Mat outImage;
	cv::drawKeypoints(frame_GARY, trainKeyPoints, outImage, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	if (!outImage.empty())
	{	
		cv::imshow("当前视频画面", outImage);
	}
}

bool MyController::best_Match()
{
	//bool mark = false;
	mark = feature->bestMatch(queryDescriptor,trainDescriptor, matches);

	if (mark)
	{
		//Mat imageMatches;
		//
		//if(!(Image.empty() || querKeypoints.empty() || frame_GARY.empty() || trainKeyPoints.empty()))
		//cv::drawMatches(frame_GARY, trainKeyPoints, Image, querKeypoints, matches, imageMatches,
		//	Scalar(255, 0, 0), Scalar(0, 255, 255));//vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS

		//if(!imageMatches.empty())
		//cv::imshow("match", imageMatches);
		//return mark;
	}

	else
	{
		mark = false;
		return mark;
	}
	
}

int MyController::Homography()
{
	if (mark)
	{
		cv::Mat homography;
		
		//if (matches.size() > 20 && matches.size() < 1000)
		//{
		//	double max_dist = 0;
		//	double min_dist = 100;

		//	for (int i = 0; i < queryDescriptor.rows; i++)
		//	{
		//		double dist = matches[i].distance;
		//		if (dist < min_dist)
		//			min_dist = dist;
		//		if (dist > max_dist)
		//			max_dist = dist;
		//	}

		//	std::vector<DMatch> good_matches;

		//	for (int i = 0; i < queryDescriptor.rows; i++)
		//	{
		//		if (matches[i].distance < 3 * min_dist)
		//		{
		//			good_matches.push_back(matches[i]);
		//		}
		//	}

		//	Mat img_matches;
		//	drawMatches(Image, querKeypoints, frame_GARY, trainKeyPoints,
		//		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		//		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

		//	//Mat img_matches;
		//	std::vector<Point2f> obj;
		//	std::vector<Point2f> scence;

		//	if (good_matches.size() > 20 && good_matches.size() < 1000)
		//	{

		//		for (size_t i = 0; i < good_matches.size(); i++)
		//		{
		//			obj.push_back(querKeypoints[good_matches[i].queryIdx].pt);
		//			scence.push_back(trainKeyPoints[good_matches[i].trainIdx].pt);
		//		}

		//		homography = findHomography(obj, scence, CV_RANSAC);

		//		std::vector<Point2f> obj_corners(4);

		//		obj_corners[0] = cvPoint(0, 0);
		//		obj_corners[1] = cvPoint(Image.cols, 0);
		//		obj_corners[2] = cvPoint(Image.cols, Image.rows);
		//		obj_corners[3] = cvPoint(0, Image.rows);

		//		std::vector<Point2f> scence_corners(4);

		//		if(!homography.empty())
		//		perspectiveTransform(obj_corners, scence_corners, homography);

		//		line(img_matches, scence_corners[0] + Point2f(Image.cols, 0), scence_corners[1] + Point2f(Image.cols, 0), Scalar(0, 255, 0), 4);
		//		line(img_matches, scence_corners[1] + Point2f(Image.cols, 0), scence_corners[2] + Point2f(Image.cols, 0), Scalar(0, 255, 0), 4);
		//		line(img_matches, scence_corners[2] + Point2f(Image.cols, 0), scence_corners[3] + Point2f(Image.cols, 0), Scalar(0, 255, 0), 4);
		//		line(img_matches, scence_corners[3] + Point2f(Image.cols, 0), scence_corners[0] + Point2f(Image.cols, 0), Scalar(0, 255, 0), 4);

		//		if(!img_matches.empty())
		//		imshow("match", img_matches);

		//		return (int)good_matches.size();
		//	}
		//	else
		//	{
		//		return 0;
		//	}

		if (matches.size() > 20 && matches.size() < 1000)
		{
			vector<Point2f> src(matches.size());
			vector<Point2f> dst(matches.size());

			for (int i = 0; i < matches.size(); i++)
			{
				src[i] = querKeypoints[matches[i].queryIdx].pt;
				dst[i] = trainKeyPoints[matches[i].trainIdx].pt;
			}

			vector <unsigned char> inliers(matches.size());

			//double min, max;
			//cv::minMaxIdx(matches, &min, &max);
			//cv::solvePnPRansac(ppcloud, imgPoints, m_cal.K, m_cal.distCoeffs, rvec, t, true, 100, 0.006 * max, 0.25 * (double)(imgPoints.size()), inliers, CV_EPNP);

			homography = findHomography(src, dst, CV_FM_RANSAC, 3, inliers);

			if (!homography.empty())
			{
				vector<DMatch> inlierMatcher;

				if (!inliers.empty())
				{
					for (int i = 0; i < inliers.size(); i++)
					{
						if (inliers[i])
							inlierMatcher.push_back(matches[i]);
					}
				}
				if (!inlierMatcher.empty())
					matches.swap(inlierMatcher);
			}

			Mat imageMatches;

			if (!matches.empty())
				cv::drawMatches(frame_GARY, trainKeyPoints, Image, querKeypoints, matches, imageMatches,
					Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);//vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS		
			if (matches.size() > 20 && matches.size() < 1000)
			{
				if (!imageMatches.empty())
					imshow("match", imageMatches);

				return (int)matches.size();
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		return 0;
	}
}



