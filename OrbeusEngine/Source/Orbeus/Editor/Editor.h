#pragma once

//#include "GL/glew.h"
//#include <GLFW/glfw3.h>
//
#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw_gl3.h"

//#include "Core/Engine.h"
//#include "Window/Window.h"
//
//#include "Core/Input.h"
//
//#include "Core/Transform.h"
//#include "Core/World.h"
//#include "Components/Camera.h"

class Editor
{
public:
	virtual void init() = 0;
	virtual void destroy() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() = 0;
};
