#pragma once

#include "Core/GameComponent.h"

#include <string>
#include <unordered_map>

#include "Math/Matrix4.h"

class MeshData;
class BoneData;
class AnimationData;

struct Animation;
struct KeyFrame;
struct Bone;
struct Key;

class SkeletalMesh : public GameComponent
{
	friend class AnimatedModelEditor;
	friend class SkeletalMeshRenderer;
private:
	MeshData* meshData;
	BoneData* boneData;
	AnimationData* animationData;

	std::string meshFilePath;

	Animation* currentAnimation;
	float animationTime;
	float rate = 1.0f;
	bool playing = true;
	bool looping = true; // TODO: Esto seguramente tenga que ir en la animación

	std::unordered_map<std::string, Matrix4> currentPose;
	std::vector<Matrix4> currentPoseV;


public:
	SkeletalMesh(const std::string& meshFilePath);
	~SkeletalMesh();

	MeshData* getMeshData() const { return meshData; }
	BoneData* getBoneData() const { return boneData; }
	AnimationData* getAnimationData() const { return animationData; }

	std::string getMeshFilePath() const { return meshFilePath; }

	virtual void init() override;
	void update(float deltaTime);

	void setPlaying(bool playing) { this->playing = playing; }
	bool isPlaying() { return playing; }
	void setLooping(bool looping) { this->looping = looping; }
	bool isLooping() { return looping; }
	void setRate(float rate) { this->rate = rate; }
	float getRate() { return rate; }
	void setAnimationTime(float time, bool update = true);
	float getAnimationTime() { return animationTime; }

	void setAnimation(Animation* animation, float time = 0, bool update = true);
	Animation* getCurrentAnimation() { return currentAnimation; }

private:
	std::unordered_map<std::string, Matrix4> calculateCurrentAnimationPose();
	void calculateCurrentPose();
	void getPreviousAndNextFrames(KeyFrame*& previousFrame, KeyFrame*& nextFrame);
	float calculateProgression(KeyFrame* previousFrame, KeyFrame* nextFrame);
	Key interpolateKeys(Key* keyA, Key* keyB, float progression);
	std::unordered_map<std::string, Matrix4> interpolatePoses(KeyFrame* previousFrame, KeyFrame* nextFrame, float progression);
};
