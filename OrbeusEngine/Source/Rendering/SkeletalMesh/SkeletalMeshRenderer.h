#pragma once

#include "Components\SkeletalMesh.h"

class Shader;
class Camera;
class MeshData;

struct Matrix4;
struct Bone;

class SkeletalMeshRenderer
{
	friend class AnimatedModelEditor;
private:
	Shader* shader;
	Shader* boneShader;
	MeshData* boneMeshData;

	// For debug
	bool drawMeshes = true;
	bool drawBones = false;

	void recursiveDrawBones(Bone* currentBone);
	void recursiveGetBoneTransforms(Bone* currentBone, const Matrix4& parentTransform, std::vector<Matrix4>& transforms);

public:
	SkeletalMeshRenderer();
	~SkeletalMeshRenderer();
	void render(Camera* camera);
};
