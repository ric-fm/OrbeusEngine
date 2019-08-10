#pragma once

class Shader;

class TextRenderer
{
private:
	Shader* shader;

public:
	TextRenderer();
	~TextRenderer();

	void render();
};
