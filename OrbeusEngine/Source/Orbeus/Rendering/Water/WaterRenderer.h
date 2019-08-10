#pragma once

class WaterRenderer
{
private:
	float waveSpeed = 0.03f;
	float moveFactor = 0.0f;
public:

	void render(float deltaTime);
};
