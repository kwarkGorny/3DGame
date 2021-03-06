#include "FileUtils.hpp"

#include "basic/Logger.hpp"

#include <fstream>

std::string loadFile(const std::string& filePath)
{
	std::ifstream t(filePath);
	if (!t.is_open())
	{
		logger::error("such file dont exist " + filePath );
		return "";
	}
	t.seekg(0, std::ios::end);
	const auto size = t.tellg();
	t.seekg(0);
	std::string file(size, ' ');
	t.read(&file[0], size);
	return file;
}
