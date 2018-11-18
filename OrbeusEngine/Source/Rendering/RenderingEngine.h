#pragma once

#include <vector>

#include "Math/Vector3.h"

#include "Components/Light.h"

class Shader;
class Engine;

class MeshRenderer;
class TextRenderer;
class TerrainRenderer;

class RenderingEngine
{
private:
	Engine* engine;
	Shader* ambientShader;

	std::vector<Light*> lights;

	MeshRenderer* meshRenderer;
	TextRenderer* textRenderer;
	TerrainRenderer* terrainRenderer;

public:
	RenderingEngine(Engine* engine);
	~RenderingEngine();

	void registerLight(Light* light);

	void init();
	void render(float deltaTime);

	MeshRenderer* getMeshRenderer() const { return meshRenderer; }
	TextRenderer* getTextRenderer() const { return textRenderer; }
	TerrainRenderer* getTerrainRenderer() const { return terrainRenderer; }
};