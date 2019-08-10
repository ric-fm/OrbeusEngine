#include "SkeletalMesh.h"

#include "Orbeus/Core/GameObject.h"

#include "Orbeus/Rendering/Mesh/MeshData.h"
#include "Orbeus/Rendering/SkeletalMesh/Bone.h"
#include "Orbeus/Rendering/SkeletalMesh/Animation.h"

#include "Orbeus/Math/Math.h"
#include "Orbeus/Math/Vector3.h"
#include "Orbeus/Math/Quaternion.h"

#include "Orbeus/ResourceManagement/FBXLoader.h"
#include "Orbeus/Utils/Log.h"

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
		calculateCurrentPose();
	}
}

void SkeletalMesh::setAnimationTime(float time, bool update)
{
	animationTime = time;
	if (update)
	{
		calculateCurrentPose();
	}
}

void SkeletalMesh::setAnimation(Animation* animation, float time, bool update)
{
	animationTime = time;
	currentAnimation = animation;
	if (update)
	{
		calculateCurrentPose();
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

void recursiveGetPose(Bone* bone, const Matrix4& parentTransform, std::unordered_map<std::string, Matrix4>& pose, std::vector<Matrix4>& transforms)
{
	Matrix4 boneTransform = parentTransform * pose[bone->name];
	transforms.push_back(boneTransform);
	for (Bone* childBone : bone->children)
	{
		recursiveGetPose(childBone, boneTransform, pose, transforms);
	}
}
void SkeletalMesh::calculateCurrentPose()
{
	std::unordered_map<std::string, Matrix4> currentPose = calculateCurrentAnimationPose();
	currentPoseV.clear();
	recursiveGetPose(boneData->rootBone, Matrix4(), currentPose, currentPoseV);
	Log::info("");
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

	for (unsigned int keyIndex = 0; keyIndex < previousKeys.size(); keyIndex++)
	{
		Key* previousKey = previousKeys[keyIndex];
		Key* nextKey = nextKeys[keyIndex];
		Key interpolatedKey = interpolateKeys(previousKey, nextKey, progression);

		currentPose[previousKey->boneName] = Matrix4::Translation(interpolatedKey.position) * Matrix4::Rotation(interpolatedKey.rotation);
	}

	return currentPose;
}
