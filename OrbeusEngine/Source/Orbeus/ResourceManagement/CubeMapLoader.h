#pragma once

#include <string>


class CubeMap;

class CubeMapLoader
{
public:
	static CubeMap* generateCubeMap(const std::string& path, float size = 1.0f);

};
