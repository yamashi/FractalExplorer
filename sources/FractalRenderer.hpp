
/*
 *  FractalRenderer.hpp
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
 */

#ifndef FRACTAL_RENDERER_HPP
#define FRACTAL_RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Common.hpp"

#ifdef CL_BUILD
#include "MandelbrotRendererCL.hpp"
#endif

class FractalRenderer {
public:
	FractalRenderer(unsigned width, unsigned height);
	~FractalRenderer(void);
	
	void performRendering(void);
	
	void setZoom(double zoom);
	void setNormalizedPosition(Vector2lf normalizedPosition);
	void setResolution(int resolution);
	
	double getZoom(void);
	const Vector2lf& getNormalizedPosition(void);
	int getResolution(void);
	const sf::Time& getLastRenderingTime(void);
	
	const sf::Texture& getTexture(void);
	
private:
	unsigned char *m_data;
	unsigned m_dataSize;
	sf::Texture m_texture;
	
	Vector2lf m_normalizedPosition;
	double m_scale;
	int m_resolution;
	int m_image_x;
	int m_image_y;
	
	sf::Time m_lastRenderingTime;

#ifdef CL_BUILD
	OpenCLDevice m_openclDevice;
	std::unique_ptr<MandelbrotRendererCL> m_renderer;
#endif
	
};

#endif
