#include "Log.h"


#include <stdio.h>
#include <stdarg.h>

#define LOG_FORMAT "[%s]: %s\n"		// [LOG_LEVEL]: format\n

int log(const char* level, const char* format, va_list args)
{
	int length = snprintf(NULL, 0, LOG_FORMAT, level, format) + 1;
	char* logFormat = new char[length];
	length = snprintf(logFormat, length, LOG_FORMAT, level, format);

	vprintf(logFormat, args);

	delete(logFormat);

	return length;
}


Log::LogLevel Log::level = Log::ERROR;

int Log::info(const char * format, ...)
{
	if (Log::level < Log::LogLevel::INFO)
	{
		return -1;
	}

	va_list args;
	va_start(args, format);
	int result =  log("INFO", format, args);
	va_end(args);
	
	return result;
}

int Log::debug(const char * format, ...)
{
	if (Log::level < Log::LogLevel::DEBUG)
	{
		return -1;
	}

	va_list args;
	va_start(args, format);
	int result = log("DEBUG", format, args);
	va_end(args);

	return result;
}

int Log::warning(const char * format, ...)
{
	if (Log::level < Log::LogLevel::WARNING)
	{
		return -1;
	}

	va_list args;
	va_start(args, format);
	int result = log("WARNING", format, args);
	va_end(args);

	return result;
}

int Log::error(const char * format, ...)
{
	if (Log::level < Log::LogLevel::ERROR)
	{
		return -1;
	}

	va_list args;
	va_start(args, format);
	int result = log("ERROR", format, args);
	va_end(args);

	return result;
}
