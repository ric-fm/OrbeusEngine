#include "AnimatedModelEditor.h"

#include "Core/World.h"
#include "Core/GameObject.h"
#include "Components/SkeletalMesh.h"
#include "Rendering/SkeletalMesh/Animation.h"
#include "Math/Math.h"
#include "Utils/Log.h"

void AnimatedModelEditor::init()
{
	auto meshes = World::getInstance().getComponents<SkeletalMesh>();
	if (meshes.size() > 0)
	{
		selectedMesh = meshes[0];
		selectedGameObject = selectedMesh->getOwner();
		selectedMesh->setAnimation(selectedMesh->getAnimationData()->animations[0]);
	}
}

void AnimatedModelEditor::destroy()
{
}

void AnimatedModelEditor::update(float deltaTime)
{
}

void AnimatedModelEditor::render()
{
	if (!selectedMesh)
		return;
	ImGui::SetNextWindowPos(ImVec2(400, 20), ImGuiCond_Always, ImVec2(0, 0));
	ImGui::SetNextWindowBgAlpha(0.1f);
	if (ImGui::Begin("Animation Info", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		ImGui::Text("GameObject: %s", selectedGameObject->getName().c_str());
		AnimationData* animData = selectedMesh->getAnimationData();
		Animation* currentAnimation = selectedMesh->getCurrentAnimation();

		std::string animationItems;
		for (int i = 0; i < animData->animations.size(); ++i)
		{
			Animation* animation = animData->animations[i];
			animationItems += animation->name;
			animationItems += '\0';
		}
		animationItems += '\0';

		std::vector<Animation*>& animations = selectedMesh->animationData->animations;
		int animationIndex = find(animations.begin(), animations.end(), currentAnimation) - animations.begin();
		if (ImGui::Combo("Animation", &animationIndex, animationItems.c_str()))
		{
			selectedMesh->setAnimation(selectedMesh->getAnimationData()->animations[animationIndex]);
		}
		if (currentAnimation)
		{
			ImGui::Text("%f s | %d frames", (currentAnimation->length), currentAnimation->frames.size());

			float animationLength = currentAnimation->length;
			float animationTime = selectedMesh->getAnimationTime();
			if (ImGui::SliderFloat("Time", &animationTime, 0.0f, animationLength))
			{
				selectedMesh->setAnimationTime(animationTime);
			}
		}

		bool looping = selectedMesh->isLooping();
		if (ImGui::Checkbox("Loop", &looping))
		{
			selectedMesh->setLooping(looping);
		}

		float rate = selectedMesh->getRate();
		if (ImGui::InputFloat("Rate", &rate, 0.1f, 1.0f))
		{
			selectedMesh->setRate(rate);
		}

		bool playing = selectedMesh->isPlaying();
		if (ImGui::Button((playing ? "PAUSE" : "PLAY")))
		{
			playing = !playing;
			selectedMesh->setPlaying(playing);
		}
	}
	ImGui::End();
}
