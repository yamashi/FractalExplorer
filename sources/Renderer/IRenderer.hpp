#pragma once

#include "../Real/mpfreal.hpp"

class IRenderer
{
public:
	virtual void render(unsigned char *pixelBuffer, unsigned width, unsigned heigth,
		mpfreal& zoom, int resolution, mpfreal& x, mpfreal& y) = 0;
};