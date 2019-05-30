#pragma once

#include <string>
#include <vector>

#include "Math/Vector3.h"
#include "Math/Quaternion.h"

struct Key
{
	std::string boneName;
	Vector3 position;
	Quaternion rotation;
};

struct KeyFrame
{
	float timeStamp;
	std::vector<Key*> keys;
};

struct Animation
{
	std::string name;
	std::vector<KeyFrame*> frames;
	float length;
};

class AnimationData
{
public:
	std::vector<Animation*> animations;
};