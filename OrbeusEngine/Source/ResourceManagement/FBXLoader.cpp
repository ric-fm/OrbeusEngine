#include "FBXLoader.h"

#include "Math/Math.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/Material.h"
#include "Rendering/Mesh/MeshData.h"
#include "Rendering/SkeletalMesh/Bone.h"
#include "Rendering/SkeletalMesh/Animation.h"
#include "ResourceManagement/ResourceManager.h"

#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"

#include "Utils/String.h"
#include "Utils/Log.h"

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
		std::vector<Bone*> bones;
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
				FbxMatrix boneMatrix = pose->GetMatrix(poseNodeIndex);
				//Matrix4 transform = Matrix4(
				//	boneMatrix.Get(0, 0),
				//	boneMatrix.Get(0, 1),
				//	boneMatrix.Get(0, 2),
				//	boneMatrix.Get(0, 3),

				//	boneMatrix.Get(1, 0),
				//	boneMatrix.Get(1, 1),
				//	boneMatrix.Get(1, 2),
				//	boneMatrix.Get(1, 3),

				//	boneMatrix.Get(2, 0),
				//	boneMatrix.Get(2, 1),
				//	boneMatrix.Get(2, 2),
				//	boneMatrix.Get(2, 3),

				//	boneMatrix.Get(3, 0),
				//	boneMatrix.Get(3, 1),
				//	boneMatrix.Get(3, 2),
				//	boneMatrix.Get(3, 3)
				//);
				//targetBone->transformMatrix = transform;
				double sign;
				FbxVector4 scale;
				FbxVector4 shearing;
				FbxQuaternion rotation;
				FbxVector4 translation;
				boneMatrix.GetElements(translation, rotation, shearing, scale, sign);

				Vector3 pos;
				Quaternion rot;
				pos.set(translation.mData[0], translation.mData[1], translation.mData[2]);
				rot.set(rotation.mData[0], rotation.mData[1], rotation.mData[2], rotation.mData[3]);

				targetBone->transformMatrix = Matrix4::Translation(pos) * Matrix4::Rotation(rot);
			}
		}
	}
	if (rootBone)
	{
		result = new BoneData();
		result->rootBone = rootBone;
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
	for (int i = 1; i < times.size(); ++i)
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
	for (int childIndex = 0; childIndex < animData->children.size(); ++childIndex)
	{
		recursiveGetMinMaxTimes(animData->children[childIndex], minTime, maxTime);
	}
}

void getAnimationTimes(NodeAnimData* animData, FbxTime& startTime, FbxTime& endTime, FbxTime& length)
{
	recursiveGetMinMaxTimes(animData, startTime, endTime);
	length = endTime - startTime;
}

void recursiveGetKeys(NodeAnimData* animData, FbxTime time, std::vector<Key*>& keys)
{
	ComponentCurve* posCurve = animData->keyCurve->posCurve;
	ComponentCurve* rotCurve = animData->keyCurve->rotCurve;
	Vector3 position(0, 0, 0);
	Vector3 rotation(0, 0, 0);
	if (posCurve->x != nullptr)
	{
		position.x = posCurve->x->Evaluate(time);
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
	keys.push_back(key);

	for (int childIndex = 0; childIndex < animData->children.size(); ++childIndex)
	{
		recursiveGetKeys(animData->children[childIndex], time, keys);
	}
}

AnimationData* getAnimations(FbxScene* scene, FbxNode* skeletonRootNode, BoneData* boneData)
{
	AnimationData* result;

	int animStackCount = scene->GetMemberCount<FbxAnimStack>();
	if (animStackCount == 0)
	{
		Log::warning("Mesh has no animations");
		return nullptr;
	}

	result = new AnimationData();

	// Get map of FbxNodes by name
	//std::unordered_map<std::string, FbxNode*> boneMap;
	//recursiveMapSkeleton(skeletonRootNode, boneMap);

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

			FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>();
			NodeAnimData* rootWorkingNode = nullptr;
			recursiveGetAnimData(scene, skeletonRootNode, animStack, animLayer, rootWorkingNode);

			FbxTime startTime;
			FbxTime endTime;
			FbxTime length;
			getAnimationTimes(rootWorkingNode, startTime, endTime, length);

			float keyFrameStart = startTime.GetSecondDouble();
			float keyFrameEnd = endTime.GetSecondDouble();
			float keyFrameLength = length.GetSecondDouble();
			animation->length = keyFrameLength;// * 1000.0f; //TODO: Mejor en milisegundos o segundos?

			int framesPerSeconds = 24;
			int numFrames = ceil(keyFrameLength * framesPerSeconds);
			float samplingInterval = keyFrameLength / numFrames;

			for (float t = keyFrameStart; t < keyFrameEnd; t += samplingInterval)
			{
				KeyFrame* keyFrame = new KeyFrame();
				keyFrame->timeStamp = t;
				FbxTime currentTime;
				currentTime.SetSecondDouble(t);

				recursiveGetKeys(rootWorkingNode, currentTime, keyFrame->keys);
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

MeshData* FBXLoader::getMesh(FbxScene* scene, FbxMesh* mesh, FbxNode* skeletonRootNode)
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

	unsigned int polyCount = mesh->GetPolygonCount();
	unsigned int verticesCount = mesh->GetControlPointsCount();
	std::vector<Vertex> vertices;
	vertices.reserve(verticesCount);
	std::vector<unsigned int> indices;
	indices.reserve(polyCount * 3);


	// HashMap of ObjectIndex to detect Indexing Vertex Duplication
	std::unordered_map<unsigned int, unsigned int> processedVertices;

	const FbxVector4* controlPoints = mesh->GetControlPoints();
	int vertexCount = 0;
	for (int polygonIndex = 0; polygonIndex < polyCount; ++polygonIndex)
	{
		for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
		{
			const int controlPointIndex = mesh->GetPolygonVertex(polygonIndex, vertexIndex);

			if (processedVertices.find(controlPointIndex) != processedVertices.end())
			{
				indices.push_back(processedVertices[controlPointIndex]);
			}
			else
			{
				processedVertices[controlPointIndex] = vertexCount;
				indices.push_back(vertexCount);

				FbxVector4 currentVertex;
				FbxVector4 currentNormal;
				FbxVector2 currentUV;

				currentVertex = controlPoints[controlPointIndex];

				mesh->GetPolygonVertexNormal(polygonIndex, vertexIndex, currentNormal);

				const char* uvName = NULL;
				bool unmapped;
				mesh->GetPolygonVertexUV(polygonIndex, vertexIndex, uvName, currentUV, unmapped);

				vertices.push_back(
					{
						Vector3(currentVertex[0], currentVertex[1], currentVertex[2]),
						Vector3(currentNormal[0], currentNormal[1], currentNormal[2]),
						Vector2(currentUV[0], currentUV[1]),
						Vector3(-1, -1, -1), // Initialize ids to -1. To used before in the skinning process
						Vector3() // Initialize weigths to 0
					}
				);

				++vertexCount;
			}
		}
	}

	// Skinning
	int skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount <= 0)
	{
		Log::warning("Mesh has no skin");
		return nullptr; // TODO: Por el momento devuelvo null si no tiene skinning. Gestionar esto para cargar mallas sin esqueleto
	}

	FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
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

	std::unordered_map<std::string, int> boneIndexMap;
	recursiveIndexMapSkeleton(skeletonRootNode, boneIndexMap);

	for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
	{
		FbxCluster* cluster = skin->GetCluster(clusterIndex);
		FbxNode* clusterLink = cluster->GetLink();
		std::string boneName = clusterLink->GetName();
		int boneIndex = boneIndexMap[boneName];
		if (!clusterLink)
		{
			Log::warning("cluster has no link");
			continue;
		}

		int pointCount = cluster->GetControlPointIndicesCount();
		if (pointCount == 0)
		{
			Log::warning("cluster has 0 point indices");
			continue;
		}

		const int* pointIndices = cluster->GetControlPointIndices();
		const double* pointWeights = cluster->GetControlPointWeights();

		for (int pointIndex = 0; pointIndex < pointCount; ++pointIndex)
		{
			int controlPointIndex = pointIndices[pointIndex];
			float weight = pointWeights[pointIndex];

			if (processedVertices.find(controlPointIndex) != processedVertices.end())
			{
				int vertexIndex = processedVertices[controlPointIndex];
				Vertex& vertex = vertices[vertexIndex];

				Vector3& ids = vertex.ids;
				Vector3& weights = vertex.weights;
				// TODO: Ordenar por peso y quedarme con los mas relevantes
				for (int i = 0; i < 3; ++i)
				{
					if (ids[i] < 0)
					{
						ids[i] = boneIndex;
						weights[i] = weight;
						break;
					}
				}
			}
		}
	}

	// Check vertices bone ids an set to 0 for correct use in shader
	// TODO: Mirar si realmente es necesario
	for (int i = 0; i < vertices.size(); ++i)
	{
		Vector3& idx = vertices[i].ids;
		for (int j = 2; j >= 0; -j)
		{
			if (idx[j] >= 0)
				break;
			idx[j] = 0;
		}
	}

	// Load VAO
	result->vertexArray = new VertexArray();
	result->vertexBuffer = new VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), vertices.size());
	result->indexBuffer = new IndexBuffer(indices.data(), indices.size());

	VertexBufferLayout layout;
	layout.Push<float>(3); // pos
	layout.Push<float>(3); // normal
	layout.Push<float>(2); // uv
	layout.Push<float>(3); // ids
	layout.Push<float>(3); // weights

	result->vertexArray->AddBuffer(result->vertexBuffer, layout);

	return result;
}

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

void FBXLoader::loadModel(const std::string& filePath, MeshData*& meshData, BoneData*& boneData, AnimationData*& animationData)
{
	FbxScene* scene;
	std::vector<FbxMesh*> meshes;
	std::vector<FbxNode*> skeletons;
	if (parseFile(filePath, scene, meshes, skeletons))
	{
		if (skeletons.size() > 0)
		{
			boneData = getBones(scene, meshes[0], skeletons[0]);
			animationData = getAnimations(scene, skeletons[0], boneData);
		}
		if (meshes.size() > 0)
		{
			meshData = getMesh(scene, meshes[0], skeletons[0]/*, boneData*/);
		}
	}
	else
	{
		Log::error("Load model failed");
	}
}
