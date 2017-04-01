#ifndef __CVUNITY_H__
#define __CVUNITY_H__

#ifdef CVUNITY_EXPORTS
#define _DLLExport __declspec(dllexport)
#else
#define _DLLExport __declspec(dllexport)
#endif

extern "C"
{
	_DLLExport int Imagerows();
	//_DLLExport bool test_update_camera_frame(unsigned int *buffer);
	_DLLExport bool update_camera_frame(unsigned int *buffer);
	_DLLExport bool get_camera_size(int *width, int *height);
	_DLLExport bool open_camera();
	_DLLExport bool close_camera();
	_DLLExport int count();
	_DLLExport bool get_projection(float *m, float width, float height, float near_plane,float far_plane);
	_DLLExport bool get_texture_filepath(char *filepath);

	_DLLExport void cameraframe_detekeypoints();
	_DLLExport bool best_Match();
	_DLLExport int Homography();
}

#endif