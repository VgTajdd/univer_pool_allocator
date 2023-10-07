// ========================================================================= //
// Copyright (c) 2023 Agustin Jesus Durand Diaz.                             //
// This code is licensed under the Apache License 2.0.                       //
// MemoryPoolAllocator.h                                                     //
// ========================================================================= //

#pragma once

#include "MemoryPoolContainer.h"

#ifdef DEBUG
#define PRINT_ACTIVITY
#endif

#ifdef PRINT_ACTIVITY
#include <iostream>
#else
#include <new>
#include <limits>
#endif

namespace univer::memory
{
/**
 * @brief Memory pool allocator.
 * MemoryPoolAllocator<T> creates a static MemoryPoolContainer for each different type T.
 * This MemoryPoolContainer is used to allocate and deallocate objects of type T (1 object at a time).
 * If more than one object of type T is needed in a singe allocation, std::malloc is used.
 *
 * @tparam T Type of the objects.
 */
template<class T>
class MemoryPoolAllocator
{
public:
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;

	MemoryPoolAllocator() = default;

	template<class U>
	constexpr MemoryPoolAllocator( const MemoryPoolAllocator<U>& ) noexcept {}

	/**
	 * @brief This function allocates n elements of type T.
	 *
	 * @param n Number of elements.
	 * @return pointer Pointer to the allocated memory.
	 */
	[[nodiscard]] pointer allocate( std::size_t n )
	{
		if ( n > std::numeric_limits<std::size_t>::max() / sizeof( T ) )
			throw std::bad_array_new_length();

		if ( n == 1 )
		{
			auto p = s_pool.allocate();
			return p;
		}

		if ( auto p = static_cast<pointer>( std::malloc( n * sizeof( T ) ) ) )
		{
			report( p, n );
			return p;
		}

		throw std::bad_alloc();
	}

	/**
	 * @brief This function deallocates n elements of type T.
	 *
	 * @param p Pointer to the memory.
	 * @param n Number of elements.
	 */
	void deallocate( pointer p, std::size_t n ) noexcept
	{
		if ( n == 1 )
		{
			s_pool.deallocate( p );
			return;
		}
		report( p, n, 0 );
		std::free( p );
	}
private:
	void report( pointer p, std::size_t n, bool alloc = true ) const
	{
#ifdef PRINT_ACTIVITY
		std::cout << "[MemoryPoolAllocator] "
			<< ( alloc ? "Alloc: [" : "Dealloc: [" ) << n << " elements "
			<< typeid( T ).name() << "] " << sizeof( T ) * n
			<< " bytes at " << std::hex << std::showbase
			<< reinterpret_cast<void*>( p ) << std::dec << '\n';
#endif
	}

	static MemoryPoolContainer<T> s_pool;
};

template<class T>
MemoryPoolContainer<T> MemoryPoolAllocator<T>::s_pool;

template<class T, class U>
bool operator==( const MemoryPoolAllocator<T>&, const MemoryPoolAllocator<U>& ) { return true; }

template<class T, class U>
bool operator!=( const MemoryPoolAllocator<T>&, const MemoryPoolAllocator<U>& ) { return false; }
}
