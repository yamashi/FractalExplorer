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
namespace detail
{

	// No-Op function to assign to function pointer
	template <typename T>
	void DoNothing(T*)
	{
	}

	// No-Op function to assign to function pointer
	template <typename T>
	T* ReturnNull(const T*)
	{
		return NULL;
	}

	// T: Static object type (examples: Base, int)
	// U: Dynamic object type, is normally the same as T (examples: Derived, int)
	template <typename T, typename U, template <typename> class OwnershipPolicy>
	T* ForwardCopy(const T* pointer)
	{
		// Ensure fully defined type
		static_cast<void>(sizeof(U));
	
		// Cast to U*: If Copy() is a function template parametrized with its parameter, the exact type U is 
		// recognized. Otherwise, the implicit U* -> T* conversion is automatically applied, undoing static_cast.
		return OwnershipPolicy<T>::Copy( static_cast<const U*>(pointer) );
	}

	// T: Static object type (examples: Base, int)
	// U: Dynamic object type, is normally the same as T (examples: Derived, int)
	template <typename T, typename U, template <typename> class OwnershipPolicy>
	void ForwardDestroy(T* pointer)
	{
		// Ensure fully defined type
		static_cast<void>(sizeof(U));
	
		// Cast to U*: If Destroy() is a function template parametrized with its pointer type, the exact type U is 
		// recognized. Otherwise, the implicit U* -> T* conversion is automatically applied, undoing static_cast.
		OwnershipPolicy<T>::Destroy( static_cast<U*>(pointer) );
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	template <typename T, template <typename> class OwnershipPolicy>
	SmartPtrImplBase<T, OwnershipPolicy>::SmartPtrImplBase()
	// uninitialized (will be done in derived class's Clear() method)
	{
	}

	template <typename T, template <typename> class OwnershipPolicy>
	template <typename U>
	SmartPtrImplBase<T, OwnershipPolicy>::SmartPtrImplBase(U*)
	: mCopyFn(&ForwardCopy<T, U, OwnershipPolicy>)
	{
	}

	template <typename T, template <typename> class OwnershipPolicy>
	void SmartPtrImplBase<T, OwnershipPolicy>::CopyTo(SmartPtrImplBase& dest) const
	{
		dest.mCopyFn = mCopyFn;
	}

	template <typename T, template <typename> class OwnershipPolicy>
	void SmartPtrImplBase<T, OwnershipPolicy>::Swap(SmartPtrImplBase& other)
	{
		std::swap(mCopyFn, other.mCopyFn);
	}

	template <typename T, template <typename> class OwnershipPolicy>
	void SmartPtrImplBase<T, OwnershipPolicy>::Clear()
	{
		mCopyFn = &detail::ReturnNull<T>;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	template <typename T>
	SmartPtrImplBase<T, NoCopy>::SmartPtrImplBase()
	{
	}

	template <typename T>
	SmartPtrImplBase<T, NoCopy>::SmartPtrImplBase(T*)
	{
	}

	template <typename T>
	void SmartPtrImplBase<T, NoCopy>::CopyTo(SmartPtrImplBase&) const
	{
	}

	template <typename T>
	void SmartPtrImplBase<T, NoCopy>::Swap(SmartPtrImplBase&)
	{
	}

	template <typename T>
	void SmartPtrImplBase<T, NoCopy>::Clear()
	{
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	template <typename T, template <typename> class OwnershipPolicy>
	SmartPtrImpl<T, OwnershipPolicy>::SmartPtrImpl()
	: SmartPtrImplBase<T, OwnershipPolicy>()
	// uninitialized
	{
		Clear();
	}

	template <typename T, template <typename> class OwnershipPolicy>
	template <typename U>
	SmartPtrImpl<T, OwnershipPolicy>::SmartPtrImpl(U* pointer)
	: SmartPtrImplBase<T, OwnershipPolicy>(pointer)
	, mPointer(pointer)
	, mDestroyFn(&ForwardDestroy<T, U, OwnershipPolicy>)
	{
	}

	template <typename T, template <typename> class OwnershipPolicy>
	SmartPtrImpl<T, OwnershipPolicy>::~SmartPtrImpl()
	{
		// Indirection via function pointer to avoid dependency on fully defined type T in class definition
		mDestroyFn(mPointer);
	}

	template <typename T, template <typename> class OwnershipPolicy>
	void SmartPtrImpl<T, OwnershipPolicy>::CopyTo(SmartPtrImpl& dest) const
	{
		dest.mPointer = IsNull() ? NULL : mCopyFn(mPointer);
		dest.mDestroyFn = mDestroyFn;

		SmartPtrImplBase<T, OwnershipPolicy>::CopyTo(dest);
	}

	template <typename T, template <typename> class OwnershipPolicy>
	void SmartPtrImpl<T, OwnershipPolicy>::Swap(SmartPtrImpl<T, OwnershipPolicy>& other)
	{
		SmartPtrImplBase<T, OwnershipPolicy>::Swap(other);
		std::swap(mPointer, other.mPointer);
		std::swap(mDestroyFn, other.mDestroyFn);
	}

	template <typename T, template <typename> class OwnershipPolicy>
	void SmartPtrImpl<T, OwnershipPolicy>::Clear()
	{
		// Initialize with dummy function to avoid dependency on T, in case the actual objects are never accessed
		mPointer = NULL;
		mDestroyFn = &detail::DoNothing<T>;
		SmartPtrImplBase<T, OwnershipPolicy>::Clear();
	}

	template <typename T, template <typename> class OwnershipPolicy>
	T* SmartPtrImpl<T, OwnershipPolicy>::Release()
	{
		// Save pointer, empty content and return saved pointer
		T* copy = mPointer;
		Clear();
		return copy;
	}

	template <typename T, template <typename> class OwnershipPolicy>
	bool SmartPtrImpl<T, OwnershipPolicy>::IsNull() const
	{
		return mPointer == NULL;
	}

	template <typename T, template <typename> class OwnershipPolicy>
	T& SmartPtrImpl<T, OwnershipPolicy>::operator* () const
	{
		assert(!IsNull());
		return *mPointer;
	}

	template <typename T, template <typename> class OwnershipPolicy>
	T* SmartPtrImpl<T, OwnershipPolicy>::operator-> () const
	{
		assert(!IsNull());
		return mPointer;
	}

	template <typename T, template <typename> class OwnershipPolicy>
	T* SmartPtrImpl<T, OwnershipPolicy>::Get() const
	{
		return mPointer;
	}

	template <typename T, template <typename> class OwnershipPolicy>
	void swap(SmartPtrImpl<T, OwnershipPolicy>& lhs, SmartPtrImpl<T, OwnershipPolicy>& rhs)
	{
		lhs.Swap(rhs);
	}

	template <typename T, template <typename> class OwnershipPolicy, class SmartPtr>
	SmartPtrImpl<T, OwnershipPolicy>& SmartPtrAccessor::GetImpl(SmartPtr& ptr)
	{
		return ptr.mImpl;
	}

	template <typename T, template <typename> class OwnershipPolicy, class SmartPtr>
	const SmartPtrImpl<T, OwnershipPolicy>& SmartPtrAccessor::GetImpl(const SmartPtr& ptr)
	{
		return ptr.mImpl;
	}

} // namespace detail
} // namespace thor
