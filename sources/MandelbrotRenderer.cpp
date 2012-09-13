
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
									   mpf_t& zoom, int resolution, mpf_t& x, mpf_t& y):
m_pixelBuffer(pixelBuffer),
m_pixelBufferWidth(width),
m_pixelBufferHeigth(heigth),
m_resolution(resolution)
{
	mpf_init(m_x);
	mpf_init(m_y);
	mpf_init(m_zoom);

	mpf_set(m_x, x);
	mpf_set(m_y, y);
	mpf_set(m_zoom, zoom);
}

MandelbrotRenderer::~MandelbrotRenderer()
{
	mpf_clear(m_x);
	mpf_clear(m_y);
	mpf_clear(m_zoom);
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

	/*int image_x = 451;
	int image_y = 254;

	{
		double tmpZoom = mpf_get_d(m_zoom);
		//double zoom_x = m_zoom * m_pixelBufferWidth / (fractal_right - fractal_left);
		long double zoom_y = tmpZoom * m_pixelBufferHeigth / (fractal_top - fractal_bottom);
		long double zoom_x = zoom_y;

		long double fractal_width = m_pixelBufferWidth * tmpZoom;
		long double fractal_heigth = m_pixelBufferHeigth * tmpZoom;

		long double fractal_x = fractal_width * mpf_get_d(m_x) - m_pixelBufferWidth / 2 + image_x;
		long double fractal_y = fractal_heigth * mpf_get_d(m_y) - m_pixelBufferHeigth / 2 + image_y;

			ldouble2 c= {
				fractal_x / (double)zoom_x + fractal_left,
				fractal_y / (double)zoom_y + fractal_bottom
			};
			ldouble2 z=c;

			
			std::cout <<  c.x << " " << c.y << std::endl;

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
	}*/
	
	//double zoom_x = m_zoom * m_pixelBufferWidth / (fractal_right - fractal_left);
	mpf_t zoom_y; mpf_init(zoom_y);
	mpf_t fractal_width; mpf_init(fractal_width);
	mpf_t fractal_heigth; mpf_init(fractal_heigth);
	mpf_t tmp; mpf_init(tmp); 
	mpf_t const2; mpf_init(const2);

	mpf_set_d(const2, 2.0);
	
	mpf_set_d(tmp, double(m_pixelBufferHeigth) / 2.4);
	mpf_mul(zoom_y, m_zoom, tmp); // zoom_y = m_zoom * double(m_pixelBufferHeigth) / (fractal_top - fractal_bottom)
	
	mpf_set_ui(tmp, m_pixelBufferWidth);
	mpf_mul(fractal_width, tmp, m_zoom);

	mpf_set_ui(tmp, m_pixelBufferHeigth);
	mpf_mul(fractal_heigth, tmp, m_zoom);

	
	#pragma omp parallel for
	for (int image_x = 0; image_x < m_pixelBufferWidth; ++image_x)
	{
		mpf_t result; mpf_init(result);
		mpf_t localTmp; mpf_init(localTmp);
		mpf_t fractal_x; mpf_init(fractal_x);

		mpf_set_d(localTmp, m_pixelBufferWidth / 2); // localTmp = m_pixelBufferWidth / 2 + image_x
		mpf_mul(fractal_x, fractal_width, m_x); //fractal_x = fractal_width * m_x
		mpf_sub(result, fractal_x, localTmp); // result = fractal_width * m_x - (m_pixelBufferWidth / 2);
		mpf_add_ui(fractal_x, result, image_x); // fractal_x = fractal_width * m_x - (m_pixelBufferWidth / 2) + image_x;

		for (int image_y = 0; image_y < m_pixelBufferHeigth; ++image_y)
		{
			
			mpf_t fractal_y; mpf_init(fractal_y);

			mpf_t localTmp2; mpf_init(localTmp2);
			mpf_t localTmp3; mpf_init(localTmp3);

			mpf_t cx, cy; mpf_init(cx); mpf_init(cy);
			mpf_t zx, zy; mpf_init(zx); mpf_init(zy);
			mpf_t tx, ty; mpf_init(tx); mpf_init(ty);
			
			mpf_set_d(localTmp, m_pixelBufferHeigth / 2); // localTmp = m_pixelBufferHeigth / 2 + image_y
			mpf_mul(fractal_y, fractal_heigth, m_y); //fractal_x = fractal_height * m_y
			mpf_sub(result, fractal_y, localTmp); // result = fractal_height * m_y - (m_pixelBufferHeigth / 2);
			mpf_add_ui(fractal_y, result, image_y); // fractal_y = fractal_height * m_y - (m_pixelBufferHeigth / 2) + image_y;
			
			mpf_set_d(localTmp, fractal_left);

			mpf_div(cx, fractal_x,zoom_y); // cx = fractal_x/zoom_y;
			mpf_add(result, cx, localTmp); // result = cx + fractal_left
			mpf_set(cx, result);

			mpf_set_d(localTmp, fractal_bottom);

			mpf_div(cy, fractal_y,zoom_y); // cx = fractal_y/zoom_y;
			mpf_add(result, cy, localTmp); // result = cy + fractal_bottom
			mpf_set(cy, result);

			mpf_set(zx, cx);
			mpf_set(zy, cy);

			int count;
			for (count=0;count<m_resolution;count++)
			{
				mpf_mul(localTmp, zx, zx); // zx * zx
				mpf_mul(localTmp2, zy, zy); // zy * zy
				mpf_add(result, localTmp, localTmp2); // result = zx * zx + zy * zy

				if (mpf_cmp_d(result, 4.0f) > 0) 
					break;

				mpf_sub(tx, localTmp, localTmp2); // tx = zx * zx - zy * zy
				mpf_add(result, tx, cx); // result = zx * zx - zy * zy + cx
				mpf_set(tx, result); // zx = zx * zx - zy * zy + cx

				
				mpf_mul(localTmp, zx, const2); // localTmp = zx * 2.0
				mpf_mul(result, localTmp, zy); // result = zx * 2 * zy
				mpf_add(zy, result, cy); //zy = zx * 2 * zy + cy

				mpf_set(zx, tx);
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

			mpf_clear(fractal_y);
			mpf_clear(cx);
			mpf_clear(cy);
			mpf_clear(tx);
			mpf_clear(ty);
			mpf_clear(zx);
			mpf_clear(zy);
			
			
			mpf_clear(localTmp2);
			mpf_clear(localTmp3);
		}
		
		mpf_clear(localTmp);
		mpf_clear(result);
		mpf_clear(fractal_x);
	}

	mpf_clear(tmp);
	mpf_clear(zoom_y);
	mpf_clear(fractal_width);
	mpf_clear(fractal_heigth);
	mpf_clear(const2);
}

#endif