#pragma once

class Shader;

class TerrainRenderer
{
private:
	Shader* shader;
public:
	TerrainRenderer();
	~TerrainRenderer();

	void render();
};
