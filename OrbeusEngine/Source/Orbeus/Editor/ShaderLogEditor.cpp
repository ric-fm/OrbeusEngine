#include "ShaderLogEditor.h"

#include "Orbeus/Rendering/Shader.h"
#include "Orbeus/ResourceManagement/ShaderLoader.h"

void ShaderLogEditor::init()
{
	shader = ShaderLoader::loadShader("Resources/Shaders/Mesh-vs.shader", "Resources/Shaders/Mesh-fs.shader");
}

void ShaderLogEditor::destroy()
{
}

void ShaderLogEditor::update(float deltaTime)
{
}


void ShaderLogEditor::render()
{
	ImGui::SetNextWindowPos(ImVec2(400, 20), ImGuiCond_Always, ImVec2(0, 0));
	ImGui::SetNextWindowBgAlpha(0.1f);
	if (ImGui::Begin("Shader Info", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		ImGui::Text("Shader: %s", "Mesh");
		ImGui::Separator();

		const std::unordered_map<std::string, Uniform>& uniforms = shader->getUniforms();

		for (auto& it : uniforms)
		{
			std::string label = GLDataTypeToString(it.second.type) + ": " + it.second.name;
			ImGui::Text( label.c_str());
		}
	}
	ImGui::End();
}
