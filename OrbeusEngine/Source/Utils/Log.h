#pragma once


class Log
{
public:
	enum LogLevel
	{
		NONE = 0,
		INFO = 1,
		DEBUG = 2,
		WARNING = 3,
		ERROR = 4
	};

	static LogLevel getLevel();
	static void setLevel(LogLevel logLevel);

	static int info(const char* format, ...);
	static int debug(const char* format, ...);
	static int warning(const char* format, ...);
	static int error(const char* format, ...);
};