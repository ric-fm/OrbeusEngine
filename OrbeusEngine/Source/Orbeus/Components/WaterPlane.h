#pragma once

#include "Orbeus/Core/GameComponent.h"

class Quad;
class Texture;

class WaterPlane : public GameComponent
{
	friend class WaterRenderer;
private:
	Quad* quad;
	Texture* reflectionTexture;
	Texture* refractionTexture;
	Texture* refractionDepthTexture;
	Texture* dudvMap;
	Texture* normalMap;

public:
	WaterPlane();
	~WaterPlane();

	Texture* getReflectionTexture() const { return reflectionTexture; }
	void setReflectionTexture(Texture* texture) { reflectionTexture = texture; }
	Texture* getRefractionTexture() const { return refractionTexture; }
	void setRefractionTexture(Texture* texture) { refractionTexture = texture; }
	Texture* getRefractionDepthTexture() const { return refractionDepthTexture; }
	void setRefractionDepthTexture(Texture* texture) { refractionDepthTexture = texture; }
	Texture* getDudvMap() const { return dudvMap; }
	void setDudvMap(Texture* texture) { dudvMap = texture; }
	Texture* getNormalMap() const { return normalMap; }
	void setNormalMap(Texture* texture) { normalMap = texture; }
};
