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

#ifndef THOR_SMARTPTRIMPL_HPP
#define THOR_SMARTPTRIMPL_HPP

#include <Thor/SmartPtr/OwnershipPolicies.hpp>
#include <Thor/Tools/NonCopyable.hpp>
#include <Thor/Detail/Metaprogramming.hpp>
#include <Thor/Detail/SafeBoolIdiom.hpp>

#include <algorithm>
#include <cassert>


namespace thor
{
namespace detail
{

	// Forward declarations
	template <typename T, template <typename> class OwnershipPolicy>
	class MovedPtrRefBase;

	template <typename T, template <typename> class OwnershipPolicy>
	class MovedPtrRef;
		
	// Helper base class that provides either functionality for deep copy
	// (when OwnershipPolicy != NoCopy) or is empty in other cases
	template <typename T, template <typename> class OwnershipPolicy>
	class SmartPtrImplBase : private NonCopyable
	{
		public:
			typedef T*					(*CopyFnType)(const T*);

		public:
			// Constructors
										SmartPtrImplBase();
			template <typename U>
			explicit					SmartPtrImplBase(U* pointer);


			// Accessors and modifiers
			void						CopyTo(SmartPtrImplBase& dest) const;
			void						Swap(SmartPtrImplBase& other);
			void						Clear();

		protected:
			CopyFnType					mCopyFn;
	};

	template <typename T>
	class SmartPtrImplBase<T, NoCopy> : private NonCopyable
	{
		public:
			// Constructors
										SmartPtrImplBase();
			explicit					SmartPtrImplBase(T* pointer);

			// Accessors and modifiers
			void						CopyTo(SmartPtrImplBase& dest) const;
			void						Swap(SmartPtrImplBase& other);
			void						Clear();
	};

	// Helper class for various smart-pointer implementations to reduce code duplication
	// Actual documentation: See any of the wrapping smart-pointer classes
	template <typename T, template <typename> class OwnershipPolicy>
	class SmartPtrImpl : public SmartPtrImplBase<T, OwnershipPolicy>
	{
		public:
			typedef void				(*DestroyFnType)(T*);

		public:
			// Constructors and destructors
										SmartPtrImpl();
			template <typename U>
			explicit					SmartPtrImpl(U* pointer);
										~SmartPtrImpl();
		
			// Accessors and modifiers
			void						CopyTo(SmartPtrImpl& dest) const;
			void						Swap(SmartPtrImpl& other);
			void						Clear();
			T*							Release();
			bool						IsNull() const;
			T&							operator* () const;
			T*							operator-> () const;
			T*							Get() const;
			
		private:
			T*							mPointer;
			DestroyFnType				mDestroyFn;

		friend class MovedPtrRef<T, OwnershipPolicy>;
		friend class MovedPtrRefBase<T, OwnershipPolicy>;
	};

	// Global swap
	template <typename T, template <typename> class OwnershipPolicy>
	void swap(SmartPtrImpl<T, OwnershipPolicy>& lhs, SmartPtrImpl<T, OwnershipPolicy>& rhs);

	// Accessor for smart pointer internals
	struct SmartPtrAccessor
	{
		template <typename T, template <typename> class OwnershipPolicy, class SmartPtr>
		static SmartPtrImpl<T, OwnershipPolicy>& GetImpl(SmartPtr& ptr);

		template <typename T, template <typename> class OwnershipPolicy, class SmartPtr>
		static const SmartPtrImpl<T, OwnershipPolicy>& GetImpl(const SmartPtr& ptr);
	};

} // namespace detail
} // namespace thor

#include <Thor/Detail/SmartPtrImpl.inl>
#endif // THOR_SMARTPTRIMPL_HPP
