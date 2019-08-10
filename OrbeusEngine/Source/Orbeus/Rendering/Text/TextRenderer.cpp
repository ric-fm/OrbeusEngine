#include "TextRenderer.h"

#include <vector>
#include <GL/glew.h>

#include "Orbeus/Rendering/Texture.h"
#include "Orbeus/Rendering/Shader.h"
#include "Orbeus/Rendering/VertexArray.h"
#include "Orbeus/Rendering/Text/TextMeshData.h"
#include "Orbeus/Rendering/Text/Font.h"

#include "Orbeus/Math/Vector2.h"

#include "Orbeus/Components\GUIText.h"
#include "Orbeus/Core\Transform.h"
#include "Orbeus/Core\World.h"
#include "Orbeus/ResourceManagement/ShaderLoader.h"


TextRenderer::TextRenderer()
{
	shader = ShaderLoader::loadShader("Resources/Shaders/Text/SDF-vs.shader", "Resources/Shaders/Text/SDF-fs.shader");
}

TextRenderer::~TextRenderer()
{
	if (shader != nullptr)
	{
		delete shader;
	}
}

void TextRenderer::render()
{
	std::vector<GUIText*> texts = World::getInstance().getComponents<GUIText>();

	if (texts.size() > 0)
	{
		// OpenGL setup for text rendering
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		
		shader->bind();
		shader->SetInt("atlas", 0);

		for (unsigned int i = 0; i < texts.size(); ++i)
		{
			GUIText* text = texts[i];

			Vector2 translation(text->getTransform()->getPosition().x, text->getTransform()->getPosition().y);
			shader->SetFloat2("translation", translation);
			shader->SetFloat3("color", text->getColor());

			text->getFont()->getTextureAtlas()->bind();

			TextMeshData* meshData = text->getMeshData();
			
			meshData->vertexArray->bind();
			meshData->vertexArray->draw(shader);

			meshData->vertexArray->unbind();
		}

		shader->unbind();

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}
