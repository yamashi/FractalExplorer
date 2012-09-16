
/*
 *  FractalRenderer.cpp
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
 *
 * Source altered by Maxime Griot
 */

#include "FractalRenderer.hpp"
#include "Renderer/MandelbrotRendererCL.hpp"
#include "Renderer/MandelbrotRenderer.hpp"
#include <iostream>


FractalRenderer::FractalRenderer(unsigned width, unsigned heigth) :
m_data(NULL),
m_texture(),
m_normalizedPosition(0.310617, 0.435056),
m_scale(1.0),
m_resolution(100),
m_image_x(width),
m_image_y(heigth),
m_lastRenderingTime(sf::Time::Zero),
m_mode(true),
isMultiPrecision(false)
{
	m_data = new unsigned char[m_image_x * m_image_y * 4];
	std::memset(m_data, 0, m_image_x * m_image_y * 4);
	
	if (m_texture.create(m_image_x, m_image_y))
	{
		m_texture.setSmooth(true);
	}
	else
	{
		std::cout << "texture size is too big for your crapy graphics card" << std::endl;
	}
}

FractalRenderer::~FractalRenderer()
{
	delete[] m_data;
}

#include <cstdio>

void FractalRenderer::performRendering()
{
	_performRendering();
}

void FractalRenderer::_performRendering(void)
{
	isRendering = true;
	sf::Clock timer;

	IRenderer* renderer = nullptr;
	if(m_mode)
		renderer = new MandelbrotRendererCL;
	else
		renderer = new MandelbrotRenderer;

	mpfreal zoom, posx, posy;

	zoom = m_scale;
	posx = (double)m_normalizedPosition.x;
	posy = (double)m_normalizedPosition.y;

	renderer->render(m_data, m_image_x, m_image_y, zoom, m_resolution, posx, posy);

	delete renderer;

	m_texture.update(m_data);
	
	m_lastRenderingTime = timer.getElapsedTime();

	isRendering = false;
}

void FractalRenderer::setZoom(double zoom)
{
	m_scale = zoom;
}

void FractalRenderer::setMode(bool mode)
{
	m_mode = mode;
}

void FractalRenderer::setNormalizedPosition(Vector2lf normalizedPosition)
{
	m_normalizedPosition = normalizedPosition;
}


void FractalRenderer::setResolution(int resolution)
{
	m_resolution = resolution;
}


double FractalRenderer::getZoom(void)
{
	return m_scale;
}


bool FractalRenderer::getMode() const
{
	return m_mode;
}

const Vector2lf& FractalRenderer::getNormalizedPosition(void)
{
	return m_normalizedPosition;
}


int FractalRenderer::getResolution(void)
{
	return m_resolution;
}

const sf::Time& FractalRenderer::getLastRenderingTime(void)
{
	return m_lastRenderingTime;
}

const sf::Texture& FractalRenderer::getTexture(void)
{
	return m_texture;
}




