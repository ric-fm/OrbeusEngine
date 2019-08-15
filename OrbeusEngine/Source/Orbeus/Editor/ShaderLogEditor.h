#pragma once

#pragma once

#include "Orbeus/Editor/Editor.h"

class Shader;

class ShaderLogEditor : public Editor
{
private:
	Shader* shader;

public:
	virtual void init() override;
	virtual void destroy() override;
	virtual void update(float deltaTime) override;
	virtual void render() override;
};