// http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html?url=files/GUID-618465BC-5060-406C-9E83-097430B2C3A2.htm,topicNumber=d30e4361,hash=WS8E4C2438B09B7F9C-50E6E6531197CCD93C5-7FF0
#pragma once

#include <fbxsdk.h>
#include <string>
#include <unordered_map>

#include "Math/Vector3.h"
#include "Math/Matrix4.h"
#include "Math/Quaternion.h"

class MeshData;
class BoneData;
class AnimationData;

struct ComponentCurve
{
	FbxAnimCurve* x;
	FbxAnimCurve* y;
	FbxAnimCurve* z;
};

struct KeyCurve
{
	ComponentCurve* posCurve;
	ComponentCurve* rotCurve;
	FbxTime minTime;
	FbxTime maxTime;
};

struct NodeAnimData
{
	FbxNode* node;
	KeyCurve* keyCurve;
	std::vector<NodeAnimData*> children;
};

class FBXLoader
{
private:
	static MeshData* getMesh(FbxScene* scene, FbxMesh* mesh, FbxNode* skeletonRootNode);

public:
	static void loadModel(const std::string& filePath, MeshData*& meshData, BoneData*& boneData, AnimationData*& animationData);
};
