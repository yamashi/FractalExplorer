
/*
 *  MandelbrotRendererCL.cpp
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

#include "MandelbrotRendererCL.hpp"
#include <iostream>
#include <SFML/System.hpp>

GPU_FILLKERNEL_2D(unsigned char,
	mandelbrot,(double sz, double zoom, double xoff,double yoff, int resolution),

	double fw = w * sz;
	double fh = h * sz;

	double2 c=(double2)(
		(fw*xoff - w/2 + i)/zoom - 2.1,
		(fh*yoff - h/2 + j)/zoom - 1.2
	);
	double2 z=c;

	int count;
	for (count=0;count<resolution;count++)
	{
		if ((z.x*z.x+z.y*z.y)>4.0f) 
			break;
		z=(double2)(
				z.x*z.x-z.y*z.y + c.x,
				2.0f*z.x*z.y + c.y
			);
	}

	result=count;
)

MandelbrotRendererCL::MandelbrotRendererCL(OpenCLDevice* device, unsigned char *pixelBuffer, unsigned width, unsigned heigth):
m_pixelBuffer(pixelBuffer),
m_pixelBufferWidth(width),
m_pixelBufferHeigth(heigth),
m_img(width, heigth)
{
}


void MandelbrotRendererCL::operator()(double zoom, int resolution,const Vector2lf& normalizedPosition)
{
	m_img = mandelbrot(zoom, zoom * m_pixelBufferHeigth / (2.4),normalizedPosition.x,normalizedPosition.y, resolution);
	unsigned char* ca = new unsigned char[m_pixelBufferWidth*m_pixelBufferHeigth];
	m_img.read(ca);
	for(unsigned x = 0; x < m_pixelBufferWidth;++x)
		for(unsigned y = 0; y < m_pixelBufferHeigth;++y)
		{
			if (ca[y * m_pixelBufferWidth + x] == resolution)
			{
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 0] = 0;
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 1] = 54;
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 2] = 76;
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 3] = 255;
			}
			else
			{
				int val = ca[y * m_pixelBufferWidth + x] * 255 / resolution;
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 0] = val;
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 1] = 0;
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 2] = 0;
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 3] = 255;
			}
		}

	delete[] ca;
}

#endif