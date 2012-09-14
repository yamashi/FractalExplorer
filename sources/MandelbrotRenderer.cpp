
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
									   const mpfr::mpreal& zoom, int resolution, const mpfr::mpreal& x, const mpfr::mpreal& y):
m_pixelBuffer(pixelBuffer),
m_pixelBufferWidth(width),
m_pixelBufferHeigth(heigth),
m_resolution(resolution),
m_zoom(zoom),
m_x(x),
m_y(y)
{

}

MandelbrotRenderer::~MandelbrotRenderer()
{
}

struct mpreal2
{
	mpfr::mpreal x;
	mpfr::mpreal y;
};

void MandelbrotRenderer::operator()() const
{
	const mpfr::mpreal fractal_left = -2.1;
	const mpfr::mpreal fractal_right = 0.6;
	const mpfr::mpreal fractal_bottom = -1.2;
	const mpfr::mpreal fractal_top = 1.2;
	const mpfr::mpreal const2 = 2.0;

	//double zoom_x = m_zoom * m_pixelBufferWidth / (fractal_right - fractal_left);
	mpfr::mpreal zoom_y = m_zoom * m_pixelBufferHeigth / (fractal_top - fractal_bottom);

	mpfr::mpreal fractal_width = m_pixelBufferWidth * m_zoom;
	mpfr::mpreal fractal_heigth = m_pixelBufferHeigth * m_zoom;

	#pragma omp parallel for
	for (int image_x = 0; image_x < m_pixelBufferWidth; ++image_x)
	{
		mpfr::mpreal fractal_x = fractal_width * m_x - m_pixelBufferWidth / 2 + image_x;
		mpfr::mpreal fractal_y;
		mpreal2 c;
		mpreal2 z;
		mpreal2 tmp;
		mpreal2 t;

		for (int image_y = 0; image_y < m_pixelBufferHeigth; ++image_y)
		{
			fractal_y = fractal_heigth * m_y - m_pixelBufferHeigth / 2 + image_y;

			c.x = fractal_x / zoom_y + fractal_left;
			c.y = fractal_y / zoom_y + fractal_bottom;

			z=c;

			unsigned int count = 0;
			for (;count<m_resolution; ++count)
			{
				tmp.x = z.x * z.x;
				tmp.y = z.y * z.y;

				if ((tmp.x + tmp.y)>4.0f) 
					break;

				t.x = tmp.x-tmp.y + c.x;
				t.y = const2*z.x*z.y + c.y;

				z=std::move(t);
			}
		
			if (count == m_resolution)
			{
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 0] = 0;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 1] = 0;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 2] = 0;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 3] = 255;
			}
			else
			{
				int val = count * 255 / m_resolution;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 0] = val;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 1] = 0;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 2] = 0;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 3] = 255;
			}
		}
	}
}

#endif