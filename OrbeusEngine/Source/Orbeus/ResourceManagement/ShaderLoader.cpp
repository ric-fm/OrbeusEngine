#include "ShaderLoader.h"

#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include "Orbeus/Utils/Log.h"

std::string readShaderFile(const std::string& filePath)
{
	std::string result;

	std::ifstream file(filePath.c_str());
	std::string line;
	if (file.is_open())
	{
		while (getline(file, line))
		{
			std::istringstream lineStream(line);
			std::string element;
			lineStream >> element;

			if (element == "#include")
			{
				lineStream >> element;

				std::string includePath = "Resources/Shaders/" + element;

				std::ifstream file2(includePath.c_str());
				if (file2.is_open())
				{
					while (getline(file2, line))
					{
						result += line + '\n';
					}
				}
				file2.close();
			}
			else
			{
				result += line + '\n';
			}
		}

		file.close();
	}
	else
	{
		Log::error("SHADER. File %s doesn't exists", filePath.c_str());

	}

	return result;
}

Shader* ShaderLoader::loadShader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
	Shader* result = new Shader(vertexFilePath, fragmentFilePath);

	std::string vertexSource = readShaderFile(vertexFilePath);
	std::string fragmentSource = readShaderFile(fragmentFilePath);

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vsSrc = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &vsSrc, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		Log::error("VERTEX COMPILATION FAILED: %s", infoLog);
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fsSrc = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fsSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		Log::error("FRAGMENT COMPILATION FAILED: %s", infoLog);
	}

	result->ID = glCreateProgram();

	glAttachShader(result->ID, vertexShader);
	glAttachShader(result->ID, fragmentShader);
	glLinkProgram(result->ID);


	glGetProgramiv(result->ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(result->ID, 512, NULL, infoLog);
		Log::error("SHADER LINKING FAILED: %s", infoLog);

	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	registerAttributesAndUniforms(result);

	return result;
}

void ShaderLoader::registerAttributesAndUniforms(Shader* shader)
{
	//@NOTE glGetProgramResource is for OpenGL 4.3+
	GLint numActiveAttribs = 0;
	GLint numActiveUniforms = 0;
	glGetProgramInterfaceiv(shader->ID, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);
	glGetProgramInterfaceiv(shader->ID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

	std::vector<GLchar> nameData(256);
	std::vector<GLenum> properties;
	properties.push_back(GL_NAME_LENGTH);
	properties.push_back(GL_TYPE);
	properties.push_back(GL_LOCATION);
	properties.push_back(GL_REFERENCED_BY_VERTEX_SHADER);
	properties.push_back(GL_REFERENCED_BY_FRAGMENT_SHADER);
	std::vector<GLint> values(properties.size());
	for (int attrib = 0; attrib < numActiveAttribs; ++attrib)
	{
		glGetProgramResourceiv(shader->ID, GL_PROGRAM_INPUT, attrib, properties.size(),
			&properties[0], values.size(), NULL, &values[0]);

		nameData.resize(values[0]); //The length of the name.
		glGetProgramResourceName(shader->ID, GL_PROGRAM_INPUT, attrib, nameData.size(), NULL, &nameData[0]);
		std::string name((char*)& nameData[0], nameData.size() - 1);
		shader->registerAttribute(name, GLIntToGLDataType(values[1]), values[2]);
	}

	for (int unif = 0; unif < numActiveUniforms; ++unif)
	{
		glGetProgramResourceiv(shader->ID, GL_UNIFORM, unif, properties.size(),
			&properties[0], values.size(), NULL, &values[0]);

		nameData.resize(values[0]); //The length of the name.
		glGetProgramResourceName(shader->ID, GL_UNIFORM, unif, nameData.size(), NULL, &nameData[0]);

		std::string name((char*)& nameData[0], nameData.size() - 1);
		GLStageType stage(values[3] ? GLStageType::VERTEX : values[4] ? GLStageType::PIXEL : GLStageType::NONE);
		shader->registerUniform(name, GLIntToGLDataType(values[1]), stage, values[2]);
	}
}
