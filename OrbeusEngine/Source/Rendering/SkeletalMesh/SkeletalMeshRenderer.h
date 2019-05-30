#pragma once

#include "Components\SkeletalMesh.h"

class Shader;
class Camera;
class MeshData;

class Matrix4;
struct Bone;

class SkeletalMeshRenderer
{
private:
	Shader* shader;
	Shader* boneShader;
	MeshData* boneMeshData;

	void recursiveDrawBones(Bone* currentBone);
	void RecursiveGetBoneTransforms(Bone* currentBone, const Matrix4& parentTransform, std::vector<Matrix4>& transforms);

public:
	SkeletalMeshRenderer();
	~SkeletalMeshRenderer();
	void render(Camera* camera);
};
