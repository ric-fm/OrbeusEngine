#pragma once

#include <string>
#include <vector>
#include "Math/Matrix4.h"

struct Bone
{
public:
	int id;
	std::string name;
	Matrix4 transformMatrix;
	Matrix4 inverseTransformMatrix;
	Bone* parent;
	std::vector<Bone*> children;
};

class BoneData
{
public:
	Bone* rootBone;
	std::vector<Bone*> bones;
};