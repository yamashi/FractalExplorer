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

namespace thor
{

template <typename T>
T Length(const sf::Vector3<T>& vector)
{
	return TrigonometricTraits<T>::Sqrt(SquaredLength(vector));
}

template <typename T>
T SquaredLength(const sf::Vector3<T>& vector)
{
	return DotProduct(vector, vector);
}

template <typename T>
sf::Vector3<T> UnitVector(const sf::Vector3<T>& vector)
{
	assert(vector != sf::Vector3<T>());
	return vector / Length(vector);
}

template <typename T>
T PolarAngle(const sf::Vector3<T>& vector)
{
	assert(vector != sf::Vector3<T>());
	return TrigonometricTraits<T>::ArcTan2(vector.y, vector.x);
}

template <typename T>
T ElevationAngle(const sf::Vector3<T>& vector)
{
	assert(vector != sf::Vector3<T>());

	T baseLength = Length(sf::Vector2<T>(vector.x, vector.y));
	return TrigonometricTraits<T>::ArcTan2(vector.z, baseLength);
}

// ---------------------------------------------------------------------------------------------------------------------------


template <typename T>
T DotProduct(const sf::Vector3<T>& lhs, const sf::Vector3<T>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template <typename T>
sf::Vector3<T> CrossProduct(const sf::Vector3<T>& lhs, const sf::Vector3<T>& rhs)
{
	return sf::Vector3<T>(
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x);
}

template <typename T>
sf::Vector3<T> ComponentwiseProduct(const sf::Vector3<T>& lhs, const sf::Vector3<T>& rhs)
{
	return sf::Vector3<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

template <typename T>
sf::Vector3<T> ComponentwiseQuotient(const sf::Vector3<T>& lhs, const sf::Vector3<T>& rhs)
{
	assert(rhs.x != 0 && rhs.y != 0 && rhs.z != 0);
	return sf::Vector3<T>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}

// ---------------------------------------------------------------------------------------------------------------------------


template <typename T>
sf::Vector3<T> ToVector3(const sf::Vector2<T>& vector2)
{
	return sf::Vector3<T>(vector2.x, vector2.y, T());
}

} // namespace thor
