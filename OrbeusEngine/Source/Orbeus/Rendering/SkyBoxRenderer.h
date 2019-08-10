#pragma once

class CubeMap;
class Shader;
class Camera;

class SkyBoxRenderer
{
private:
	CubeMap* cubeMap;
	Shader* shader;
public:
	SkyBoxRenderer();
	~SkyBoxRenderer();
	void render(Camera* camera);

};
