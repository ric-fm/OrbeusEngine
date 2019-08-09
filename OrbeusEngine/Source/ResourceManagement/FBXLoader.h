// http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html?url=files/GUID-618465BC-5060-406C-9E83-097430B2C3A2.htm,topicNumber=d30e4361,hash=WS8E4C2438B09B7F9C-50E6E6531197CCD93C5-7FF0
#pragma once

#include <fbxsdk.h>
#include <string>
#include <unordered_map>
#include <set>

#include "Math/Vector3.h"
#include "Math/Vector2.h"
#include "Math/Matrix4.h"
#include "Math/Quaternion.h"
#include "Utils/Hash.h"

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
	bool isRoot = false;
	KeyCurve* keyCurve;
	std::vector<NodeAnimData*> children;
};

struct Vertex2
{
	Vector3 position;
	Vector3 normal;
	Vector2 texCoord;
	Vector3 ids;
	Vector3 weights;

	bool operator==(const Vertex2& other) const
	{
		return (position == other.position
			&& normal == other.normal
			&& texCoord == other.texCoord);
	}
};
namespace std
{
	template<>
	struct hash<Vertex2>
	{
		size_t operator()(Vertex2 const& vertex) const noexcept
		{
			std::size_t result = 0;
			hash_combine(result, vertex.position.x);
			hash_combine(result, vertex.position.y);
			hash_combine(result, vertex.position.z);
			hash_combine(result, vertex.normal.x);
			hash_combine(result, vertex.normal.y);
			hash_combine(result, vertex.normal.z);
			hash_combine(result, vertex.texCoord.x);
			hash_combine(result, vertex.texCoord.y);
			return result;
		}
	};
}


class FBXLoader
{
private:
	static MeshData* getMesh(FbxScene* scene, FbxMesh* mesh, FbxNode* skeletonRootNode, const Matrix4& meshMatrix);

public:
	static void loadModel(const std::string& filePath, MeshData*& meshData, BoneData*& boneData, AnimationData*& animationData);
};
