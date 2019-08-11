#include "FBXLoader.h"

#include "Orbeus/Math/Math.h"
#include "Orbeus/Math/Matrix4.h"
#include "Orbeus/Math/Vector2.h"
#include "Orbeus/Math/Vector3.h"
#include "Orbeus/Math/Quaternion.h"

#include "Orbeus/Rendering/Buffer.h"
#include "Orbeus/Rendering/Material.h"
#include "Orbeus/Rendering/Mesh/MeshData.h"
#include "Orbeus/Rendering/SkeletalMesh/Bone.h"
#include "Orbeus/Rendering/SkeletalMesh/Animation.h"
#include "Orbeus/ResourceManagement/ResourceManager.h"

#include "Orbeus/Utils/String.h"
#include "Orbeus/Utils/Log.h"

#include <assert.h> 

Matrix4 getFbxNodeTransformMatrix(FbxNode* node, bool global = true, FbxTime time = FbxTime(0))
{
	FbxMatrix meshMatrix;
	if (global)
		meshMatrix = node->EvaluateGlobalTransform(time);
	else
		meshMatrix = node->EvaluateLocalTransform(time);
	float f = 100;
	Matrix4 transform = Matrix4(
		(float)meshMatrix.Get(0, 0) / f,
		(float)meshMatrix.Get(1, 0),
		(float)meshMatrix.Get(2, 0),
		(float)meshMatrix.Get(3, 0),

		(float)meshMatrix.Get(0, 1),
		(float)meshMatrix.Get(1, 1),
		(float)meshMatrix.Get(2, 1) / f,
		(float)meshMatrix.Get(3, 1),

		(float)meshMatrix.Get(0, 2),
		(float)meshMatrix.Get(1, 2) / f,
		(float)meshMatrix.Get(2, 2),
		(float)meshMatrix.Get(3, 2),

		(float)meshMatrix.Get(0, 3),
		(float)meshMatrix.Get(1, 3),
		(float)meshMatrix.Get(2, 3),
		(float)meshMatrix.Get(3, 3)
	);
	//transform.transpose();
	//return transform;


	double sign;
	FbxVector4 scale;
	FbxVector4 shearing;
	FbxQuaternion rotation;
	FbxVector4 translation;
	meshMatrix.GetElements(translation, rotation, shearing, scale, sign);

	Vector3 pos((float)translation.mData[0], (float)translation.mData[1], (float)translation.mData[2]);
	Quaternion rot((float)rotation.mData[0], (float)rotation.mData[1], (float)rotation.mData[2], (float)rotation.mData[3]);
	Vector3 sc((float)scale.mData[0], (float)scale.mData[1], (float)scale.mData[2]);

	//Matrix4 mat = Matrix4::Translation(pos) * Matrix4::Rotation(rot) * Matrix4::Scaling(sc / 100.0f);

	return Matrix4::Translation(pos / 100.0f) * Matrix4::Rotation(rot) * Matrix4::Scaling(sc / 100.0f);
}

void recursiveIndexMapSkeleton(FbxNode* currentNode, std::unordered_map<std::string, int>& boneMap, int idx = 0)
{
	std::string boneName = currentNode->GetName();

	boneMap[boneName] = idx++;

	int childrenCount = currentNode->GetChildCount();
	for (int childIndex = 0; childIndex < childrenCount; ++childIndex)
	{
		FbxNode* childNode = currentNode->GetChild(childIndex);
		if (childNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			recursiveIndexMapSkeleton(childNode, boneMap, idx);
		}
	}
}

void recursiveParseBones(FbxNode* currentNode, Bone* parentBone, std::vector<Bone*>& bones, std::unordered_map<std::string, Bone*>& boneMap)
{
	std::string boneName = currentNode->GetName();
	FbxSkeleton* skeleton = currentNode->GetSkeleton();

	Bone* bone = new Bone();
	bone->id = bones.size();
	bone->name = boneName;
	bone->parent = parentBone;
	if (parentBone)
		parentBone->children.push_back(bone);

	bones.push_back(bone);
	boneMap[boneName] = bone;
	int childrenCount = currentNode->GetChildCount();
	for (int childIndex = 0; childIndex < childrenCount; ++childIndex)
	{
		FbxNode* childNode = currentNode->GetChild(childIndex);
		if (childNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			recursiveParseBones(childNode, bone, bones, boneMap);
		}
	}
}

BoneData* getBones(FbxScene* scene, FbxMesh* mesh, FbxNode* skeletonRootNode)
{
	BoneData* result = nullptr;
	Bone* rootBone = nullptr;

	int skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount <= 0)
	{
		Log::warning("Mesh has no skin");
		return nullptr;
	}

	FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
	if (!skin)
	{
		Log::warning("Error obtaining skin");
		return nullptr;
	}

	int clusterCount = skin->GetClusterCount();
	if (clusterCount <= 0)
	{
		Log::warning("Skin has no bones");
	}

	std::vector<Bone*> bones;

	// TODO: Mirar bien esto de las poses
	// Puede haber varias poses? De momento lo capamos a 1 pose
	// Parece que puede llegar modelos sin pose
	int poseCount = scene->GetPoseCount();
	for (int poseIndex = 0; poseIndex < poseCount && poseIndex < 1; ++poseIndex)
	{
		FbxPose* pose = scene->GetPose(poseIndex);
		if (!pose->IsBindPose())
		{
			Log::info("PoseIndex %d is no bind pose", poseIndex);
			continue;
		}

		// Parse bones
		//std::vector<Bone*> bones;
		std::unordered_map<std::string, Bone*> boneMap;
		recursiveParseBones(skeletonRootNode, nullptr, bones, boneMap);

		int numBones = bones.size();
		if (numBones <= 0)
		{
			Log::warning("Parse bones failed");
			break;
		}
		Log::info("Mesh has %d bones", numBones);
		rootBone = bones[0];

		// Process bone transforms
		int poseNodeCount = pose->GetCount();
		for (int poseNodeIndex = 0; poseNodeIndex < poseNodeCount; ++poseNodeIndex)
		{
			FbxNode* poseNode = pose->GetNode(poseNodeIndex);
			if (!poseNode)
			{
				Log::warning("Invalid poseNode");
				continue;
			}
			std::string poseNodeName = poseNode->GetName();
			if (boneMap.find(poseNodeName) != boneMap.end())
			{
				Bone* targetBone = boneMap[poseNodeName];

				Matrix4 parentInverseMatrix;
				if (targetBone->parent != nullptr)
					parentInverseMatrix = targetBone->parent->inverseTransformMatrix;

				targetBone->transformMatrix = getFbxNodeTransformMatrix(poseNode) * parentInverseMatrix;
				targetBone->inverseTransformMatrix = targetBone->transformMatrix.getInversed();
				Log::info("");
			}
		}
	}
	if (rootBone)
	{
		result = new BoneData();
		result->rootBone = rootBone;
		result->bones = bones;
	}
	return result;
}

void recursiveMapSkeleton(FbxNode* currentNode, std::unordered_map<std::string, FbxNode*>& boneMap)
{
	std::string boneName = currentNode->GetName();

	boneMap[boneName] = currentNode;

	int childrenCount = currentNode->GetChildCount();
	for (int childIndex = 0; childIndex < childrenCount; ++childIndex)
	{
		FbxNode* childNode = currentNode->GetChild(childIndex);
		if (childNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			recursiveMapSkeleton(childNode, boneMap);
		}
	}
}

void getCurveTimes(FbxAnimCurve* curve, std::vector<FbxTime>& times)
{
	FbxTimeSpan timeSpan;
	curve->GetTimeInterval(timeSpan);
	times.push_back(timeSpan.GetStart());
	times.push_back(timeSpan.GetStop());
}

void getMinMaxTime(const std::vector<FbxTime>& times, FbxTime& minTime, FbxTime& maxTime)
{
	if (times.size() == 0)
		return;
	minTime = maxTime = times[0];
	for (unsigned int i = 1; i < times.size(); ++i)
	{
		if (times[i] < minTime)
			minTime = times[i];
		if (times[i] > maxTime)
			maxTime = times[i];
	}
}

void getAnimData(FbxScene* scene, FbxNode* node, FbxAnimStack* animStack, FbxAnimLayer* animLayer, NodeAnimData*& animData)
{
	KeyCurve* keyCurve = animData->keyCurve = new KeyCurve();

	std::vector<FbxTime> keyTimes;

	keyCurve->posCurve = new ComponentCurve();
	keyCurve->rotCurve = new ComponentCurve();

	keyCurve->posCurve->x = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
	if (keyCurve->posCurve->x != nullptr)
	{
		getCurveTimes(keyCurve->posCurve->x, keyTimes);
	}
	keyCurve->posCurve->y = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	if (keyCurve->posCurve->y != nullptr)
	{
		getCurveTimes(keyCurve->posCurve->y, keyTimes);
	}
	keyCurve->posCurve->z = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
	if (keyCurve->posCurve->z != nullptr)
	{
		getCurveTimes(keyCurve->posCurve->z, keyTimes);
	}

	keyCurve->rotCurve->x = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
	if (keyCurve->rotCurve->x != nullptr)
	{
		getCurveTimes(keyCurve->rotCurve->x, keyTimes);
	}
	keyCurve->rotCurve->y = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	if (keyCurve->rotCurve->y != nullptr)
	{
		getCurveTimes(keyCurve->rotCurve->y, keyTimes);
	}
	keyCurve->rotCurve->z = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
	if (keyCurve->rotCurve->z != nullptr)
	{
		getCurveTimes(keyCurve->rotCurve->z, keyTimes);
	}

	getMinMaxTime(keyTimes, keyCurve->minTime, keyCurve->maxTime);
}

void recursiveGetAnimData(FbxScene* scene, FbxNode* currentNode, FbxAnimStack* animStack, FbxAnimLayer* animLayer, NodeAnimData*& animData)
{
	animData = new NodeAnimData();
	animData->node = currentNode;
	getAnimData(scene, currentNode, animStack, animLayer, animData);

	int childrenCount = currentNode->GetChildCount();
	for (int childIndex = 0; childIndex < childrenCount; ++childIndex)
	{
		FbxNode* childNode = currentNode->GetChild(childIndex);
		if (childNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			NodeAnimData* childWorkingNode = nullptr;
			recursiveGetAnimData(scene, childNode, animStack, animLayer, childWorkingNode);
			animData->children.push_back(childWorkingNode);
		}
	}
}

void recursiveGetMinMaxTimes(NodeAnimData* animData, FbxTime& minTime, FbxTime& maxTime)
{
	if (animData->keyCurve->minTime < minTime)
		minTime = animData->keyCurve->minTime;
	if (animData->keyCurve->maxTime > maxTime)
		maxTime = animData->keyCurve->maxTime;
	for (unsigned int childIndex = 0; childIndex < animData->children.size(); ++childIndex)
	{
		recursiveGetMinMaxTimes(animData->children[childIndex], minTime, maxTime);
	}
}

void getAnimationTimes(NodeAnimData* animData, FbxTime& startTime, FbxTime& endTime, FbxTime& length)
{
	recursiveGetMinMaxTimes(animData, startTime, endTime);
	length = endTime - startTime;
}

void recursiveGetKeys(NodeAnimData* animData, FbxTime time, std::vector<Key*>& keys, const Matrix4& meshTransform, std::unordered_map <std::string, Bone*>& boneMap)
{
	ComponentCurve* posCurve = animData->keyCurve->posCurve;
	ComponentCurve* rotCurve = animData->keyCurve->rotCurve;
	Vector3 position(0, 0, 0);
	Vector3 rotation(0, 0, 0);
	if (posCurve->x != nullptr)
	{
		position.x = -posCurve->x->Evaluate(time);
	}
	if (posCurve->y != nullptr)
	{
		position.y = posCurve->y->Evaluate(time);
	}
	if (posCurve->z != nullptr)
	{
		position.z = posCurve->z->Evaluate(time);
	}
	if (rotCurve->x != nullptr)
	{
		rotation.x = rotCurve->x->Evaluate(time);
	}
	if (rotCurve->y != nullptr)
	{
		rotation.y = rotCurve->y->Evaluate(time);
	}
	if (rotCurve->z != nullptr)
	{
		rotation.z = rotCurve->z->Evaluate(time);
	}

	Key* key = new Key();
	key->boneName = animData->node->GetName();
	key->position = position;
	key->rotation = Quaternion::EulerAngles(rotation);

	if (animData->isRoot)
	{
		rotation.y *= -1;
		key->rotation = Quaternion::EulerAngles(rotation);
		Matrix4 t = Matrix4::Rotation(Quaternion::EulerAngles(Vector3(-90, 0, 0)));
		//Matrix4 keyMatrix = meshTransform * Matrix4::Translation(key->position) * Matrix4::Rotation(key->rotation);
		Matrix4 keyMatrix = t * Matrix4::Translation(key->position) * Matrix4::Rotation(key->rotation);
		key->position = keyMatrix.getTranslation();
		key->rotation = keyMatrix.getRotation();
	}
	else
	{
		rotation.z *= -1;
		key->rotation = Quaternion::EulerAngles(rotation);
		Bone* bone = boneMap[key->boneName];
		Matrix4 keyMatrix = Matrix4::Translation(key->position) * Matrix4::Rotation(key->rotation);
		keyMatrix = keyMatrix * bone->inverseTransformMatrix;
		key->position = keyMatrix.getTranslation();
		key->rotation = keyMatrix.getRotation();
	}

	keys.push_back(key);

	std::string boneName = animData->node->GetName();
	if (key->boneName == "Bone")
	{
		Log::info("");
	}
	if (key->boneName == "Bone.001")
	{
		Log::info("");
	}

	for (unsigned int childIndex = 0; childIndex < animData->children.size(); ++childIndex)
	{
		recursiveGetKeys(animData->children[childIndex], time, keys, meshTransform, boneMap);
	}
}

void recursiveMapBones(Bone* bone, std::unordered_map<std::string, Bone*>& boneMap)
{
	boneMap[bone->name] = bone;
	for (Bone* childBone : bone->children)
	{
		recursiveMapBones(childBone, boneMap);
	}
}

AnimationData* getAnimations(FbxScene* scene, FbxNode* skeletonRootNode, BoneData* boneData, const Matrix4& meshTransform)
{
	AnimationData* result;

	int animStackCount = scene->GetMemberCount<FbxAnimStack>();
	if (animStackCount == 0)
	{
		Log::warning("Mesh has no animations");
		return nullptr;
	}

	result = new AnimationData();

	// Process all animations
	for (int animStackIndex = 0; animStackIndex < animStackCount; ++animStackIndex)
	{

		FbxAnimStack* animStack = scene->GetMember<FbxAnimStack>(animStackIndex);
		std::string animName = animStack->GetName();
		std::vector<std::string> subNames;
		splitString(animName, '|', subNames); // TODO: Blender exporta las animaciones de la forma: SkeletonName|AnimationName. Mirar en otros editores.
		animName = subNames[1];

		int animLayerCount = animStack->GetMemberCount<FbxAnimLayer>();
		if (animLayerCount > 0)
		{
			Animation* animation = new Animation();
			animation->name = animName;
			Log::info("Animation: %s", animName.c_str());

			FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>();
			NodeAnimData* rootWorkingNode = nullptr;
			recursiveGetAnimData(scene, skeletonRootNode, animStack, animLayer, rootWorkingNode);
			rootWorkingNode->isRoot = true;

			FbxTime startTime;
			FbxTime endTime;
			FbxTime length;
			getAnimationTimes(rootWorkingNode, startTime, endTime, length);

			float keyFrameStart = (float)startTime.GetSecondDouble();
			float keyFrameEnd = (float)endTime.GetSecondDouble();
			float keyFrameLength = (float)length.GetSecondDouble();
			animation->length = keyFrameLength;// * 1000.0f; //TODO: Mejor en milisegundos o segundos?

			int framesPerSeconds = 24;
			int numFrames = (int)ceil(keyFrameLength * framesPerSeconds);
			float samplingInterval = keyFrameLength / numFrames;

			FbxMatrix meshMatrix = skeletonRootNode->EvaluateGlobalTransform();

			std::unordered_map <std::string, Bone*> boneMap;
			recursiveMapBones(boneData->rootBone, boneMap);

			for (float t = keyFrameStart; t < keyFrameEnd; t += samplingInterval)
			{
				KeyFrame* keyFrame = new KeyFrame();
				keyFrame->timeStamp = t;
				FbxTime currentTime;
				currentTime.SetSecondDouble(t);

				recursiveGetKeys(rootWorkingNode, currentTime, keyFrame->keys, meshTransform, boneMap);
				animation->frames.push_back(keyFrame);
			}
			result->animations.push_back(animation);
		}
		else
		{
			Log::warning("Anim %s has no layers", animName.c_str());
		}
	}
	return result;
}
//
//MeshData* FBXLoader::getMesh(FbxScene* scene, FbxMesh* mesh, FbxNode* skeletonRootNode, const Matrix4& meshMatrix)
//{
//	if (!mesh->GetNode())
//		return nullptr;
//
//	MeshData* result = new MeshData();
//
//	struct Vertex
//	{
//		Vector3 position;
//		Vector3 normal;
//		Vector2 texCoord;
//		Vector3 ids;
//		Vector3 weights;
//	};
//
//	unsigned int polyCount = mesh->GetPolygonCount();
//	unsigned int verticesCount = mesh->GetControlPointsCount();
//	std::vector<Vertex> vertices;
//	vertices.reserve(verticesCount);
//	std::vector<unsigned int> indices;
//	indices.reserve(polyCount * 3);
//
//	// HashMap of ObjectIndex to detect Indexing Vertex Duplication
//	std::unordered_map<unsigned int, unsigned int> processedVertices;
//
//	// Get the UV set name
//	const char* uvSetName = nullptr;
//	FbxStringList uvSetNames;
//	mesh->GetUVSetNames(uvSetNames);
//	if (uvSetNames.GetCount() > 1)
//		Log::warning("Mesh %s has %d uv sets. Only the first will be considered", mesh->GetName(), uvSetNames.GetCount());
//	if (uvSetNames.GetCount() > 0)
//		uvSetName = uvSetNames.GetStringAt(0);
//
//	const FbxVector4* controlPoints = mesh->GetControlPoints();
//	int vertexCount = 0;
//	int vCount = 0;
//	int lPolyIndexCounter = 0;
//	for (int polygonIndex = 0; polygonIndex < polyCount; ++polygonIndex)
//	{
//		//for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
//			for (int vertexIndex = 2; vertexIndex >= 0; --vertexIndex) // For CW order
//		{
//			const int controlPointIndex = mesh->GetPolygonVertex(polygonIndex, vertexIndex);
//
//			if (processedVertices.find(controlPointIndex) != processedVertices.end())
//			{
//				indices.push_back(processedVertices[controlPointIndex]);
//			}
//			else
//			{
//				processedVertices[controlPointIndex] = vertexCount;
//				indices.push_back(vertexCount);
//
//				FbxVector4 currentVertex;
//				FbxVector4 currentNormal;
//				FbxVector2 currentUV;
//
//				currentVertex = controlPoints[controlPointIndex];
//
//				mesh->GetPolygonVertexNormal(polygonIndex, vertexIndex, currentNormal);
//
//				bool unmapped;
//				mesh->GetPolygonVertexUV(polygonIndex, vertexIndex, uvSetName, currentUV, unmapped);
//
//				int normalCount = mesh->GetElementUVCount();
//				FbxGeometryElementUV* vertexUV = mesh->GetElementUV(0);
//				int uvIndex = mesh->GetTextureUVIndex(polygonIndex, vertexIndex);
//				Vector2 mUV;
//				bool useIndex = vertexUV->GetReferenceMode() != FbxGeometryElement::eDirect;
//				int indexCount = useIndex ? vertexUV->GetIndexArray().GetCount() : 0;
//				switch (vertexUV->GetMappingMode())
//				{
//				case FbxGeometryElement::eByControlPoint:
//					Log::info("");
//					break;
//				case FbxGeometryElement::eByPolygonVertex:
//					if (lPolyIndexCounter < indexCount)
//					{
//						int index = useIndex ? vertexUV->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;
//						mUV.x = vertexUV->GetDirectArray().GetAt(index).mData[0];
//						mUV.y = vertexUV->GetDirectArray().GetAt(index).mData[1];
//						lPolyIndexCounter++;
//						Log::info("%d", index);
//					}
//					Log::info("");
//					break;
//				default:
//					Log::info("");
//					break;
//				}
//
//
//				//meshMatrix
//
//				Vertex v = {
//						Vector3(-currentVertex[0], currentVertex[1], currentVertex[2]),
//						Vector3(currentNormal[0], currentNormal[1], currentNormal[2]),
//						//Vector2(currentUV[0], currentUV[1]),
//						mUV,
//						Vector3(-1, -1, -1), // Initialize ids to -1. To used after in the skinning process
//						Vector3() // Initialize weigths to 0
//				};
//
//				Matrix4 vertexMatrix = Matrix4::Translation(v.position);
//				Matrix4 cMatrix = meshMatrix * vertexMatrix;
//				v.position = cMatrix.getTranslation();
//
//				Vector3 pos = v.position;
//				Vector2 uv = v.texCoord;
//
//				double uvX = currentUV[0];
//				double uvY = currentUV[1];
//				float fuvX = uvX;
//				float fuvY = uvY;
//
//				vertices.push_back(
//					v
//				);
//				//vertices.push_back(
//				//	{
//				//		Vector3(currentVertex[0], currentVertex[1], currentVertex[2]),
//				//		Vector3(currentNormal[0], currentNormal[1], currentNormal[2]),
//				//		Vector2(currentUV[0], 1.0f - currentUV[1]),
//				//		Vector3(-1, -1, -1), // Initialize ids to -1. To used after in the skinning process
//				//		Vector3() // Initialize weigths to 0
//				//	}
//				//);
//				++vertexCount;
//			}
//		}
//	}
//
//	// Skinning
//	int skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
//	if (skinCount > 0)
//	{
//		FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
//		if (!skin)
//		{
//			Log::warning("Error obtaining mesh skin");
//			return nullptr;
//		}
//
//		int clusterCount = skin->GetClusterCount();
//		if (clusterCount <= 0)
//		{
//			Log::warning("Skin has no bones");
//			return nullptr;
//		}
//
//		if (skeletonRootNode)
//		{
//			std::unordered_map<std::string, int> boneIndexMap;
//			recursiveIndexMapSkeleton(skeletonRootNode, boneIndexMap);
//
//			for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
//			{
//				FbxCluster* cluster = skin->GetCluster(clusterIndex);
//				FbxNode* clusterLink = cluster->GetLink();
//				std::string boneName = clusterLink->GetName();
//				int boneIndex = boneIndexMap[boneName];
//				if (!clusterLink)
//				{
//					Log::warning("cluster has no link");
//					continue;
//				}
//
//				int pointCount = cluster->GetControlPointIndicesCount();
//				if (pointCount == 0)
//				{
//					Log::warning("cluster has 0 point indices");
//					continue;
//				}
//
//				const int* pointIndices = cluster->GetControlPointIndices();
//				const double* pointWeights = cluster->GetControlPointWeights();
//
//				for (int pointIndex = 0; pointIndex < pointCount; ++pointIndex)
//				{
//					int controlPointIndex = pointIndices[pointIndex];
//					float weight = pointWeights[pointIndex];
//
//					if (processedVertices.find(controlPointIndex) != processedVertices.end())
//					{
//						int vertexIndex = processedVertices[controlPointIndex];
//						Vertex& vertex = vertices[vertexIndex];
//
//						Vector3& ids = vertex.ids;
//						Vector3& weights = vertex.weights;
//						// TODO: Ordenar por peso y quedarme con los mas relevantes
//						for (int i = 0; i < 3; ++i)
//						{
//							if (ids[i] < 0)
//							{
//								ids[i] = boneIndex;
//								weights[i] = weight;
//								break;
//							}
//						}
//					}
//				}
//			}
//
//			// Check vertices bone ids an set to 0 for correct use in shader
//			// TODO: Mirar si realmente es necesario
//			for (int i = 0; i < vertices.size(); ++i)
//			{
//				Vector3& idx = vertices[i].ids;
//				for (int j = 2; j >= 0; -j)
//				{
//					if (idx[j] >= 0)
//						break;
//					idx[j] = 0;
//				}
//			}
//		}
//	}
//
//	// Material
//	std::vector<Material>& materials = result->materials;
//	FbxNode* meshNode = mesh->GetNode();
//	int materialCount = meshNode->GetMaterialCount();
//	if (materialCount > 0)
//	{
//		FbxSurfaceMaterial* surfaceMaterial = meshNode->GetMaterial(0);
//		if (surfaceMaterial)
//		{
//			Material material;
//			FbxProperty p = surfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
//			if (p.IsValid())
//			{
//				FbxDouble3 diffuseColor = p.Get<FbxDouble3>();
//				material.diffuse.set(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
//			}
//			p = surfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
//			if (p.IsValid())
//				FbxDouble diffuseIntensity = p.Get<FbxDouble>();
//
//			p = surfaceMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
//			if (p.IsValid())
//			{
//				FbxDouble3 specularColor = p.Get<FbxDouble3>();
//				material.specular.set(specularColor[0], specularColor[1], specularColor[2]);
//			}
//			p = surfaceMaterial->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
//			if (p.IsValid())
//			{
//				FbxDouble specularIntensity = p.Get<FbxDouble>();
//				material.specularIntensity = (float)specularIntensity;
//			}
//
//			for (int layerIndex = 0; layerIndex < FbxLayerElement::sTypeTextureCount; ++layerIndex)
//			{
//				FbxProperty materialProperty = surfaceMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[layerIndex]);
//				if (materialProperty.IsValid())
//				{
//					int textureCount = materialProperty.GetSrcObjectCount<FbxTexture>();
//					for (int textureIndex = 0; textureIndex < textureCount; ++textureIndex)
//					{
//						FbxTexture* texture = materialProperty.GetSrcObject<FbxTexture>(textureIndex);
//						if (texture)
//						{
//							std::string propertyName = materialProperty.GetName().Buffer();
//							FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);
//							std::string textureName = texture->GetName();
//
//							if (fileTexture)
//							{
//								if (propertyName == "DiffuseColor")
//								{
//									std::string texturePath = fileTexture->GetFileName();
//									Texture* texture = ResourceManager::getInstance().getTexture(texturePath, "texture_diffuse");
//									material.diffuseTexture = texture;
//									material.useDiffuseTexture = true;
//
//								}
//								else if (propertyName == "SpecularColor")
//								{
//									std::string texturePath = fileTexture->GetFileName();
//									Texture* texture = ResourceManager::getInstance().getTexture(texturePath, "texture_specular");
//									material.specularTexture = texture;
//									material.useSpecularTexture = true;
//								}
//							}
//						}
//					}
//				}
//			}
//			materials.push_back(material);
//		}
//	}
//
//	// Load VAO
//	result->vertexArray = new VertexArray();
//	result->vertexBuffer = new VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), vertices.size());
//	result->indexBuffer = new IndexBuffer(indices.data(), indices.size());
//
//	VertexBufferLayout layout;
//	layout.Push<float>(3); // pos
//	layout.Push<float>(3); // normal
//	layout.Push<float>(2); // uv
//	layout.Push<float>(3); // ids
//	layout.Push<float>(3); // weights
//
//	result->vertexArray->AddBuffer(result->vertexBuffer, layout);
//	result->vertexArray->setIndexBuffer(result->indexBuffer);
//
//	return result;
//}

void recursiveGetFBXNodes(FbxNode* node, std::vector<FbxMesh*>& meshes, std::vector<FbxNode*>& skeletons)
{
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
	if (nodeAttribute)
	{
		FbxNodeAttribute::EType attributeType = nodeAttribute->GetAttributeType();
		if (attributeType == FbxNodeAttribute::eMesh)
		{
			meshes.push_back(node->GetMesh());
		}
		else if (attributeType == FbxNodeAttribute::eSkeleton)
		{
			skeletons.push_back(node);
		}
	}

	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		recursiveGetFBXNodes(node->GetChild(i), meshes, skeletons);
	}
}

bool parseFile(const std::string& filePath, FbxScene*& scene, std::vector<FbxMesh*>& meshes, std::vector<FbxNode*>& skeletons)
{
	FbxManager* fbxManager = FbxManager::Create();
	FbxIOSettings* ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ioSettings);

	(*(fbxManager->GetIOSettings())).SetBoolProp(IMP_FBX_MATERIAL, true);
	(*(fbxManager->GetIOSettings())).SetBoolProp(IMP_FBX_TEXTURE, true);
	(*(fbxManager->GetIOSettings())).SetBoolProp(IMP_FBX_LINK, true);
	(*(fbxManager->GetIOSettings())).SetBoolProp(IMP_FBX_SHAPE, true);
	(*(fbxManager->GetIOSettings())).SetBoolProp(IMP_FBX_GOBO, true);
	(*(fbxManager->GetIOSettings())).SetBoolProp(IMP_FBX_ANIMATION, true);
	(*(fbxManager->GetIOSettings())).SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

	FbxImporter* importer = FbxImporter::Create(fbxManager, "");
	bool importStatus = importer->Initialize(filePath.c_str(), -1, fbxManager->GetIOSettings());
	if (!importStatus)
	{
		Log::error("FbxImporter::Initialize() failed: %s", importer->GetStatus().GetErrorString());
		return false;
	}

	scene = FbxScene::Create(fbxManager, "ImportScene");
	importer->Import(scene);

	// Convert Axis System
	// Default FBX SDK axis is right handed, y-up
	FbxAxisSystem sceneAxisSystem = scene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem ourAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
	if (sceneAxisSystem != ourAxisSystem)
	{
		Log::warning("Converting axis system");
		ourAxisSystem.ConvertScene(scene);
		Log::warning("Axis system converted");
	}

	FbxSystemUnit sceneSystemUnit = scene->GetGlobalSettings().GetSystemUnit();
	if (sceneSystemUnit.GetScaleFactor() != 1.0)
	{
		Log::warning("Converting system unit");
		FbxSystemUnit::cm.ConvertScene(scene);
		Log::warning("System unit converted");
	}

	Log::info("Triangulating scene");
	FbxGeometryConverter lGeomConverter(fbxManager);
	lGeomConverter.Triangulate(scene, true);
	Log::info("Scene triangulated");

	Log::info("Getting relevant nodes");
	recursiveGetFBXNodes(scene->GetRootNode(), meshes, skeletons);
	Log::info("Getting relevant nodes completed");

	return true;
}



MeshData* FBXLoader::getMesh(FbxScene* scene, FbxMesh* mesh, FbxNode* skeletonRootNode, const Matrix4& meshMatrix)
{
	if (!mesh->GetNode())
		return nullptr;

	MeshData* result = new MeshData();

	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector2 texCoord;
		Vector3 ids;
		Vector3 weights;
	};
	/*
	namespace std
	{
		template<>
		struct hash<Vertex>
		{
			size_t operator()(Vertex const& vertex) const noexcept
			{
				std::size_t result = 0;
				hash_combine(result, vertex.position);
				hash_combine(result, vertex.normal);
				hash_combine(result, vertex.texCoord);
				return result;
			}
		};
	}*/

	unsigned int polygonCount = mesh->GetPolygonCount();
	int polygonSize = mesh->GetPolygonSize(0);
	Log::info("poly syze %d", polygonSize);

	FbxLayerElementArrayTemplate<int>* materialIndice = nullptr;
	FbxGeometryElement::EMappingMode materialMappingMode = FbxGeometryElement::eNone;

	// For every material, record the offsets in every VBO and triangle counts
	struct SubMesh
	{
		SubMesh() : IndexOffset(0), TriangleCount(0) {}

		int IndexOffset;
		int TriangleCount;
	};
	FbxArray<SubMesh*> subMeshes;
	if (mesh->GetElementMaterial())
	{
		materialIndice = &mesh->GetElementMaterial()->GetIndexArray();
		materialMappingMode = mesh->GetElementMaterial()->GetMappingMode();
		if (materialIndice && materialMappingMode == FbxGeometryElement::eByPolygon)
		{
			assert(materialIndice->GetCount() == polygonCount);
			if (materialIndice->GetCount() == polygonCount)
			{
				for (unsigned int polyIndex = 0; polyIndex < polygonCount; ++polyIndex)
				{
					const int materialIndex = materialIndice->GetAt(polyIndex);
					if (subMeshes.GetCount() < materialIndex + 1)
					{
						subMeshes.Resize(materialIndex + 1);
					}
					if (subMeshes[materialIndex] == nullptr)
					{
						subMeshes[materialIndex] = new SubMesh;
					}
					subMeshes[materialIndex]->TriangleCount += 1;
				}

				// Make sure we have no "holes" (NULL) in the mSubMeshes table. This can happen
				// if, in the loop above, we resized the mSubMeshes by more than one slot.
				for (int i = 0; i < subMeshes.GetCount(); ++i)
				{
					if (subMeshes[i] == nullptr)
						subMeshes[i] = new SubMesh;
				}

				// Record the offset (how many vertex)
				const int materialCount = subMeshes.GetCount();
				int offset = 0;
				for (int index = 0; index < materialCount; ++index)
				{
					subMeshes[index]->IndexOffset = offset;
					offset += subMeshes[index]->TriangleCount * 3;
					// This will be used as counter in the following procedures, reset to zero
					subMeshes[index]->TriangleCount = 0;
				}
				assert(offset == polygonCount * 3);
			}
		}
	}

	// All faces will use the same material.
	if (subMeshes.GetCount() == 0)
	{
		subMeshes.Resize(1);
		subMeshes[0] = new SubMesh;
	}

	bool allByControlPoint = true;
	bool hasNormal = mesh->GetElementNormalCount() > 0;
	bool hasUV = mesh->GetElementUVCount() > 0;
	FbxGeometryElement::EMappingMode normalMappingMode = FbxGeometryElement::eNone;
	FbxGeometryElement::EMappingMode uvMappingMode = FbxGeometryElement::eNone;
	if (hasNormal)
	{
		normalMappingMode = mesh->GetElementNormal(0)->GetMappingMode();
		if (normalMappingMode == FbxGeometryElement::eNone)
		{
			hasNormal = false;
		}
		if (hasNormal && normalMappingMode != FbxGeometryElement::eByControlPoint)
		{
			allByControlPoint = false;
		}
	}
	if (hasUV)
	{
		uvMappingMode = mesh->GetElementUV(0)->GetMappingMode();
		if (uvMappingMode == FbxGeometryElement::eNone)
		{
			hasUV = false;
		}
		if (hasUV && uvMappingMode != FbxGeometryElement::eByControlPoint)
		{
			allByControlPoint = false;
		}
	}

	int TRIANGLE_VERTEX_COUNT = 3;
	int polygonVertexCount = mesh->GetControlPointsCount();
	if (!allByControlPoint)
	{
		polygonVertexCount = polygonCount * TRIANGLE_VERTEX_COUNT;
	}
	int VERTEX_STRIDE = 4;
	int NORMAL_STRIDE = 3;
	int UV_STRIDE = 2;

	std::vector<Vertex> vertices(polygonVertexCount);
	std::vector<unsigned int> indices(polygonCount * TRIANGLE_VERTEX_COUNT);

	std::unordered_map<Vertex2, unsigned int> verticesMap;
	std::vector<Vertex2> vertices2;
	std::vector<unsigned int> indices2(polygonCount * TRIANGLE_VERTEX_COUNT);

	FbxStringList uvNames;
	mesh->GetUVSetNames(uvNames);
	const char* uvName = nullptr;
	if (hasUV && uvNames.GetCount())
	{
		uvName = uvNames[0];
	}

	const FbxVector4* controlPoints = mesh->GetControlPoints();
	FbxVector4 currentVertex;
	FbxVector4 currentNormal;
	FbxVector2 currentUV;
	if (allByControlPoint)
	{
		const FbxGeometryElementNormal* normalElement = nullptr;
		const FbxGeometryElementUV* uvElement = nullptr;
		if (hasNormal)
		{
			normalElement = mesh->GetElementNormal(0);
		}
		if (hasUV)
		{
			uvElement = mesh->GetElementUV(0);
		}
		for (int index = 0; index < polygonVertexCount; ++index)
		{
			Vector3 position;
			Vector3 normal;
			Vector2 uv;

			// Save the vertex position.
			currentVertex = controlPoints[index];
			position[0] = static_cast<float>(currentVertex[0]);
			position[1] = static_cast<float>(currentVertex[1]);
			position[2] = static_cast<float>(currentVertex[2]);

			// Save the normal.
			if (hasNormal)
			{
				int normalIndex = index;
				if (normalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
				{
					normalIndex = normalElement->GetIndexArray().GetAt(index);
				}
				currentNormal = normalElement->GetDirectArray().GetAt(normalIndex);
				normal[0] = static_cast<float>(currentNormal[0]);
				normal[1] = static_cast<float>(currentNormal[1]);
				normal[2] = static_cast<float>(currentNormal[2]);
			}

			// Save the UV.
			if (hasUV)
			{
				int uvIndex = index;
				if (uvElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
				{
					uvIndex = uvElement->GetIndexArray().GetAt(index);
				}
				currentUV = uvElement->GetDirectArray().GetAt(uvIndex);
				uv[0] = static_cast<float>(currentUV[0]);
				uv[1] = static_cast<float>(currentUV[1]);
			}
			vertices[index] = { position, normal, uv };
		}
	}
	int controlPointsCount = mesh->GetControlPointsCount();
	std::vector<std::vector<unsigned int>> indexMap(controlPointsCount);
	std::vector<std::set<unsigned int>> indexMap2(controlPointsCount);

	//std::unordered_map<unsigned int, unsigned int> indexMap2;

	int vertexCount = 0;
	for (unsigned int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
	{
		// The material for current face.
		int materialIndex = 0;
		if (materialIndice && materialMappingMode == FbxGeometryElement::eByPolygon)
		{
			materialIndex = materialIndice->GetAt(polygonIndex);
		}

		// Where should I save the vertex attribute index, according to the material
		const int indexOffset = subMeshes[materialIndex]->IndexOffset + subMeshes[materialIndex]->TriangleCount * 3;
		for (int verticeIndex = 0; verticeIndex < TRIANGLE_VERTEX_COUNT; ++verticeIndex)
			//for (int verticeIndex = TRIANGLE_VERTEX_COUNT-1; verticeIndex >= 0; --verticeIndex)
		{
			const int controlPointIndex = mesh->GetPolygonVertex(polygonIndex, verticeIndex);
			if (allByControlPoint)
			{
				indices[indexOffset + verticeIndex] = static_cast<unsigned int>(controlPointIndex);
			}
			// Populate the array with vertex attribute, if by polygon vertex.
			else
			{
				indices[indexOffset + (TRIANGLE_VERTEX_COUNT - 1) - verticeIndex] = static_cast<unsigned int>(vertexCount);
				indexMap[controlPointIndex].push_back(static_cast<unsigned int>(vertexCount));
				Vector3 position;
				Vector3 normal;
				Vector2 uv;

				currentVertex = controlPoints[controlPointIndex];
				position[0] = static_cast<float>(-currentVertex[0]);
				position[1] = static_cast<float>(currentVertex[1]);
				position[2] = static_cast<float>(currentVertex[2]);

				Matrix4 vertexMatrix = Matrix4::Translation(position);
				Matrix4 cMatrix = meshMatrix * vertexMatrix;
				position = cMatrix.getTranslation();

				if (hasNormal)
				{
					mesh->GetPolygonVertexNormal(polygonIndex, verticeIndex, currentNormal);
					normal[0] = static_cast<float>(currentNormal[0]);
					normal[1] = static_cast<float>(currentNormal[1]);
					normal[2] = static_cast<float>(currentNormal[2]);
				}

				if (hasUV)
				{
					bool unmappedUV;
					mesh->GetPolygonVertexUV(polygonIndex, verticeIndex, uvName, currentUV, unmappedUV);
					uv[0] = static_cast<float>(currentUV[0]);
					uv[1] = static_cast<float>(currentUV[1]);
				}
				vertices[vertexCount] = { position, normal, uv };

				Vertex2 v2 = { position, normal, uv };
				if (verticesMap.find(v2) != verticesMap.end())
				{
					//indices[indexOffset + (TRIANGLE_VERTEX_COUNT - 1) - verticeIndex] = static_cast<unsigned int>(vertexCount);
					indices2[indexOffset + (TRIANGLE_VERTEX_COUNT - 1) - verticeIndex] = verticesMap[v2];
					indexMap2[controlPointIndex].insert(verticesMap[v2]);
				}
				else
				{
					unsigned int vertexIndex = static_cast<unsigned int>(vertices2.size());
					indices2[indexOffset + (TRIANGLE_VERTEX_COUNT - 1) - verticeIndex] = vertexIndex;
					indexMap2[controlPointIndex].insert(vertexIndex);
					vertices2.push_back(v2);
					verticesMap[v2] = vertexIndex;
				}
			}
			++vertexCount;
		}
		subMeshes[materialIndex]->TriangleCount += 1;
	}

	// Skinning
	int skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount > 0)
	{
		FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
		FbxSkin::EType skinningType = skin->GetSkinningType();

		if (!skin)
		{
			Log::warning("Error obtaining mesh skin");
			return nullptr;
		}

		int clusterCount = skin->GetClusterCount();
		if (clusterCount <= 0)
		{
			Log::warning("Skin has no bones");
			return nullptr;
		}

		struct Weight
		{
			int boneIndex = -1;
			float weight = 0.0f;

			static bool compare(const Weight& w1, const Weight& w2)
			{
				return w1.weight > w2.weight;
			}
		};




		std::unordered_map<unsigned int, std::vector<Weight>> weights;
		//std::vector<float> vertexIndeices(vertexCount);
		//std::vector<float> weights(vertexCount);
		if (skeletonRootNode)
		{
			std::unordered_map<std::string, int> boneIndexMap;
			recursiveIndexMapSkeleton(skeletonRootNode, boneIndexMap);
			for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
			{
				FbxCluster* cluster = skin->GetCluster(clusterIndex);
				FbxCluster::ELinkMode clusterMode = cluster->GetLinkMode();

				int vertexIndexCount = cluster->GetControlPointIndicesCount();
				for (int k = 0; k < vertexIndexCount; ++k)
				{
					int index = cluster->GetControlPointIndices()[k];
					if (index >= vertexCount)
						continue;

					float weight = static_cast<float>(cluster->GetControlPointWeights()[k]);

					if (weight == 0.0)
						continue;
					Log::info("");

					FbxNode* clusterLink = cluster->GetLink();
					std::string boneName = clusterLink->GetName();
					int boneIndex = boneIndexMap[boneName];

					if (weights.find(index) == weights.end())
					{
						weights[index] = {};
					}
					weights[index].push_back({ boneIndex, weight });
				}
			}
			for (auto& vertexWeights : weights)
			{
				int vI = vertexWeights.first;

				auto& vIndices = indexMap[vI];
				//int vertexIndex = indices[vII];
				//Vertex& vertex = vertices[vertexIndex];
				auto& boneWeights = vertexWeights.second;

				auto& vIndices2 = indexMap2[vI];


				//int vII2 = indexMap2[vI];
				//int vertexIndex2 = indices2[vII2];
				//Vertex2& vertex2 = vertices2[vertexIndex2];
				//Vector3& vertexPos = vertex.position;
				//Vector3& vertexPos2 = vertex2.position;

				std::sort(boneWeights.begin(), boneWeights.end(), Weight::compare);

				for (auto& idx : vIndices)
				{
					Vertex& v = vertices[idx];
					for (unsigned int i = 0; i < boneWeights.size() && i < 3; ++i)
					{
						v.ids[i] = (float)boneWeights[i].boneIndex;
						v.weights[i] = boneWeights[i].weight;
					}
					v.weights.normalize();
				}

				for (auto& idx : vIndices2)
				{
					Vertex2& v = vertices2[idx];
					for (unsigned int i = 0; i < boneWeights.size() && i < 3; ++i)
					{
						v.ids[i] = (float)boneWeights[i].boneIndex;
						v.weights[i] = boneWeights[i].weight;
					}
					v.weights.normalize();
				}

				//for (int i = 0; i < boneWeights.size() && i < 3; ++i)
				//{
					//vertex.ids[i] = boneWeights[i].boneIndex;
					//vertex.weights[i] = boneWeights[i].weight;

					//for (auto& idx : vIndices)
					//{
					//	Vertex& v = vertices[idx];
					//	v.ids[i] = boneWeights[i].boneIndex;
					//	v.weights[i] = boneWeights[i].weight;
					//}

					//for (auto& v : vertices2)
					//{
					//	if (v.position == vertexPos2)
					//	{
					//		v.ids[i] = boneWeights[i].boneIndex;
					//		v.weights[i] = boneWeights[i].weight;
					//	}
					//}
					/*vertex2.ids[i] = boneWeights[i].boneIndex;
					vertex2.weights[i] = boneWeights[i].weight;*/


					//}
					//vertex2.weights.normalize();
			}
			//Log::info("");
		}
	}

	Log::info("");

	// Material
	std::vector<Material>& materials = result->materials;
	FbxNode* meshNode = mesh->GetNode();
	int materialCount = meshNode->GetMaterialCount();
	if (materialCount > 0)
	{
		FbxSurfaceMaterial* surfaceMaterial = meshNode->GetMaterial(0);
		if (surfaceMaterial)
		{
			Material material;
			FbxProperty p = surfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (p.IsValid())
			{
				FbxDouble3 diffuseColor = p.Get<FbxDouble3>();
				material.diffuse.set((float)diffuseColor[0], (float)diffuseColor[1], (float)diffuseColor[2]);
			}
			p = surfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
			if (p.IsValid())
				FbxDouble diffuseIntensity = p.Get<FbxDouble>();

			p = surfaceMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
			if (p.IsValid())
			{
				FbxDouble3 specularColor = p.Get<FbxDouble3>();
				material.specular.set((float)specularColor[0], (float)specularColor[1], (float)specularColor[2]);
			}
			p = surfaceMaterial->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
			if (p.IsValid())
			{
				FbxDouble specularIntensity = p.Get<FbxDouble>();
				material.specularIntensity = (float)specularIntensity;
			}

			for (int layerIndex = 0; layerIndex < FbxLayerElement::sTypeTextureCount; ++layerIndex)
			{
				FbxProperty materialProperty = surfaceMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[layerIndex]);
				if (materialProperty.IsValid())
				{
					int textureCount = materialProperty.GetSrcObjectCount<FbxTexture>();
					for (int textureIndex = 0; textureIndex < textureCount; ++textureIndex)
					{
						FbxTexture* texture = materialProperty.GetSrcObject<FbxTexture>(textureIndex);
						if (texture)
						{
							std::string propertyName = materialProperty.GetName().Buffer();
							FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);
							std::string textureName = texture->GetName();

							if (fileTexture)
							{
								if (propertyName == "DiffuseColor")
								{
									std::string texturePath = fileTexture->GetFileName();
									Texture* texture = ResourceManager::getInstance().getTexture(texturePath, "texture_diffuse");
									material.diffuseTexture = texture;
									material.useDiffuseTexture = true;

								}
								else if (propertyName == "SpecularColor")
								{
									std::string texturePath = fileTexture->GetFileName();
									Texture* texture = ResourceManager::getInstance().getTexture(texturePath, "texture_specular");
									material.specularTexture = texture;
									material.useSpecularTexture = true;
								}
							}
						}
					}
				}
			}
			materials.push_back(material);
		}
	}

	// Load VAO
	result->vertexArray = ORB::VertexArray::Create();
	//result->vertexBuffer = new VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), vertices.size());
	//result->indexBuffer = new IndexBuffer(indices.data(), indices.size());
	result->vertexBuffer = ORB::VertexBuffer::Create(vertices2.data(), vertices2.size() * sizeof(Vertex2), vertices2.size());
	result->indexBuffer = ORB::IndexBuffer::Create(indices2.data(), indices2.size());

	ORB::VertexBufferLayout layout = {
		{"aPos", ORB::AttributeType::FLOAT3},
		{"aNormal", ORB::AttributeType::FLOAT3},
		{"aTexCoord", ORB::AttributeType::FLOAT2},
		{"aIndices", ORB::AttributeType::FLOAT3},
		{"aWeights", ORB::AttributeType::FLOAT3},
	};

	result->vertexArray->addVertexBuffer(result->vertexBuffer, layout);
	result->vertexArray->setIndexBuffer(result->indexBuffer);

	return result;
}



void FBXLoader::loadModel(const std::string& filePath, MeshData*& meshData, BoneData*& boneData, AnimationData*& animationData)
{
	FbxScene* scene;
	std::vector<FbxMesh*> meshes;
	std::vector<FbxNode*> skeletons;
	if (parseFile(filePath, scene, meshes, skeletons))
	{
		FbxMesh* fbxMesh = nullptr;
		FbxNode* fbxSkeletonNode = nullptr;
		if (meshes.size() > 0)
			fbxMesh = meshes[0];
		if (skeletons.size() > 0)
			fbxSkeletonNode = skeletons[0];

		Matrix4 meshMatrix;
		if (fbxMesh)
		{
			meshMatrix = getFbxNodeTransformMatrix(fbxMesh->GetNode());
		}
		if (fbxSkeletonNode)
		{
			boneData = getBones(scene, fbxMesh, fbxSkeletonNode);


			animationData = getAnimations(scene, fbxSkeletonNode, boneData, meshMatrix);
		}
		if (fbxMesh)
		{
			meshData = getMesh(scene, fbxMesh, fbxSkeletonNode, meshMatrix);
			Log::info("");
		}
	}
	else
	{
		Log::error("Load model failed");
	}
}
