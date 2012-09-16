#pragma once

#include <string>

class Configuration
{
public:

	void serialize(const std::string& filename);
	void deserialize(const std::string& filename);

	long double x,y;
	long double zoom;
	unsigned int resolution;

private:
};