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

private:
	static LogLevel level;

public:
	static LogLevel getLevel() { return level; }
	static void setLevel(LogLevel logLevel){ level = logLevel; }

	static int info(const char* format, ...);
	static int debug(const char* format, ...);
	static int warning(const char* format, ...);
	static int error(const char* format, ...);
};