
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

#include "../Common.hpp"

#ifdef OMP_BUILD

#include "MandelbrotRenderer.hpp"
#include <iostream>
#include <SFML/System.hpp>

struct ldouble2
{
	long double x;
	long double y;
};

void MandelbrotRenderer::render(unsigned char *pixelBuffer, unsigned width, unsigned heigth,
	mpfreal& zoom, int resolution, mpfreal& x, mpfreal& y)
{
	const long double fractal_left = -2.1;
	const long double fractal_right = 0.6;
	const long double fractal_bottom = -1.2;
	const long double fractal_top = 1.2;

	int counter = 0;
	int percentage = 0;
	mpfreal zoom_y;
	mpfreal fractal_width; 
	mpfreal fractal_heigth;
	mpfreal tmp;
	mpfreal const2;
	const2 = 2.0;

	tmp = double(heigth) / 2.4;
	mpf_mul(*zoom_y, *zoom, *tmp); // zoom_y = m_zoom * double(m_pixelBufferHeigth) / (fractal_top - fractal_bottom)
	
	tmp = (int)width;
	mpf_mul(*fractal_width, *tmp, *zoom);

	tmp = (int)heigth;
	mpf_mul(*fractal_heigth, *tmp, *zoom);

	
	#pragma omp parallel for
	for (int image_x = 0; image_x < width; ++image_x)
	{
		mpfreal result; 

		mpfreal localTmp; 
		mpfreal localTmp2; 

		mpfreal fractal_x; 
		mpfreal fractal_y; 

		mpfreal cx, cy; 
		mpfreal zx, zy; 

		localTmp = (int)width / 2;

		mpf_mul(*fractal_x, *fractal_width, *x); //fractal_x = fractal_width * m_x
		mpf_sub(*result, *fractal_x, *localTmp); // result = fractal_width * m_x - (m_pixelBufferWidth / 2);
		mpf_add_ui(*fractal_x, *result, image_x); // fractal_x = fractal_width * m_x - (m_pixelBufferWidth / 2) + image_x;

		localTmp = (double)fractal_left;

		mpf_div(*cx, *fractal_x,*zoom_y); // cx = fractal_x/zoom_y;
		mpf_add(*cx, *cx, *localTmp); // cx = cx + fractal_left

		for (int image_y = 0; image_y < heigth; ++image_y)
		{			
			localTmp = (int)heigth / 2;
			mpf_mul(*fractal_y, *fractal_heigth, *y); //fractal_y = fractal_height * m_y
			mpf_sub(*result, *fractal_y, *localTmp); // result = fractal_height * m_y - (m_pixelBufferHeigth / 2);
			mpf_add_ui(*fractal_y, *result, image_y); // fractal_y = fractal_height * m_y - (m_pixelBufferHeigth / 2) + image_y;
			
			localTmp = (double)fractal_bottom;

			mpf_div(*cy, *fractal_y,*zoom_y); // cx = fractal_y/zoom_y;
			mpf_add(*cy, *cy, *localTmp); // result = cy + fractal_bottom

			zx = cx;
			zy = cy;

			unsigned int count;
			for (count=0;count<resolution;++count)
			{
				mpf_mul(*localTmp, *zx, *zx); // zx * zx
				mpf_mul(*localTmp2, *zy, *zy); // zy * zy
				mpf_add(*result,*localTmp, *localTmp2); // result = zx * zx + zy * zy

				if (mpf_cmp_d(*result, 4.0f) > 0) 
					break;

				mpf_sub(*result, *localTmp, *localTmp2); // tx = zx * zx - zy * zy
				mpf_add(*localTmp2, *result, *cx); // result = zx * zx - zy * zy + cx
			
				mpf_mul(*localTmp, *zx, *const2); // localTmp = zx * 2.0
				mpf_mul(*result, *localTmp, *zy); // result = zx * 2 * zy
				mpf_add(*zy, *result, *cy); //zy = zx * 2 * zy + cy

				zx = localTmp2;
			}
		
			if (count == resolution)
			{
				pixelBuffer[(image_y * width + image_x) * 4 + 0] = 0;
				pixelBuffer[(image_y * width + image_x) * 4 + 1] = 0;
				pixelBuffer[(image_y * width + image_x) * 4 + 2] = 0;
				pixelBuffer[(image_y * width + image_x) * 4 + 3] = 255;
			}
			else
			{
				int val = count * 255 / resolution;
				pixelBuffer[(image_y * width + image_x) * 4 + 0] = val;
				pixelBuffer[(image_y * width + image_x) * 4 + 1] = 0;
				pixelBuffer[(image_y * width + image_x) * 4 + 2] = 0;
				pixelBuffer[(image_y * width + image_x) * 4 + 3] = 255;
			}


		}

#pragma omp critical
		{
			++counter;
			if(counter >= width/20)
			{
				percentage += 5;
				counter = 0;
				std::cout << "\xd" << percentage << "% done";
			}
		}
		
	}
}

#endif