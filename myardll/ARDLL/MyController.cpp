#include "MyController.hpp"
#include"ARPipeline.hpp"
#include <fstream> 
MyController::MyController()
{
	camera = std::make_shared<CCamera>();



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
	
	bool temp = false;

	temp = camera->GetCameraFrame(frame);

	cv::resize(frame, result, Size(320, 240), 0, 0, CV_INTER_LINEAR);

	cv::cvtColor(result, out, COLOR_BGR2BGRA);

	
	return temp;
}

bool MyController::get_camera_size(int *width, int *height)
{
	return camera->GetCameraSize(width, height);
}



bool MyController::found_texture()
{

}

bool MyController::getPatternLocation(char *filepath,char*rtpath)
{
	//string file_path = filepath;
	    //camera->GetCameraFrame(frame);
	if (filepath != "")
	{

		Image = imread(filepath);
		CameraCalibration calibration(632.15129446869992f, 632.32473457467449f, 299.98892234216760f, 229.19581451681279f);
		ARPipeline pipeline(Image, calibration);
		bool find = pipeline.processFrame(frame);

		Matrix44 m44 = pipeline.getPatternLocation().getMat44();

		ofstream myfile(rtpath, ios::ate | ios::out);

		if (myfile)
		{
			for (int col = 0; col < 4; col++)
			{
				for (int row = 0; row < 3; row++)
				{

					myfile << m44.mat[col][row] << " ";
				}
				myfile << endl;


			}
			myfile.close();

		}


		return find;
	}
	if (filepath = "")
	{
		return 0;
	}
	
}


