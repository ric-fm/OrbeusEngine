#include "VisualLogger.h"

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "Core\Engine.h"
#include "Window\Window.h"

#include "Core\Input.h"

VisualLogger::VisualLogger()
	: showPerformanceInfo(true)
{
}

void VisualLogger::init()
{
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(Engine::getInstance().getWindow()->getHandler(), true);
	ImGui::StyleColorsDark();
}

void VisualLogger::destroy()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}

void VisualLogger::update(float deltaTime)
{
	static float acumDeltaTime = 0.0f;
	static float frameCount = 0.0f;

	++frameCount;
	acumDeltaTime += deltaTime;
	if (acumDeltaTime > 1.0f)
	{
		performanceInfo.FPS = frameCount;
		performanceInfo.MS = (1.0f / performanceInfo.FPS) * 1000;

		frameCount = 0;
		acumDeltaTime = 0.0f;
	}

	static bool lastF1KeyDown = false;
	bool f1KeyDown = Input::isKeyDown(GLFW_KEY_F1);
	if (f1KeyDown && !lastF1KeyDown)
	{
		showPerformanceInfo = !showPerformanceInfo;
	}
	lastF1KeyDown = f1KeyDown;
}

void VisualLogger::render()
{
	ImGui_ImplGlfwGL3_NewFrame();

	if (showPerformanceInfo)
	{
		ImGui::SetNextWindowPos(ImVec2(24, 24), ImGuiCond_Always, ImVec2(0,0));
		ImGui::SetNextWindowBgAlpha(0.1f);
		if (ImGui::Begin("Frame Info", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::Text("%d FPS", performanceInfo.FPS);
			ImGui::Text("%.2f MS", performanceInfo.MS);
			ImGui::Separator();
			ImGui::End();
		}
	}

	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}
