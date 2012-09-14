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

#ifndef THOR_RTTI_HPP
#define THOR_RTTI_HPP

/// @file
/// @brief Manual enhancements of Runtime Type Information

#include <Thor/Detail/RttiImpl.hpp>
#include <Thor/Config.hpp>


namespace thor
{

/// @addtogroup Tools
/// @{

/// @brief Register runtime type information for a base class
/// @details You need this for the dynamic dispatchers to cope with derived-to-base conversions.
/// Since C++ offers no possibility to recognize the base classes of a class at runtime, this must be done manually.
/// Note that multiple inheritance is not supported.
/// @n@n Example class hierarchy:
/// @code
/// class Vehicle { public: virtual ~Vehicle(); }
/// class Aircraft : public Vehicle {};
/// class Watercraft : public Vehicle {};
/// class Ship : public Watercraft {};
/// class Submarine : public Watercraft {};
/// @endcode
/// Register classes as follows (once inside a function):
/// @code
/// THOR_RTTI_BASE(Vehicle)
///   THOR_RTTI_DERIVED(Aircraft)
///   THOR_RTTI_DERIVED(Watercraft);
/// THOR_RTTI_BASE(Watercraft)
///   THOR_RTTI_DERIVED(Ship)
///   THOR_RTTI_DERIVED(Submarine);
/// @endcode
/// @hideinitializer
#define THOR_RTTI_BASE(BaseClass)			::thor::detail::RttiClass(typeid(BaseClass))

/// @brief Register runtime type information for a derived class
/// @see THOR_RTTI_BASE
/// @hideinitializer
#define THOR_RTTI_DERIVED(DerivedClass)		.Derived(typeid(DerivedClass))

/// @}

} // namespace thor

#endif // THOR_RTTI_HPP
