#include "MeshLoader.h"

#include <fstream>
#include <sstream>

#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/Material.h"
#include "Rendering/Mesh\MeshData.h"

#include "Utils/Log.h"

OBJIndex parseOBJIndex(const std::string& index)
{
	std::vector<unsigned int> tokens;
	splitString(index, '/', tokens);

	// The indices on the file start in 1, so we have to substract 1
	return { tokens[0] - 1, tokens[1] - 1, tokens[2] - 1 };
}


OBJInfo parseOBJFile(const std::string& filePath)
{
	OBJInfo result;

	std::ifstream file(filePath.c_str());

	if (file.is_open())
	{
		result.pathInfo = getPathInfo(filePath);

		std::string line;
		while (getline(file, line))
		{
			std::istringstream lineStream(line);
			std::string element;
			lineStream >> element;

			if (element == "o")
			{
				lineStream >> result.name;
			}
			else if (element == "v")
			{
				Vector3 position;
				for (unsigned int i = 0; i < 3; ++i)
				{
					lineStream >> position[i];
				}
				result.positions.push_back(position);
			}
			else if (element == "vt")
			{
				Vector2 texCoord;
				for (unsigned int i = 0; i < 2; ++i)
				{
					lineStream >> texCoord[i];
				}
				result.texCoords.push_back(texCoord);
			}
			else if (element == "vn")
			{
				Vector3 normal;
				for (unsigned int i = 0; i < 3; ++i)
				{
					lineStream >> normal[i];
				}
				result.normals.push_back(normal);
			}
			// TODO: Handle load if there is no TexCoordIndex or NormalIndex
			else if (element == "f") // Face: PositionIndex0/TexCoordIndex0/NormalIndex0 PositionIndex1/TexCoordIndex1/NormalIndex1 PositionIndex2/TexCoordIndex2/NormalIndex2
			{
				std::vector<std::string> tokens;
				splitString(lineStream.str(), ' ', tokens);

				// If tokens.size() == 4, the face has 3 indices so it is triangulated
				// If tokens.size() == 5, the face has 4 indices so we have to triangulated it
				for (unsigned int i = 0; i < tokens.size() - 3; ++i)
				{
					result.indices.push_back(parseOBJIndex(tokens[1]));
					result.indices.push_back(parseOBJIndex(tokens[2 + i]));
					result.indices.push_back(parseOBJIndex(tokens[3 + i]));
				}
			}
			else if (element == "mtllib")
			{
				lineStream >> result.mtl;
			}
		}
	}
	else
	{
		Log::error("MESH. File %s doesn't exists", filePath.c_str());
	}

	file.close();

	return result;
}

MTLInfo parseMTLFile(const std::string& filePath)
{
	MTLInfo result;
	result.pathInfo = getPathInfo(filePath);

	std::ifstream file(filePath.c_str());

	if (file.is_open())
	{
		MaterialInfo* material = nullptr;

		std::string line;
		while (getline(file, line))
		{
			std::istringstream lineStream(line);
			std::string element;

			lineStream >> element;

			if (element == "newmtl")
			{
				material = new MaterialInfo();
				lineStream >> material->name;

				result.materials.push_back(material);

			}
			else if (element == "Kd")
			{
				for (unsigned int i = 0; i < 3; ++i)
				{
					lineStream >> material->diffuse[i];
				}
			}
			else if (element == "Ks")
			{
				for (unsigned int i = 0; i < 3; ++i)
				{
					lineStream >> material->specular[i];
				}
			}
			else if (element.substr(0, element.find_last_of('_')) == "map")
			{
				if (material != nullptr)
				{
					std::string type;
					bool isTextureSupported = true;
					if (element == "map_Kd")
					{
						type = "texture_diffuse";
					}
					else if (element == "map_Ks")
					{
						type = "texture_specular";
					}
					else
					{
						isTextureSupported = false;
					}

					lineStream >> element;
					if (isTextureSupported)
					{
						material->textures.push_back(new TextureInfo({ element, type }));
					}
				}
			}
		}
	}
	else
	{
		Log::error("MESH. File %s doesn't exists", filePath.c_str());
	}

	file.close();

	return result;
}

void generateVertices(const OBJInfo& info, std::vector<Vertex>& vertices)
{
	vertices.clear();
	const auto& indices = info.indices;
	for (unsigned int i = 0; i < indices.size(); ++i)
	{
		const OBJIndex& index = indices[i];
		vertices.push_back
		(
			{
				info.positions[index.position],
				info.normals[index.normal],
				info.texCoords[index.texCoord],
			}
		);
	}
}

void generateMaterials(MTLInfo info, std::vector<Material>& materials)
{
	for (unsigned int i = 0; i < info.materials.size(); ++i)
	{
		Material material(info.materials[i]->diffuse, info.materials[i]->specular);

		std::vector<TextureInfo*> texturesInfo = info.materials[i]->textures;

		for (unsigned int j = 0; j < texturesInfo.size(); ++j)
		{
			Texture* texture = new Texture(info.pathInfo.directory + texturesInfo[j]->name, texturesInfo[j]->type);

			if (texturesInfo[j]->type == "texture_diffuse")
			{
				material.diffuseTexture = texture;
				material.useDiffuseTexture = true;
			}
			else if (texturesInfo[j]->type == "texture_specular")
			{
				material.specularTexture = texture;
				material.useSpecularTexture = true;
			}
		}
		materials.push_back(material);
	}
}

MeshData* MeshLoader::loadMesh(const std::string& filePath)
{
	MeshData* result = new MeshData();

	// Load mesh data
	OBJInfo objInfo = parseOBJFile(filePath);

	// Generate vertices
	std::vector<Vertex> vertices;
	generateVertices(objInfo, vertices);

	// Load VAO
	result->vertexArray = new VertexArray();
	result->vertexBuffer = new VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), vertices.size());
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);

	result->vertexArray->AddBuffer(result->vertexBuffer, layout);

	// Load materials
	if (!objInfo.mtl.empty())
	{
		std::string mtlFilePath = objInfo.pathInfo.directory + objInfo.mtl;
		MTLInfo mtlInfo = parseMTLFile(mtlFilePath);

		generateMaterials(mtlInfo, result->materials);
	}

	return result;
}
