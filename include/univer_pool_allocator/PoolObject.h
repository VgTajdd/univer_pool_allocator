#pragma once

#include "MemoryPoolContainer.h"

#include <utility>

using univer::memory::MemoryPoolContainer;

namespace univer::memory
{
template<typename T>
class PoolObject : public T
{
public:
	template <typename ...Args>
	PoolObject( Args && ...args ) : T( std::forward<Args>( args )... ) {}

	void* operator new( size_t size )
	{
		auto ptr{ allocator.allocate() };
		return ptr;
	}

	void operator delete( void* e )
	{
		if ( e == nullptr ) return;
		allocator.deallocate( e );
	}

	template <typename ...Args>
	static PoolObject<T>* Create( Args && ...args )
	{
		return new PoolObject<T>( std::forward<Args>( args )... );
	}

private:
	static MemoryPoolContainer<T> allocator;
};

template<typename T>
MemoryPoolContainer<T> PoolObject<T>::allocator;

template<typename T>
class PoolObjectRAII
{
public:
	template <typename ...Args>
	PoolObjectRAII( Args && ...args )
	{
		object = new PoolObject<T>( std::forward<Args>( args )... );
	}

	~PoolObjectRAII()
	{
		delete object; object = nullptr;
	}

	PoolObjectRAII( const PoolObjectRAII& other ) = delete;
	PoolObjectRAII& operator=( const PoolObjectRAII& other ) = delete;

	PoolObjectRAII( PoolObjectRAII&& other )
	{
		if ( object != nullptr )
		{
			delete object; object = nullptr;
		}
		object = other.object;
		other.object = nullptr;
	}

	PoolObjectRAII& operator=( PoolObjectRAII&& rhs )
	{
		if ( object != nullptr )
		{
			delete object; object = nullptr;
		}
		object = rhs.object;
		rhs.object = nullptr;
		return *this;
	}

	PoolObject<T>* operator->() { return object; }
	operator bool() { return object != nullptr; }

private:
	PoolObject<T>* object = nullptr;
};
}
