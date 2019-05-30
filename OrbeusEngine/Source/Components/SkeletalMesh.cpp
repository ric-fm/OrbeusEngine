#include "SkeletalMesh.h"

#include "Core/GameObject.h"

#include "Rendering/Mesh/MeshData.h"
#include "Rendering/SkeletalMesh/Bone.h"
#include "Rendering/SkeletalMesh/Animation.h"

#include "Math/Math.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

#include "ResourceManagement/FBXLoader.h"
#include "Utils/Log.h"

SkeletalMesh::SkeletalMesh(const std::string& meshFilePath)
	: meshFilePath(meshFilePath)
{
	// TODO: Tengo que ver cómo integrar la carga en el resouce manager.
	// Aquí el problema es ver si quieres cargar malla, esqueleto, animaciones, ...
	FBXLoader::loadModel(meshFilePath, meshData, boneData, animationData);
}

SkeletalMesh::~SkeletalMesh()
{
	if (meshData)
		delete meshData;
	if (boneData)
		delete boneData;
	if (animationData)
		delete animationData;
}

void SkeletalMesh::init()
{
}

void SkeletalMesh::update(float deltaTime)
{
	if (currentAnimation == nullptr || !this->isEnabled())
		return;

	if (playing)
	{
		// Increase animation time and handle looping/pause
		animationTime += deltaTime * rate;
		if (animationTime < 0)
		{
			if (looping)
				animationTime += currentAnimation->length;
			else
			{
				animationTime = currentAnimation->length;
				playing = false;
			}
		}
		else if (animationTime > currentAnimation->length)
		{
			if (looping)
				animationTime -= currentAnimation->length;
			else
			{
				animationTime = 0.0f;
				playing = false;
			}
		}

		std::unordered_map<std::string, Matrix4> currentPose = calculateCurrentAnimationPose();
		recursiveApplyPoseToBones(currentPose, boneData->rootBone, Matrix4());
	}
}

void SkeletalMesh::setAnimationTime(float time, bool update)
{
	animationTime = time;
	if (update)
	{
		std::unordered_map<std::string, Matrix4> currentPose = calculateCurrentAnimationPose();
		recursiveApplyPoseToBones(currentPose, boneData->rootBone, Matrix4());
	}
}

void SkeletalMesh::setAnimation(Animation* animation, float time, bool update)
{
	animationTime = time;
	currentAnimation = animation;
	if (update)
	{
		std::unordered_map<std::string, Matrix4> currentPose = calculateCurrentAnimationPose();
		recursiveApplyPoseToBones(currentPose, boneData->rootBone, Matrix4());
	}
}

std::unordered_map<std::string, Matrix4> SkeletalMesh::calculateCurrentAnimationPose()
{
	KeyFrame* previousFrame = nullptr;
	KeyFrame* nextFrame = nullptr;
	getPreviousAndNextFrames(previousFrame, nextFrame);
	float progression = calculateProgression(previousFrame, nextFrame);

	return interpolatePoses(previousFrame, nextFrame, progression);
}

void SkeletalMesh::recursiveApplyPoseToBones(std::unordered_map<std::string, Matrix4>& currentPose, Bone* bone, const Matrix4& parentTransform)
{
	Matrix4 currentLocalTransform = currentPose[bone->name];
	Matrix4 currentTransform = parentTransform * currentLocalTransform;
	bone->transformMatrix = currentTransform;

	std::vector<Bone*> children = bone->children;
	for (unsigned int i = 0; i < bone->children.size(); ++i)
	{
		Bone* childBone = bone->children[i];
		recursiveApplyPoseToBones(currentPose, childBone, currentTransform);
	}
}

void SkeletalMesh::getPreviousAndNextFrames(KeyFrame*& previousFrame, KeyFrame*& nextFrame)
{
	//TODO: No estoy considerando de momento el looping al obteter los frames, por lo que la interpolación no existe en el loop.
	std::vector<KeyFrame*> result;
	std::vector<KeyFrame*>& frames = currentAnimation->frames;
	previousFrame = frames[0];
	nextFrame = frames[0];
	for (unsigned int i = 1; i < frames.size(); ++i)
	{
		nextFrame = frames[i];
		if (nextFrame->timeStamp > animationTime)
		{
			break;
		}
		previousFrame = frames[i];
	}
}

float SkeletalMesh::calculateProgression(KeyFrame* previousFrame, KeyFrame* nextFrame)
{
	float totalTime = nextFrame->timeStamp - previousFrame->timeStamp;
	float currentTime = animationTime - previousFrame->timeStamp;
	return currentTime / (totalTime + Math::Epsilon); // Avoid division by 0 adding Epsilon
}

Key SkeletalMesh::interpolateKeys(Key* keyA, Key* keyB, float progression)
{
	Key result;
	result.boneName = keyA->boneName;
	result.position = Vector3::Lerp(keyA->position, keyB->position, progression);
	result.rotation = Quaternion::Lerp(keyA->rotation, keyB->rotation, progression);
	return result;
}

std::unordered_map<std::string, Matrix4> SkeletalMesh::interpolatePoses(KeyFrame* previousFrame, KeyFrame* nextFrame, float progression)
{
	std::unordered_map<std::string, Matrix4> currentPose;

	std::vector<Key*>& previousKeys = previousFrame->keys;
	std::vector<Key*>& nextKeys = nextFrame->keys;

	for (int keyIndex = 0; keyIndex < previousKeys.size(); keyIndex++)
	{
		Key* previousKey = previousKeys[keyIndex];
		Key* nextKey = nextKeys[keyIndex];
		Key interpolatedKey = interpolateKeys(previousKey, nextKey, progression);

		currentPose[previousKey->boneName] = Matrix4::Translation(interpolatedKey.position) * Matrix4::Rotation(interpolatedKey.rotation);
	}

	return currentPose;
}
