#pragma once

class Shader;
class Engine;

class RenderingEngine
{
private:
	Engine* engine;
	Shader* shader;

public:
	RenderingEngine(Engine* engine);

	Shader* getShader() const { return shader; }
	void setShader(Shader* shader) { this->shader = shader; }

	void init();
	void render(float deltaTime);
};