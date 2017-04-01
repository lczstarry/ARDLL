#pragma once

enum LOG_LEVEL
{
	LOG_ERROR,
	LOG_WARNING,
	LOG_INFO
};

/**********************************************************************************************//**
* @typedef	void(__stdcall *LOGCALLBACK)(int level, const char* msg)
*
* @brief	Defines an alias representing the log callback.
**************************************************************************************************/

typedef void(__stdcall *LOGCALLBACK)(int level, const char* msg);

/**********************************************************************************************//**
* @typedef	int(__stdcall *CAMERA_SELECTOR)()
*
* @brief	Defines an alias representing the *camera selector.
**************************************************************************************************/

typedef int(__stdcall *CAMERA_SELECTOR)();
