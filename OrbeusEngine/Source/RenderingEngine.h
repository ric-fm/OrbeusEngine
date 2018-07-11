#pragma once

#include <vector>

#include "Vector3.h"

#include "Light.h"

class Shader;
class Engine;

class RenderingEngine
{
private:
	Engine* engine;
	Shader* ambientShader;

	Vector3 ambientLight;

	std::vector<Light*> lights;

public:
	RenderingEngine(Engine* engine);

	void registerLight(Light* light);

	void init();
	void render(float deltaTime);

	Vector3& getAmbientLight() { return ambientLight; }
	void setAmbienLight(const Vector3& ambientLight) { this->ambientLight = ambientLight; }
};