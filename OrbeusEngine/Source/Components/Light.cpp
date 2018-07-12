#include "Light.h"

#include "Core/Engine.h"
#include "Rendering/RenderingEngine.h"
#include "Core/Transform.h"
#include "Core/World.h"
#include "Components/Camera.h"

void Light::init()
{
	Engine::getInstance().getRenderingEngine()->registerLight(this);
}

void DirectionalLight::init()
{
	Light::init();
	shader = new Shader("Resources/Shaders/Forward/Directional-vs.shader", "Resources/Shaders/Forward/Directional-fs.shader");
}

void DirectionalLight::updateShader()
{
	shader->bind();
	shader->SetFloat3("viewPos", World::getInstance().getActiveCamera()->getTransform()->getPosition());

	shader->SetFloat3("directionalLight.base.color", color);
	shader->SetFloat("directionalLight.base.intensity", intensity);
	shader->SetFloat3("directionalLight.direction", getTransform()->getForwardVector());
}

void PointLight::init()
{
	Light::init();
	shader = new Shader("Resources/Shaders/Forward/Point-vs.shader", "Resources/Shaders/Forward/Point-fs.shader");
}

void PointLight::updateShader()
{
	shader->bind();
	shader->SetFloat3("viewPos", World::getInstance().getActiveCamera()->getTransform()->getPosition());

	shader->SetFloat3("pointLight.base.color", color);
	shader->SetFloat("pointLight.base.intensity", intensity);
	shader->SetFloat3("pointLight.position", getTransform()->getPosition());
	shader->SetFloat("pointLight.attenuation.constant", attenuation.constant);
	shader->SetFloat("pointLight.attenuation.linear", attenuation.linear);
	shader->SetFloat("pointLight.attenuation.exponential", attenuation.exponential);
	shader->SetFloat("pointLight.radius", radius);
}

void SpotLight::init()
{
	Light::init();
	shader = new Shader("Resources/Shaders/Forward/Spot-vs.shader", "Resources/Shaders/Forward/Spot-fs.shader");
}

void SpotLight::updateShader()
{
	shader->bind();
	shader->SetFloat3("viewPos", World::getInstance().getActiveCamera()->getTransform()->getPosition());

	shader->SetFloat3("spotLight.pointLight.base.color", color);
	shader->SetFloat("spotLight.pointLight.base.intensity", intensity);
	shader->SetFloat3("spotLight.pointLight.position", getTransform()->getPosition());
	shader->SetFloat("spotLight.pointLight.attenuation.constant", attenuation.constant);
	shader->SetFloat("spotLight.pointLight.attenuation.linear", attenuation.linear);
	shader->SetFloat("spotLight.pointLight.attenuation.exponential", attenuation.exponential);
	shader->SetFloat("spotLight.pointLight.radius", radius);
	shader->SetFloat3("spotLight.direction", getTransform()->getForwardVector());
	shader->SetFloat("spotLight.cutoff", cutoff);
}
