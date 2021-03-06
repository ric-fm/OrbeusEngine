#pragma once

#include <vector>

class Shader;
class Engine;

class MeshRenderer;
class SkeletalMeshRenderer;
class TextRenderer;
class TerrainRenderer;
class SkyBoxRenderer;
class WaterRenderer;
class Camera;

class RenderingEngine
{
private:
	Engine* engine;

	MeshRenderer* meshRenderer;
	SkeletalMeshRenderer* skeletalMeshRenderer;
	TextRenderer* textRenderer;
	TerrainRenderer* terrainRenderer;
	SkyBoxRenderer* skyBoxRenderer;
	WaterRenderer* waterRenderer;

public:
	RenderingEngine(Engine* engine);
	~RenderingEngine();

	void init();
	void renderScene(Camera* camera);
	void render(float deltaTime);

	void setBlendingEnabled(bool enabled);

	MeshRenderer* getMeshRenderer() const { return meshRenderer; }
	SkeletalMeshRenderer* getSkeletalMeshRenderer() const { return skeletalMeshRenderer; }
	TextRenderer* getTextRenderer() const { return textRenderer; }
	TerrainRenderer* getTerrainRenderer() const { return terrainRenderer; }
	SkyBoxRenderer* getSkyBoxRenderer() const { return skyBoxRenderer; }
};
