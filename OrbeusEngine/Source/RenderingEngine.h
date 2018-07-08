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
	Shader* shader;

	Vector3 ambientLight;

	DirectionalLight directionalLight;
	std::vector<PointLight> pointLights;

public:
	RenderingEngine(Engine* engine);

	Shader* getShader() const { return shader; }
	void setShader(Shader* shader) { this->shader = shader; }

	void init();
	void render(float deltaTime);

	Vector3& getAmbientLight() { return ambientLight; }
	void setAmbienLight(const Vector3& ambientLight) { this->ambientLight = ambientLight; }

	DirectionalLight& getDirectionalLight() { return directionalLight; }
	void setDirectionalLight(const DirectionalLight& directionalLight) { this->directionalLight = directionalLight; }
};