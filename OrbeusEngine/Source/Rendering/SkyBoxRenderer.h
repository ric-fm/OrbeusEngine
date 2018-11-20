#pragma once

class CubeMap;
class Shader;

class SkyBoxRenderer
{
private:
	CubeMap* cubeMap;
	Shader* shader;
public:
	SkyBoxRenderer();
	~SkyBoxRenderer();
	void render();

};
