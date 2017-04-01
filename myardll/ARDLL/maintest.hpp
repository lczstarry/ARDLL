#if defined(EXPORTBUILD)
#define _DLLExport __declspec(dllexport)
#else
#define _DLLExport __declspec(dllimport);
#endif
using namespace cv;
#include "ARPipeline.hpp"
#include <opencv2/opencv.hpp>
#include <fstream> 

extern "C" bool _DLLExport  ARPipeline_getPatternLocation();

extern "C" bool _DLLExport open_camera();
extern "C" bool _DLLExport open_camera1();
extern "C" int  _DLLExport  Imagerows();
extern "C" bool _DLLExport get_camera_size(int *width, int *height);
extern "C" bool _DLLExport update_camera_frame(unsigned int *buffer);
extern "C" int _DLLExport count();
extern "C" bool _DLLExport close_camera();
extern "C" bool _DLLExport get_projection(float *m, float width, float height, float near_plane, float far_plane);
extern "C" bool _DLLExport getPatternLocation(char *filepath, char*rtpath);


