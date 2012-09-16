#include <fstream>

#include "Configuration.hpp"


void Configuration::serialize(const std::string& filename)
{
	unsigned int version = 1;
	std::ofstream file(filename.c_str(), std::ios::trunc | std::ios::binary);
	file.write((char*)&version, 4);
	file.write((char*)&x, sizeof(x));
	file.write((char*)&y, sizeof(y));
	file.write((char*)&zoom, sizeof(zoom));
	file.write((char*)&resolution, sizeof(resolution));
	file.close();
}

void Configuration::deserialize(const std::string& filename)
{
	unsigned int version = 0;
	std::ifstream file(filename.c_str(), std::ios::binary);
	file.read((char*)&version, 4);
	file.read((char*)&x, sizeof(x));
	file.read((char*)&y, sizeof(y));
	file.read((char*)&zoom, sizeof(zoom));
	file.read((char*)&resolution, sizeof(resolution));
	file.close();
}