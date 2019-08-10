#pragma once

#include "Orbeus/Editor/Editor.h"

class GameObject;
class SkeletalMesh;

class AnimatedModelEditor : public Editor
{
private:
	GameObject* selectedGameObject;
	SkeletalMesh* selectedMesh;

	int selectedBoneIndex = 0;

public:
	virtual void init() override;
	virtual void destroy() override;
	virtual void update(float deltaTime) override;
	virtual void render() override;
};