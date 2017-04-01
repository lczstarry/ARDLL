#include "stdafx.h"
#include "Logger.h"

#include <cstdarg>
#include <cstdio>
#include <stdio.h>

LOGCALLBACK CLogger::gLoggerFunc = nullptr;

CLogger::CLogger()
{
}


CLogger::~CLogger()
{
}

void CLogger::log_format(LOG_LEVEL level, const char * fmt, ...)
{
	if (!gLoggerFunc)
		return;

	char buffer[512];
	int ret;
	va_list args;

	va_start(args, fmt);
	ret = vsprintf_s(buffer, fmt, args);
	va_end(args);
	gLoggerFunc(level, buffer);
}
