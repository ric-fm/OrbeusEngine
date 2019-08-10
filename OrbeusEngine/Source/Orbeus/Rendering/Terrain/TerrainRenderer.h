#pragma once

class Shader;
class Camera;

class TerrainRenderer
{
private:
	Shader* shader;
public:
	TerrainRenderer();
	~TerrainRenderer();

	void render(Camera* camera);
};
