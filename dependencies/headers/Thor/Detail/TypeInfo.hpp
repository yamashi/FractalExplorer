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

#ifndef THOR_TYPEINFO_HPP
#define THOR_TYPEINFO_HPP

#include <Thor/Config.hpp>

#include <typeinfo>
#include THOR_TR1_HEADER(type_traits)


namespace thor
{
namespace detail
{

	// Convenience wrapper around std::type_info
	class TypeInfo
	{
		public:
			// Implicit constructor from typeid
			TypeInfo(const std::type_info& t)
			: mTypeInfo(&t)
			{
			}

			const char* GetName() const
			{
				return mTypeInfo->name();
			}

		private:
			const std::type_info*	mTypeInfo;

		friend bool operator< (const TypeInfo& lhs, const TypeInfo& rhs)
		{
			// !! to deactivate conversion warning 4800 on MSVC++ because before() returns int for no reason
			return !!lhs.mTypeInfo->before(*rhs.mTypeInfo);
		}

		friend bool operator== (const TypeInfo& lhs, const TypeInfo& rhs)
		{
			return *lhs.mTypeInfo == *rhs.mTypeInfo;
		}

		friend bool operator!= (const TypeInfo& lhs, const TypeInfo& rhs)
		{
			return !(lhs == rhs);
		}
	};


	// Returns the type-id of the object behind the pointer or reference
	template <typename T>
	TypeInfo DerefTypeid(T& reference)
	{
		return typeid(reference);
	}

	template <typename T>
	TypeInfo DerefTypeid(T* pointer)
	{
		return typeid(*pointer);
	}

	// Returns the type-id of U, when T=U*, otherwise of T
	template <typename T>
	TypeInfo DerefTypeid()
	{
		return typeid(typename std::tr1::remove_pointer<T>::type);
	}

} // namespace detail
} // namespace thor

#endif // THOR_TYPEINFO_HPP
