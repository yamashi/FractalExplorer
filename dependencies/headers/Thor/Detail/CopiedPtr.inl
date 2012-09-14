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
CopiedPtr<T, OwnershipPolicy>::CopiedPtr()
: mImpl()
{
}

template <typename T, template <typename> class OwnershipPolicy>
template <typename U>
CopiedPtr<T, OwnershipPolicy>::CopiedPtr(U* pointer)
: mImpl(pointer)
{
}

template <typename T, template <typename> class OwnershipPolicy>
CopiedPtr<T, OwnershipPolicy>::CopiedPtr(const CopiedPtr& origin)
: mImpl()
{
	// If origin is NULL, *this becomes NULL as well; otherwise, the object is copied.
	origin.mImpl.CopyTo(mImpl);
}

template <typename T, template <typename> class OwnershipPolicy>
CopiedPtr<T, OwnershipPolicy>::CopiedPtr(MovedPtr<T, OwnershipPolicy> origin)
: mImpl()
{
	swap(mImpl, detail::SmartPtrAccessor::GetImpl<T, OwnershipPolicy>(origin));
}

template <typename T, template <typename> class OwnershipPolicy>
CopiedPtr<T, OwnershipPolicy>::~CopiedPtr()
{
	// mImpl destroys and deallocates the pointer
}

template <typename T, template <typename> class OwnershipPolicy>
CopiedPtr<T, OwnershipPolicy>& CopiedPtr<T, OwnershipPolicy>::operator= (const CopiedPtr& origin)
{
	CopiedPtr tmp(origin);
	Swap(tmp);
	return *this;
}

template <typename T, template <typename> class OwnershipPolicy>
CopiedPtr<T, OwnershipPolicy>& CopiedPtr<T, OwnershipPolicy>::operator= (MovedPtr<T, OwnershipPolicy> source)
{
	CopiedPtr tmp(source);
	Swap(tmp);
	return *this;
}

template <typename T, template <typename> class OwnershipPolicy>
void CopiedPtr<T, OwnershipPolicy>::Swap(CopiedPtr<T, OwnershipPolicy>& other)
{
	swap(mImpl, other.mImpl);
}

template <typename T, template <typename> class OwnershipPolicy>
void CopiedPtr<T, OwnershipPolicy>::Reset()
{
	CopiedPtr tmp;
	Swap(tmp);
}

template <typename T, template <typename> class OwnershipPolicy>
template <typename U>
void CopiedPtr<T, OwnershipPolicy>::Reset(U* pointer)
{
	CopiedPtr tmp(pointer);
	Swap(tmp);
}

template <typename T, template <typename> class OwnershipPolicy>
T* CopiedPtr<T, OwnershipPolicy>::Release()
{
	return mImpl.Release();
}

template <typename T, template <typename> class OwnershipPolicy>
CopiedPtr<T, OwnershipPolicy>::operator detail::SafeBool() const
{
	return detail::ToSafeBool(!mImpl.IsNull());
}

template <typename T, template <typename> class OwnershipPolicy>
T& CopiedPtr<T, OwnershipPolicy>::operator* () const
{
	return mImpl.operator*();
}

template <typename T, template <typename> class OwnershipPolicy>
T* CopiedPtr<T, OwnershipPolicy>::operator-> () const
{
	return mImpl.operator->();
}

template <typename T, template <typename> class OwnershipPolicy>
T* CopiedPtr<T, OwnershipPolicy>::Get() const
{
	return mImpl.Get();
}

// ---------------------------------------------------------------------------------------------------------------------------


template <typename T, template <typename> class OwnershipPolicy>
MovedPtr<T, OwnershipPolicy> Move(CopiedPtr<T, OwnershipPolicy>& source)
{
	// Move semantics through swapping
	MovedPtr<T, OwnershipPolicy> dest;
	swap(
		detail::SmartPtrAccessor::GetImpl<T, OwnershipPolicy>(dest), 
		detail::SmartPtrAccessor::GetImpl<T, OwnershipPolicy>(source));
	return dest;
}

template <typename T, template <typename> class OwnershipPolicy>
MovedPtr<T, OwnershipPolicy> Copy(const CopiedPtr<T, OwnershipPolicy>& origin)
{
	// Create copy of source and move its contents
	CopiedPtr<T, OwnershipPolicy> copy(origin);
	return Move(copy);
}

template <typename T, template <typename> class OwnershipPolicy>
void swap(CopiedPtr<T, OwnershipPolicy>& lhs, CopiedPtr<T, OwnershipPolicy>& rhs)
{
	lhs.Swap(rhs);
}

template <typename T, template <typename> class OwnershipPolicy>
bool operator== (const CopiedPtr<T, OwnershipPolicy>&, const CopiedPtr<T, OwnershipPolicy>&)
{
	// Error: Comparison is not allowed (because every CopiedPtr is unique).
	// The operator must still exist to prevent conversion to SafeBool. 
	THOR_STATIC_ASSERT(sizeof(T) == 0);
	return false;
}

template <typename T, template <typename> class OwnershipPolicy>
bool operator!= (const CopiedPtr<T, OwnershipPolicy>&, const CopiedPtr<T, OwnershipPolicy>&)
{
	// Error: Comparison is not allowed (because every CopiedPtr is unique).
	// The operator must still exist to prevent conversion to SafeBool. 
	THOR_STATIC_ASSERT(sizeof(T) == 0);
	return false;
}

} // namespace thor
