#pragma once

#include "MemoryPoolContainer.h"

#ifdef DEBUG
#define PRINT_ACTIVITY
#endif

#ifdef PRINT_ACTIVITY
#include <iostream>
#endif

namespace univer::memory
{
/**
 * @brief Memory pool allocator.
 *
 * @tparam T Template type.
 */
template<class T>
struct MemoryPoolAllocator
{
	typedef T value_type;

	MemoryPoolAllocator() = default;

	template<class U>
	constexpr MemoryPoolAllocator( const MemoryPoolAllocator <U>& ) noexcept {}

	[[nodiscard]] T* allocate( std::size_t n )
	{
		if ( n > std::numeric_limits<std::size_t>::max() / sizeof( T ) )
			throw std::bad_array_new_length();

		if ( n == 1 )
		{
			auto p = s_pool.allocate();
			return p;
		}

		if ( auto p = static_cast<T*>( std::malloc( n * sizeof( T ) ) ) )
		{
			report( p, n );
			return p;
		}

		throw std::bad_alloc();
	}

	void deallocate( T* p, std::size_t n ) noexcept
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
	void report( T* p, std::size_t n, bool alloc = true ) const
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
MemoryPoolContainer<T>  MemoryPoolAllocator<T>::s_pool;

template<class T, class U>
bool operator==( const MemoryPoolAllocator <T>&, const MemoryPoolAllocator <U>& ) { return true; }

template<class T, class U>
bool operator!=( const MemoryPoolAllocator <T>&, const MemoryPoolAllocator <U>& ) { return false; }
}
