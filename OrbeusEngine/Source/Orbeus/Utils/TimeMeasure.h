#pragma once

#include <string>
#include <unordered_map>
#include <chrono>

const int TIME_MEASURE_STATE_ERROR = -1;
const int TIME_MEASURE_STATE_STARTED = 0;
const int TIME_MEASURE_STATE_COMPLETED = 1;

class TimeMeasure
{
	struct TM
	{
		std::chrono::steady_clock::time_point start;
		std::chrono::steady_clock::time_point end;
		int state;
	};
private:
	std::unordered_map<std::string, TM> times;

public:
	void start(const std::string& name);
	void end(const std::string& name);
	int getMilliseconds(const std::string& name);
	void clear(const std::string& name);
	void clearAll();
};
