#include "Mesh.h"

#include "ResourceManagement/ResourceManager.h"
#include "Rendering/Mesh/MeshData.h"


Mesh::Mesh(const std::string& meshFilePath)
	: meshFilePath(meshFilePath)
{
	meshData = ResourceManager::getInstance().getMesh(meshFilePath);
}

Mesh::~Mesh()
{
}

void Mesh::setMesh(const std::string & newMeshFilePath)
{
	if (meshFilePath != newMeshFilePath)
	{
		meshFilePath = newMeshFilePath;
		if (meshData != nullptr)
		{
			delete meshData;
		}
		meshData = ResourceManager::getInstance().getMesh(meshFilePath);
	}
}
