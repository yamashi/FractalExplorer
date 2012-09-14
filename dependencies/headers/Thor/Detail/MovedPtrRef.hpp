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

#ifndef THOR_MOVEDPTRREF_HPP
#define THOR_MOVEDPTRREF_HPP

#include <Thor/Detail/SmartPtrImpl.hpp>


namespace thor
{
namespace detail
{

	// Base class for MovedPtrRef, specialized for OwnershipPolicy = NoCopy
	template <typename T, template <typename> class OwnershipPolicy>
	class MovedPtrRefBase
	{
		protected:
			// Takes ownership from SmartPtrImpl object
			MovedPtrRefBase(SmartPtrImpl<T, OwnershipPolicy>& source)
			: mCopyFn(source.mCopyFn)
			{
			}

			// Gives ownership to SmartPtrImpl object
			void MoveTo(SmartPtrImpl<T, OwnershipPolicy>& dest)
			{
				dest.mCopyFn = mCopyFn;
			}

		protected:
			typename SmartPtrImpl<T, OwnershipPolicy>::CopyFnType	mCopyFn;

	};

	template <typename T>
	class MovedPtrRefBase<T, NoCopy>
	{
		protected:
			MovedPtrRefBase(SmartPtrImpl<T, NoCopy>&)
			{
			}

			void MoveTo(SmartPtrImpl<T, NoCopy>&)
			{
			}
	};

	// Helper class that allows passing and returning MovedPtr to and from functions.
	// This is achieved by an indirect conversion MovedPtr -> MovedPtrRef -> MovedPtr.
	template <typename T, template <typename> class OwnershipPolicy>
	class MovedPtrRef : public MovedPtrRefBase<T, OwnershipPolicy>
	{
		public:
			MovedPtrRef(SmartPtrImpl<T, OwnershipPolicy>& source)
			: MovedPtrRefBase<T, OwnershipPolicy>(source)
			, mPointer(source.mPointer)
			, mDestroyFn(source.mDestroyFn)
			{
				source.Release();
			}

			void MoveTo(SmartPtrImpl<T, OwnershipPolicy>& dest)
			{
				MovedPtrRefBase<T, OwnershipPolicy>::MoveTo(dest);

				dest.mPointer = mPointer;
				dest.mDestroyFn = mDestroyFn;
			}

		private:
			T*															mPointer;
			typename SmartPtrImpl<T, OwnershipPolicy>::DestroyFnType	mDestroyFn;
	};

} // namespace detail
} // namespace thor

#endif // THOR_MOVEDPTRREF_HPP
