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

#ifndef THOR_RTTIIMPL_HPP
#define THOR_RTTIIMPL_HPP

// Note: This is no .cpp file because the Dispatchers are header-only

#include <Thor/Detail/TypeInfo.hpp>

#include <vector>
#include <set>
#include <algorithm>
#include <cassert>


namespace thor
{
namespace detail
{

	// Stores a RTTI class type and links to base classes
	struct RttiClassNode
	{	
		explicit RttiClassNode(TypeInfo type)
		: type(type)
		, base(NULL)
		{
		}

		TypeInfo						type;
		mutable const RttiClassNode*	base;
	};

	// Comparison operator for std::set
	inline bool operator< (const RttiClassNode& lhs, const RttiClassNode& rhs)
	{
		return lhs.type < rhs.type;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	// Manager for class hierarchies whose RTTI is registered by the user
	struct RttiClassManager
	{
		// Return global instance
		static RttiClassManager& Instance()
		{
			static RttiClassManager instance;
			return instance;
		}

		// Add type information for a base class
		const RttiClassNode& InsertBase(TypeInfo base)
		{
			return *types.insert(base).first;
		}

		// Add type information for a derived class (and its base class)
		void InsertDerived(TypeInfo derived, const RttiClassNode& base)
		{
			std::set<RttiClassNode>::iterator itr = types.insert(derived).first;
			assert(!itr->base);
			itr->base = &base;
		}

		// Fill container with direct and indirect base classes, if available.
		// The container front contains the direct base, the back the indirect ones.
		void GetBases(TypeInfo derived, std::vector<TypeInfo>& out)
		{
			// Add class itself
			out.push_back(derived);

			// Check if the class has a base; if not, abort
			std::set<RttiClassNode>::iterator derivedItr = types.find(RttiClassNode(derived));
			if (derivedItr == types.end())
				return;

			// Go to next base class and add its type info
			const RttiClassNode* next = derivedItr->base;
			while (next)
			{
				out.push_back(next->type);
				next = next->base;
			}
		};

		// Type informations for the stored classes
		std::set<RttiClassNode> types;
	};

	// ---------------------------------------------------------------------------------------------------------------------------


	// Class used to build a class hierarchy based on runtime type informations
	struct RttiClass
	{
		explicit RttiClass(TypeInfo type)
		: base(RttiClassManager::Instance().InsertBase(type))
		{
		}

		RttiClass& Derived(TypeInfo derived)
		{
			RttiClassManager::Instance().InsertDerived(derived, base);
			return *this;
		}

		const RttiClassNode& base;
	};

	
	// ---------------------------------------------------------------------------------------------------------------------------
	
	// Type to represent pairs of "indirections" (distances to base classes)
	typedef std::pair<int, int> IndirectionPair;


	// ---------------------------------------------------------------------------------------------------------------------------

	// Collects base classes of a class, registered with the THOR_RTTI_... macros
	inline void GetRttiBaseClasses(TypeInfo derived, std::vector<TypeInfo>& out)
	{
		RttiClassManager::Instance().GetBases(derived, out);
	}

	// Computes "indirection pairs" (x, y) which contain the distances from derived to base classes (x for lhs, y for rhs).
	// indirectionLevel specifies the sum x+y for which pairs are generated.
	// (0, 0) is the direct match; i.e. argument and parameter types are the same. (1, 0) requires a derived-to-base conversion
	// of the first argument, (0, 1) one of the second argument, and so on.
	inline void ComputeRttiBaseIndirections(int nbFirst, int nbSecond, int indirectionLevel, std::vector<IndirectionPair>& out)
	{
		const int end = std::min(nbFirst-1, indirectionLevel);
		for (int i = std::max(indirectionLevel-nbSecond+1, 0); i <= end; ++i)
			out.push_back( IndirectionPair(i, indirectionLevel-i) );
	}

} // namespace detail
} // namespace thor

#endif // THOR_RTTIIMPL_HPP
