#include "Mesh.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Transform.h"
#include "Camera.h"
#include "World.h"
#include "Texture.h"
#include "Log.h"
#include "Material.h"

OBJInfo Mesh::ParseOBJFile(const std::string& filePath) const
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
					result.indices.push_back(ParseOBJIndex(tokens[1]));
					result.indices.push_back(ParseOBJIndex(tokens[2 + i]));
					result.indices.push_back(ParseOBJIndex(tokens[3 + i]));
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

OBJIndex Mesh::ParseOBJIndex(const std::string& index) const
{
	std::vector<unsigned int> tokens;
	splitString(index, '/', tokens);

	// The indices on the file start in 1, so we have to substract 1
	return { tokens[0] - 1, tokens[1] - 1, tokens[2] - 1 };
}

MTLInfo Mesh::ParseMTLFile(const std::string& filePath) const
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

void Mesh::CreateMesh(const OBJInfo& objInfo)
{
	std::vector<Vertex> vertices;

	const auto& indices = objInfo.indices;
	for (unsigned int i = 0; i < indices.size(); ++i)
	{
		const OBJIndex& index = indices[i];
		vertices.push_back
		(
			{
				objInfo.positions[index.position],
				objInfo.normals[index.normal],
				objInfo.texCoords[index.texCoord],
			}
		);
	}

	vertexArray = new VertexArray();
	vertexBuffer = new VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), vertices.size());
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);

	vertexArray->AddBuffer(vertexBuffer, layout);
}

void Mesh::LoadTextures(MTLInfo& info)
{
	for (unsigned int i = 0; i < info.materials.size(); ++i)
	{
		Material* material = new Material(info.materials[i]->diffuse, info.materials[i]->specular);
		materials.push_back(material);

		std::vector<TextureInfo*> texturesInfo = info.materials[i]->textures;

		for (unsigned int j = 0; j < texturesInfo.size(); ++j)
		{
			Texture* texture = new Texture(info.pathInfo.directory + texturesInfo[j]->name, texturesInfo[j]->type);

			if (texturesInfo[j]->type == "texture_diffuse")
			{
				material->diffuseTexture = texture;
				material->useDiffuseTexture = true;
			}
			else if (texturesInfo[j]->type == "texture_specular")
			{
				material->specularTexture = texture;
				material->useSpecularTexture = true;
			}
		}
	}
}

Mesh::Mesh(const std::string& filePath)
{
	OBJInfo info = ParseOBJFile(filePath);

	setName(info.name);

	CreateMesh(info);

	if (!info.mtl.empty())
	{
		std::string mtlFilePath = info.pathInfo.directory + info.mtl;
		MTLInfo mtlInfo = ParseMTLFile(mtlFilePath);
		LoadTextures(mtlInfo);
	}
}

Mesh::~Mesh()
{
	if (vertexBuffer != nullptr)
	{
		delete vertexBuffer;
	}
	if (vertexArray != nullptr)
	{
		delete vertexArray;
	}
}

void Mesh::render(float deltaTime, Shader* shader)
{
	shader->bind();
	shader->SetMatrix("model", getTransform()->getMatrix());
	shader->SetMatrix("view", World::getInstance().getActiveCamera()->getViewMatrix());
	shader->SetMatrix("projection", World::getInstance().getActiveCamera()->getProjectionMatrix());

	if (materials.size() > 0)
	{
		shader->SetBool("material.useDiffuseTexture", materials[0]->useDiffuseTexture);
		shader->SetBool("material.useSpecularTexture", materials[0]->useSpecularTexture);

		if (materials[0]->diffuseTexture != nullptr && materials[0]->useDiffuseTexture)
		{
			shader->SetInt("material.texture_diffuse", 0);
			materials[0]->diffuseTexture->bind(0);
		}
		else
		{
			shader->SetFloat3("material.diffuse", materials[0]->diffuse);
		}

		if (materials[0]->specularTexture != nullptr && materials[0]->useSpecularTexture)
		{
			shader->SetInt("material.texture_specular", 1);
			materials[0]->specularTexture->bind(1);
		}
		else
		{
			shader->SetFloat3("material.specular", materials[0]->specular);
		}


		shader->SetFloat("material.specularIntensity", materials[0]->specularIntensity);
		shader->SetFloat("material.specularPower", materials[0]->specularPower);
	}
	glActiveTexture(0);

	vertexArray->draw(shader);
}
