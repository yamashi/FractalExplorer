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
/// @brief Class template thor::MovedPtr

#ifndef THOR_MOVEDPTR_HPP
#define THOR_MOVEDPTR_HPP

#include <Thor/Detail/MovedPtrRef.hpp>
#include <Thor/Detail/SmartPtrImpl.hpp>
#include <Thor/Config.hpp>

#include <algorithm>
#include <cassert>


namespace thor
{

template <typename T, template <typename> class OwnershipPolicy>
class CopiedPtr;

/// @addtogroup SmartPtr
/// @{

/// @brief Smart pointer with move semantics.
/// @tparam T The pointee type
/// @tparam OwnershipPolicy Only necessary if you plan to transfer ownership to a CopiedPtr. See its declaration for more
///  information.
/// @details This type of smart pointer allows movable objects - that is, copies and assignments transfer ownership.
///  Because of that, these operations modify the origin, which can be counter-intuitive, as it doesn't behave like usual
///  value semantics. Besides, every instance is unique, no two MovedPtr objects can point to the same object (but both
///  can point to NULL).
/// @n@n Why another movable smart pointer, if there's already std::auto_ptr? There are several reasons:
///  - At auto_ptr, the pointee type T must be known at declaration (in fact, some implementations don't force it to be
///    known, resulting in no destruction of the object when using delete, what's even worse).
///  - No method or conversion operator to check for NULL (get()'s purpose is to obtain the internal pointer)
///  - No %swap() method
///  - std::auto_ptr is not smart enough to save the dynamic type (useful for polymorphic copies)
///  - Different naming convention, which complicates the interaction between different smart pointer types
///  - std::auto_ptr is deprecated in C++11 (but std::unique_ptr is not available for C++98)
template <typename T, template <typename> class OwnershipPolicy = DynamicCopy>
class MovedPtr
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		/// @details Initializes the smart pointer with a null pointer. This operation doesn't require T to be defined.
									MovedPtr();

		/// @brief Constructor from raw pointer
		/// @param pointer The initial pointer value, either pointing to an object allocated by the @a new operator or to @a NULL.
		///  In case you use this MovedPtr with the DynamicCopy policy, ensure that the static and dynamic type of the passed 
		///  pointer are equal (therefore, don't pass base class pointers that actually refer to derived classes).
		/// @tparam U Type of the assigned object, where U* is implicitly convertible to T*.
		template <typename U>
		explicit					MovedPtr(U* pointer);
		
		/// @brief Move constructor
		/// @details Transfers ownership from @a origin to @a *this. After this operation, @a origin is empty.
									MovedPtr(MovedPtr& origin);
											
		/// @brief Destructor
		/// @details Destroys the object and deallocates the corresponding memory. For null pointers, nothing happens.
									~MovedPtr();
		
		/// @brief Move assignment operator
		/// @details Transfers ownership from @a origin to @a *this. After this operation, @a origin is empty.
		MovedPtr&					operator= (MovedPtr& origin);
		
		/// @brief Swaps the pointers of @a *this and @a other.
		/// @details Swap() doesn't operate on the object itself, only on the pointer. T may be incomplete at invocation.
		void						Swap(MovedPtr& other);
		
		/// @brief Resets the internal pointer to @a NULL.
		/// @details Destroys the current object, and sets the internal pointer to @a NULL.  This operation doesn't require T to
		///  be defined.
		void						Reset();
			
		/// @brief Resets the internal pointer to the passed value.
		/// @details Destroys the current object, and re-assigns the pointer.
		/// @param pointer The initial pointer value, either pointing to an object allocated by the @a new operator or to @a NULL.
		///  In case you use this MovedPtr with the DynamicCopy policy, ensure that the static and dynamic type of the passed 
		///  pointer are equal (therefore, don't pass base class pointers that actually refer to derived classes).
		/// @tparam U Type of the assigned object, where U* is implicitly convertible to T*.
		template <typename U>
		void						Reset(U* pointer);
		
		/// @brief Transfers ownership of the held object to the caller. 
		/// @details Upon invoking Release(), the caller becomes responsible for the memory management of the
		///  returned pointer. This instance points to null after the operation.
		T*							Release();
		
		/// @brief Checks whether the pointer currently points to @a NULL.
		/// @details This allows you to write statements of the form <i>if(ptr)</i> or <i>if(!ptr)</i> in a type-safe way
		///  (@a ptr refers to a smart-pointer instance). The actual return type is implementation-defined.
		/// @return bool-like value that behaves in conditional expressions like true, if the pointer is valid; 
		///  and like false, if it points to @a NULL.
									operator THOR_DETAIL SafeBool() const;

		/// @brief Dereferences the pointer.
		/// @pre The smart pointer must be non-empty.
		T&							operator* () const;
		
		/// @brief Dereferences the pointer for member access.
		/// @pre The smart pointer must be non-empty.
		T*							operator-> () const;
		
		/// @brief Permits access to the internal pointer. Designed for rare use.
		/// @return Internally used pointer, use it wisely not to upset the MovedPtr's memory management.
		T*							Get() const;
		
		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Implementation details
	public:
		// Constructor to allow interaction with function arguments and return values
									MovedPtr(detail::MovedPtrRef<T, OwnershipPolicy> ref);
	
		// Assignment operator to allow interaction with function arguments and return values
		MovedPtr&					operator= (detail::MovedPtrRef<T, OwnershipPolicy> ref);
								
		// Conversion operator to allow interaction with function arguments and return values
									operator detail::MovedPtrRef<T, OwnershipPolicy> ();


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		detail::SmartPtrImpl<T, OwnershipPolicy>	mImpl;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Friends
	friend struct detail::SmartPtrAccessor;
};

/// @relates MovedPtr
/// @brief Creates a %MovedPtr from another %MovedPtr by copying ownership.
/// @param origin The original smart pointer whose ownership is copied.
/// @return MovedPtr acting as carrier for the copied object. The returned smart pointer contains a copy of origin.
/// @code
///  thor::MovedPtr<MyClass, thor::StaticCopy> a(new MyClass);
///  thor::MovedPtr<MyClass, thor::StaticCopy> b = thor::Copy(a);
///  // a is unchanged, b has a copy of a's object.
/// @endcode
template <typename T, template <typename> class OwnershipPolicy>
MovedPtr<T, OwnershipPolicy> Copy(const MovedPtr<T, OwnershipPolicy>& origin);

/// @relates MovedPtr
/// @brief Swaps two MovedPtr<T, OwnershipPolicy> instances.
template <typename T, template <typename> class OwnershipPolicy>
void swap(MovedPtr<T, OwnershipPolicy>& lhs, MovedPtr<T, OwnershipPolicy>& rhs);

/// @}

} // namespace thor

#include <Thor/Detail/MovedPtr.inl>
#endif // THOR_MOVEDPTR_HPP
