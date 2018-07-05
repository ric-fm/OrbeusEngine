#pragma once

#include <string>
#include <sstream>
#include <vector>

struct PathInfo
{
	std::string directory;
	std::string fileName;
	std::string extension;
};

static void splitString(const std::string& source, char token, std::vector<std::string>& result)
{
	std::istringstream sourceStream(source);
	std::string element;
	result.clear();
	while (getline(sourceStream, element, token))
	{
		result.push_back(element);
	}
}

static void splitString(const std::string& source, char token, std::vector<unsigned int>& result)
{
	std::istringstream sourceStream(source);
	std::string element;
	result.clear();
	while (getline(sourceStream, element, token))
	{
		result.push_back(std::atoi(element.c_str()));
	}
}

static PathInfo getPathInfo(const std::string& path)
{
	PathInfo result;

	std::vector<std::string> tokens;
	splitString(path, '/', tokens);

	for (unsigned int i = 0; i < tokens.size() - 1; ++i)
	{
		result.directory += tokens[i] + "/";
	}

	splitString(tokens[tokens.size() - 1], '.', tokens);
	result.fileName = tokens[0];
	result.extension = tokens[1];

	return result;
}