#pragma once


struct PerformanceInfo
{
	float MS;
	int FPS;
	float deltaTime;
};

class VisualLogger
{
public:
	static VisualLogger& getInstance()
	{
		static VisualLogger instance;

		return instance;
	}
private:
	VisualLogger();

public:
	VisualLogger(VisualLogger const&) = delete;
	void operator=(VisualLogger const&) = delete;

private:
	PerformanceInfo performanceInfo;
	bool showPerformanceInfo;

public:
	void setShowPerformanceInfo(bool show) { showPerformanceInfo = show; }
	void init();
	void destroy();
	void update(float deltaTime);
	void render();
};
