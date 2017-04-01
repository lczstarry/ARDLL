
#include "Camera.hpp"

#include <atlbase.h>
#include <dshow.h>

#pragma comment(lib,"Strmiids.lib") 


CCamera::CCamera()
{
	video_capture;
	is_opencamera = false;
	c_width = 0;
	c_height = 0;
}


CCamera::~CCamera()
{
}

bool CCamera::OpenCamera()
{
	bool is_open = video_capture.open(0); //打开编号为0的摄像机
	if (is_open)
	{
		is_opencamera = true;
		c_width = 320;  //暂且默认为宽度为320
		c_height = 240; //暂且默认为长度为240
	}
	else
	{
		is_opencamera = false;
	}

	return is_opencamera;
}

bool CCamera::CloseCamera()
{
	//video_capture->release();
	return true;

}

bool CCamera::GetCameraSize(int *width, int *height)
{
	if (!is_opencamera)
	{
		return false;
	}
	else
	{
		*width = c_width;
		*height = c_height;
		return true;
	}

}

bool CCamera::GetCameraFrame(cv::Mat &frame)
{
	if (video_capture.isOpened() && video_capture.read(frame))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int CCamera::count()
{
	int count = 0;
	CoInitialize(NULL);  

	CComPtr<ICreateDevEnum> pCreateDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);

	CComPtr<IEnumMoniker> pEm;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
	if (hr != NOERROR)
	{
		return count;
	}

	pEm->Reset();
	ULONG cFetched;
	IMoniker *pM;
	while (hr = pEm->Next(1, &pM, &cFetched), hr == S_OK)
	{
		count++;
	}

	pCreateDevEnum = NULL;
	pEm = NULL;
	return count;
}

