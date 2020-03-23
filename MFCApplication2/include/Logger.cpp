#include "stdafx.h"
#include "Logger.h"

static const int LOGGER_FILENAME_MAX              = 256;
static char g_LoggerFileName[LOGGER_FILENAME_MAX] = "lesson.log";

void LoggerCreate(const char *fileName)
{
	FILE *output;
	errno_t err;

	memset(g_LoggerFileName, 0, LOGGER_FILENAME_MAX);
	strncpy_s(g_LoggerFileName, fileName, LOGGER_FILENAME_MAX - 1);

	if (( fopen_s(&output, g_LoggerFileName, "w")) != NULL)
		fclose(output);
}

void LoggerDestroy()
{
	//
}

void LoggerWrite(const char *format, ...)
{
	va_list ap;
	FILE    *output;
	errno_t err;

	err = fopen_s(&output, g_LoggerFileName, "a+");
	if (0 != err)
		return;

	va_start(ap, format);
	vfprintf(output, format, ap);
	va_end(ap);

	fclose(output);
}
