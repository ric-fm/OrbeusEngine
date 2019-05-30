#include "TimeMeasure.h"
#include "Utils/Log.h"

void TimeMeasure::start(const std::string& name)
{
	std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
	
	if (times.find(name) != times.end())
	{
		Log::warning("[TimeMeasure]: %s exists. It will be overwritten");
	}
	TM tm;
	tm.start = t;
	tm.state = TIME_MEASURE_STATE_STARTED;
	times[name] = tm;
}

void TimeMeasure::end(const std::string & name)
{
	std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
	if (times.find(name) == times.end())
	{
		Log::warning("[TimeMeasure]: %s does not exists. You need to start first");
	}
	TM& tm = times[name];
	if (tm.state != TIME_MEASURE_STATE_STARTED)
	{
		tm.state = TIME_MEASURE_STATE_ERROR;
		Log::error("[TimeMeasure]: %s does not have started");
		return;
	}
	tm.end = t;
	tm.state = TIME_MEASURE_STATE_COMPLETED;
}

int TimeMeasure::getMilliseconds(const std::string & name)
{
	if (times.find(name) != times.end())
	{
		TM& tm = times[name];
		if (tm.state == TIME_MEASURE_STATE_COMPLETED)
		{
			auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(tm.end - tm.start).count();
			return (int)dt;
		}
	}
	Log::error("[TimeMeasure]: %s does not have start or end");
	return -1;
}

void TimeMeasure::clear(const std::string & name)
{
	if (times.find(name) != times.end())
	{
		times.erase(name);
	}
}

void TimeMeasure::clearAll()
{
	times.clear();
}
