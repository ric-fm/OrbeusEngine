#include "WaterRenderer.h"

#include <vector>
#include <math.h>

#include "Orbeus/Components/WaterPlane.h"
#include "Orbeus/Rendering/Shader.h"
#include "Orbeus/Rendering/Quad.h"
#include "Orbeus/Core/Transform.h"
#include "Orbeus/Core/World.h"
#include "Orbeus/Core/Engine.h"
#include "Orbeus/Components/Camera.h"
#include "Orbeus/Components/Light.h"

#include "Orbeus/Rendering/RenderingEngine.h"
#include "Orbeus/Rendering/Texture.h"


void WaterRenderer::render(float deltaTime)
{
	std::vector<WaterPlane*> waters = World::getInstance().getComponents<WaterPlane>();
	std::vector<DirectionalLight*> directionalLights = World::getInstance().getComponents<DirectionalLight>();

	if (waters.size() > 0)
	{
		Engine::getInstance().getRenderingEngine()->setBlendingEnabled(true);

		for (unsigned int i = 0; i < waters.size(); ++i)
		{
			WaterPlane* water = waters[i];
			Quad* quad = water->quad;
			quad->shader->bind();
			quad->shader->SetMatrix("view", World::getInstance().getActiveCamera()->getViewMatrix());
			quad->shader->SetMatrix("projection", World::getInstance().getActiveCamera()->getProjectionMatrix());
			quad->shader->SetMatrix("model", water->getTransform()->getMatrix());
			quad->shader->SetFloat3("cameraPos", World::getInstance().getActiveCamera()->getTransform()->getPosition());
			quad->shader->SetInt("reflectionTexture", 0);
			quad->shader->SetInt("refractionTexture", 1);
			quad->shader->SetInt("depthMap", 2);
			quad->shader->SetInt("normalMap", 3);
			quad->shader->SetInt("dudvMap", 4);

			// Wave movement effect
			// TODO: Remove from WaterRenderer
			moveFactor += waveSpeed * deltaTime;
			moveFactor = fmod(moveFactor, 1.0f);
			quad->shader->SetFloat("moveFactor", moveFactor);

			if (directionalLights.size() > 0)
			{
				DirectionalLight* directionalLight = directionalLights[0];
				quad->shader->SetFloat3("lightPosition", directionalLight->getTransform()->getPosition());
				quad->shader->SetFloat3("ligthColor", directionalLight->getColor());
			}

			water->getReflectionTexture()->bind(0);
			water->getRefractionTexture()->bind(1);
			water->getRefractionDepthTexture()->bind(2);
			water->getNormalMap()->bind(3);
			water->getDudvMap()->bind(4);

			quad->bind();
			quad->draw();
			quad->shader->unbind();
			quad->unbind();
			water->getReflectionTexture()->unbind();
		}
		Engine::getInstance().getRenderingEngine()->setBlendingEnabled(false);
	}
}
