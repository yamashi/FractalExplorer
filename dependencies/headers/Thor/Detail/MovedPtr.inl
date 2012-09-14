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

template <typename T, template <typename> class OwnershipPolicy>
MovedPtr<T, OwnershipPolicy>::MovedPtr()
: mImpl()
{
}

template <typename T, template <typename> class OwnershipPolicy>
template <typename U>
MovedPtr<T, OwnershipPolicy>::MovedPtr(U* pointer)
: mImpl(pointer)
{
}

template <typename T, template <typename> class OwnershipPolicy>
MovedPtr<T, OwnershipPolicy>::MovedPtr(MovedPtr& origin)
: mImpl()
{
	// Move semantics through swapping
	mImpl.Swap(origin.mImpl);
}

template <typename T, template <typename> class OwnershipPolicy>
MovedPtr<T, OwnershipPolicy>::~MovedPtr()
{
	// mImpl destroys and deallocates the pointer
}

template <typename T, template <typename> class OwnershipPolicy>
MovedPtr<T, OwnershipPolicy>& MovedPtr<T, OwnershipPolicy>::operator= (MovedPtr& origin)
{
	MovedPtr temp(origin);
	Swap(temp);
	return *this;
}

template <typename T, template <typename> class OwnershipPolicy>
void MovedPtr<T, OwnershipPolicy>::Swap(MovedPtr& other)
{
	swap(mImpl, other.mImpl);
}

template <typename T, template <typename> class OwnershipPolicy>
void MovedPtr<T, OwnershipPolicy>::Reset()
{
	MovedPtr tmp;
	Swap(tmp);
}

template <typename T, template <typename> class OwnershipPolicy>
template <typename U>
void MovedPtr<T, OwnershipPolicy>::Reset(U* pointer)
{
	MovedPtr tmp(pointer);
	Swap(tmp);
}

template <typename T, template <typename> class OwnershipPolicy>
T* MovedPtr<T, OwnershipPolicy>::Release()
{
	return mImpl.Release();
}

template <typename T, template <typename> class OwnershipPolicy>
MovedPtr<T, OwnershipPolicy>::operator detail::SafeBool() const
{
	return detail::ToSafeBool(!mImpl.IsNull());
}

template <typename T, template <typename> class OwnershipPolicy>
T& MovedPtr<T, OwnershipPolicy>::operator* () const
{
	return mImpl.operator*();
}

template <typename T, template <typename> class OwnershipPolicy>
T* MovedPtr<T, OwnershipPolicy>::operator-> () const
{
	return mImpl.operator->();
}

template <typename T, template <typename> class OwnershipPolicy>
T* MovedPtr<T, OwnershipPolicy>::Get() const
{
	return mImpl.Get();
}

template <typename T, template <typename> class OwnershipPolicy>
MovedPtr<T, OwnershipPolicy>::MovedPtr(detail::MovedPtrRef<T, OwnershipPolicy> ref)
: mImpl()
{
	// Get ownership from ref
	ref.MoveTo(mImpl);
}

template <typename T, template <typename> class OwnershipPolicy>
MovedPtr<T, OwnershipPolicy>& MovedPtr<T, OwnershipPolicy>::operator= (detail::MovedPtrRef<T, OwnershipPolicy> ref)
{
	MovedPtr tmp(ref);
	Swap(tmp);
	return *this;
}

template <typename T, template <typename> class OwnershipPolicy>
MovedPtr<T, OwnershipPolicy>::operator detail::MovedPtrRef<T, OwnershipPolicy> ()
{
	// Create object carrier which takes ownership (releases mImpl)
	return detail::MovedPtrRef<T, OwnershipPolicy>(mImpl);
}

// ---------------------------------------------------------------------------------------------------------------------------


template <typename T, template <typename> class OwnershipPolicy>
MovedPtr<T, OwnershipPolicy> Copy(const MovedPtr<T, OwnershipPolicy>& origin)
{
	// Copy contents of source without the loss of ownership
	MovedPtr<T, OwnershipPolicy> copy;
	detail::SmartPtrAccessor::GetImpl<T, OwnershipPolicy>(origin).CopyTo( 
		detail::SmartPtrAccessor::GetImpl<T, OwnershipPolicy>(copy) );
	
	return copy;
}

template <typename T, template <typename> class OwnershipPolicy>
void swap(MovedPtr<T, OwnershipPolicy>& lhs, MovedPtr<T, OwnershipPolicy>& rhs)
{
	lhs.Swap(rhs);
}

template <typename T, template <typename> class OwnershipPolicy>
bool operator== (const MovedPtr<T, OwnershipPolicy>&, const MovedPtr<T, OwnershipPolicy>&)
{
	// Error: Comparison is not allowed (because every MovedPtr is unique).
	// The operator must still exist to prevent conversion to SafeBool. 
	THOR_STATIC_ASSERT(sizeof(T) == 0);
	return false;
}

template <typename T, template <typename> class OwnershipPolicy>
bool operator!= (const MovedPtr<T, OwnershipPolicy>&, const MovedPtr<T, OwnershipPolicy>&)
{
	// Error: Comparison is not allowed (because every MovedPtr is unique).
	// The operator must still exist to prevent conversion to SafeBool. 
	THOR_STATIC_ASSERT(sizeof(T) == 0);
	return false;
}

} // namespace thor
