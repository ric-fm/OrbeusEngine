#include "AnimatedModelEditor.h"

#include "Orbeus/Core/World.h"
#include "Orbeus/Core/GameObject.h"
#include "Orbeus/Components/SkeletalMesh.h"
#include "Orbeus/Rendering/SkeletalMesh/Animation.h"
#include "Orbeus/Math/Math.h"
#include "Orbeus/Math/Quaternion.h"
#include "Orbeus/Utils/Log.h"
#include "Orbeus/Core/Engine.h"
#include "Orbeus/Rendering/RenderingEngine.h"
#include "Orbeus/Rendering/SkeletalMesh/SkeletalMeshRenderer.h"
#include "Orbeus/Rendering/SkeletalMesh/Bone.h"

void AnimatedModelEditor::init()
{
	auto meshes = World::getInstance().getComponents<SkeletalMesh>();
	if (meshes.size() > 0)
	{
		selectedMesh = meshes[0];
		selectedGameObject = selectedMesh->getOwner();
		if (selectedMesh->animationData != nullptr)
		{
			selectedMesh->setPlaying(false);
			selectedMesh->setAnimation(selectedMesh->getAnimationData()->animations[0], 0.0f);
		}
		else
			selectedMesh = nullptr;
	}
}

void AnimatedModelEditor::destroy()
{
}

void AnimatedModelEditor::update(float deltaTime)
{
}

void recursiveGetBones(Bone* bone, std::vector<Bone*>& bones)
{
	bones.push_back(bone);
	for (Bone* childBone : bone->children)
	{
		recursiveGetBones(childBone, bones);
	}
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
		bool drawMesh = Engine::getInstance().getRenderingEngine()->getSkeletalMeshRenderer()->drawMeshes;
		if (ImGui::Checkbox("Draw mesh", &drawMesh))
		{
			Engine::getInstance().getRenderingEngine()->getSkeletalMeshRenderer()->drawMeshes = drawMesh;
		}
		bool drawBones = Engine::getInstance().getRenderingEngine()->getSkeletalMeshRenderer()->drawBones;
		if (ImGui::Checkbox("Draw bones", &drawBones))
		{
			Engine::getInstance().getRenderingEngine()->getSkeletalMeshRenderer()->drawBones = drawBones;
		}

		BoneData* boneData = selectedMesh->getBoneData();
		std::vector<Bone*> bones;
		recursiveGetBones(boneData->rootBone, bones);
		std::string boneItems;
		for (unsigned int i = 0; i < bones.size(); ++i)
		{
			Bone* bone = bones[i];
			boneItems += bone->name;
			boneItems += '\0';
		}
		boneItems += '\0';
		bool boneChanged = false;
		if (ImGui::Combo("Bones", &selectedBoneIndex, boneItems.c_str()))
		{
			
		}

		std::vector<Matrix4>& pose = selectedMesh->currentPoseV;
		Matrix4& currentBoneMatrix = pose[selectedBoneIndex];

		float positions[3];
		Vector3 bonePosition = currentBoneMatrix.getTranslation();
		positions[0] = bonePosition[0];
		positions[1] = bonePosition[1];
		positions[2] = bonePosition[2];
		if (ImGui::SliderFloat3("Position", positions, -100, 100, "%.2f"))
		{
			bonePosition.set(positions[0], positions[1], positions[2]);
			Log::info("New bone position %s", bonePosition.toString().c_str());
			boneChanged = true;
		}
		//if (ImGui::InputFloat3("Position", positions))
		//{
		//	bonePosition.set(positions[0], positions[1], positions[2]);
		//	Log::info("New bone position %s", bonePosition.toString().c_str());
		//	boneChanged = true;
		//}

		float rotations[3];
		Vector3 boneRotation = currentBoneMatrix.getRotation().getEulerAngles();
		rotations[0] = boneRotation[0];
		rotations[1] = boneRotation[1];
		rotations[2] = boneRotation[2];
		if (ImGui::SliderFloat3("Rotation", rotations, -180, 180, "%.2f"))
		{
			boneRotation.set(rotations[0], rotations[1], rotations[2]);
			Log::info("New bone rotation %s", boneRotation.toString().c_str());
			boneChanged = true;
		}
		//if (ImGui::InputFloat3("Rotation", rotations))
		//{
		//	boneRotation.set(rotations[0], rotations[1], rotations[2]);
		//	Log::info("New bone rotation %s", boneRotation.toString().c_str());
		//	boneChanged = true;
		//}
		if (boneChanged)
		{
			currentBoneMatrix = Matrix4::Translation(bonePosition) * Matrix4::Rotation(Quaternion::EulerAngles(boneRotation));
		}

		AnimationData* animData = selectedMesh->getAnimationData();
		Animation* currentAnimation = selectedMesh->getCurrentAnimation();

		std::string animationItems;
		for (unsigned int i = 0; i < animData->animations.size(); ++i)
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
