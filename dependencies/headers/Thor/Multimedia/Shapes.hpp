/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2012 Jan Haller
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////

/// @file
/// @brief Factory functions that create different shapes (concave and convex)

#ifndef THOR_SHAPES_HPP
#define THOR_SHAPES_HPP

#include <Thor/Config.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sf
{

	class Shape;
	class ConvexShape;

} // namespace sf


namespace thor
{

class ConcaveShape;

/// @addtogroup Multimedia
/// @{

/// @brief Namespace for predefined shapes
/// @details This namespace contains some functions to create predefined shapes like stars or rounded rectangles.
///  The namespace Shapes can be considered an extension to the specific SFML shape classes like sf::CircleShape or sf::RectangleShape.
namespace Shapes
{

	/// @brief Converts a specific shape to a general, convex SFML shape.
	/// @return General shape with the same properties (shape attributes, texture, transform) as the parameter. Note that you lose
	///  specific information (such as a circle radius) during this conversion.
	sf::ConvexShape THOR_API	ToConvexShape(const sf::Shape& shape);

	/// @brief Creates a rectangle with radiused corners.
	/// @param direction Line direction.
	/// @param thickness Line thickness (> 0.f).
	/// @param color Color of the line.
	/// @return A sf::ConvexShape copy representing the line.
	sf::ConvexShape THOR_API	Line(sf::Vector2f direction, const sf::Color& color, float thickness = 1.f);

	/// @brief Creates a rectangle with radiused corners.
	/// @param size Width and height of the rectangle.
	/// @param cornerRadius The radius of the circle at the corners.
	/// @param fillColor The rounded rectangle's fill color.
	/// @param outlineThickness The thickness of the outline.
	/// @param outlineColor Color used for the outline.
	/// @return A sf::ConvexShape copy containing the rounded rectangle.
	sf::ConvexShape THOR_API	RoundedRect(sf::Vector2f size, float cornerRadius,
									const sf::Color& fillColor, float outlineThickness = 0.f, const sf::Color& outlineColor = sf::Color());

	/// @brief Creates a regular, convex polygon shape.
	/// @param nbPoints The number of polygon points (the smallest possible value is 3 for a triangle).
	/// @param radius The distance from the center to the points (> 0.f).
	/// @param fillColor The color which is used to fill the inside of the polygon.
	/// @param outlineThickness The thickness of the outline.
	/// @param outlineColor The color used for the outline.
	/// @return A sf::ConvexShape copy representing the regular polygon.
	sf::ConvexShape THOR_API	Polygon(unsigned int nbPoints, float radius, const sf::Color& fillColor,
									float outlineThickness = 0.f, const sf::Color& outlineColor = sf::Color());

	/// @brief Creates a regular star shape.
	/// @param nbStarPoints The number of points (the smallest possible is a 3-pointed star).
	/// @param innerRadius The distance from the center to the inner points (> 0.f).
	/// @param outerRadius The distance from the center to the outer points  (> innerRadius).
	/// @param fillColor The color which is used to fill the star.
	/// @param outlineThickness The thickness of the outline.
	/// @param outlineColor The color used for the outline.
	/// @return A convex shape representing the star.
	sf::ConvexShape THOR_API	Star(unsigned int nbStarPoints, float innerRadius, float outerRadius, const sf::Color& fillColor,
									float outlineThickness = 0.f, const sf::Color& outlineColor = sf::Color());

	/// @brief Creates a pie (a sector of a circle).
	/// @param radius Radius of the circle.
	/// @param filledAngle The part of the circle which is filled (angle in degrees). The value doesn't have to be inside [0,360[.
	/// @param fillColor The color which is used to fill the pie.
	/// @param outlineThickness The thickness of the outline.
	/// @param outlineColor The color used for the outline.
	/// @return A concave shape representing the pie.
	ConcaveShape THOR_API		Pie(float radius, float filledAngle, const sf::Color& fillColor, float outlineThickness = 0.f,
									const sf::Color& outlineColor = sf::Color());

} // namespace Shapes

/// @}

} // namespace thor

#endif // THOR_SHAPES_HPP
