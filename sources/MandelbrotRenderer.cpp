
/*
 *  MandelbrotRenderer.cpp
 *	Mandelbrot Fractal Explorer Project - Copyright (c) 2012 Lucas Soltic
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
 * Altered by Maxime Griot
 */

#include "Common.hpp"

#ifdef OMP_BUILD

#include "MandelbrotRenderer.hpp"
#include <iostream>
#include <SFML/System.hpp>

MandelbrotRenderer::MandelbrotRenderer(unsigned char *pixelBuffer, unsigned width, unsigned heigth,
									   double zoom, int resolution, const Vector2lf& normalizedPosition):
m_pixelBuffer(pixelBuffer),
m_pixelBufferWidth(width),
m_pixelBufferHeigth(heigth),
m_zoom(zoom),
m_resolution(resolution),
m_normalizedPosition(normalizedPosition)
{
}

struct ldouble2
{
	long double x;
	long double y;
};

void MandelbrotRenderer::operator()() const
{
	const long double fractal_left = -2.1;
	const long double fractal_right = 0.6;
	const long double fractal_bottom = -1.2;
	const long double fractal_top = 1.2;
	
	//double zoom_x = m_zoom * m_pixelBufferWidth / (fractal_right - fractal_left);
	long double zoom_y = m_zoom * m_pixelBufferHeigth / (fractal_top - fractal_bottom);
	long double zoom_x = zoom_y;
	
	long double fractal_width = m_pixelBufferWidth * m_zoom;
	long double fractal_heigth = m_pixelBufferHeigth * m_zoom;
	
	#pragma omp parallel for
	for (int image_x = 0; image_x < m_pixelBufferWidth; ++image_x)
	{
		for (int image_y = 0; image_y < m_pixelBufferHeigth; ++image_y)
		{
			long double fractal_x = fractal_width * m_normalizedPosition.x - m_pixelBufferWidth / 2 + image_x;
			long double fractal_y = fractal_heigth * m_normalizedPosition.y - m_pixelBufferHeigth / 2 + image_y;

			ldouble2 c= {
				fractal_x / (double)zoom_x + fractal_left,
				fractal_y / (double)zoom_y + fractal_bottom
			};
			ldouble2 z=c;

			int count;
			for (count=0;count<m_resolution;count++)
			{
				if ((z.x*z.x+z.y*z.y)>4.0f) 
					break;
				ldouble2 t = {
					z.x*z.x-z.y*z.y + c.x,
					2.0f*z.x*z.y + c.y
				};
				z=std::move(t);
			}
		
			if (count == m_resolution)
			{
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 0] = 0;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 1] = 54;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 2] = 76;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 3] = 255;
			}
			else
			{
				int val = count * 255 / m_resolution;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 0] = 0;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 1] = 0;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 2] = val;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 3] = 255;
			}
		}
	}
}

#endif