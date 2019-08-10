#pragma once

#include <string>
#include <sstream>
#include <vector>

#include <stdio.h>
#include <stdarg.h>

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

static void splitString(const std::string& source, char token, std::vector<int>& result)
{
	std::istringstream sourceStream(source);
	std::string element;
	result.clear();
	while (getline(sourceStream, element, token))
	{
		result.push_back(std::atoi(element.c_str()));
	}
}

static void splitString(const std::string& source, char token, std::vector<float>& result)
{
	std::istringstream sourceStream(source);
	std::string element;
	result.clear();
	while (getline(sourceStream, element, token))
	{
		result.push_back(std::stof(element.c_str()));
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

static std::string formatString(const char* format, ...)
{
	std::string result;
	va_list args;
	va_start(args, format);

	int length = vsnprintf(NULL, 0, format, args) + 1;
	char* buffer = new char[length];
	length = vsnprintf(buffer, length, format, args);

	result = std::string(buffer);

	delete(buffer);

	va_end(args);

	return result;
}
