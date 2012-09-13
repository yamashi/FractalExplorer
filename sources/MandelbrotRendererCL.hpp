/*
 *  MandelbrotRendererCL.hpp
 *	Mandelbrot Fractal Explorer Project - Copyright (c) 2012 Lucas Soltic & Maxime Griot
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held
 *  liable for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute
 *  it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment
 *  in the product documentation would be appreciated but
 *  is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any
 *  source distribution.
 *
 */

#include "Common.hpp"
#ifdef CL_BUILD

#ifndef MANDELBROT_RENDERER_CL_HPP
#define MANDELBROT_RENDERER_CL_HPP

#include <SFML/System/Vector2.hpp>
#include "OpenCLDevice.h"
#include "epgpu.h"

class MandelbrotRendererCL {
	unsigned char *m_pixelBuffer;
	unsigned m_pixelBufferWidth;
	unsigned m_pixelBufferHeigth;

	gpu_vector2d<unsigned char> m_img;
	
public:
	MandelbrotRendererCL(OpenCLDevice* device, unsigned char *pixelBuffer, unsigned width, unsigned heigth);
	
	void operator()(double zoom, int resolution, const Vector2lf& normalizedPosition);
};

#endif
#endif
