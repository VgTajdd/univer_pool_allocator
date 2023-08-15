// ========================================================================= //
// Copyright (c) 2023 Agustin Jesus Durand Diaz.                             //
// This code is licensed under the Apache License 2.0.                       //
// PoolObject.h                                                              //
// ========================================================================= //

#pragma once

#include "MemoryPoolContainer.h"

#include <utility>

using univer::memory::MemoryPoolContainer;

namespace univer::memory
{
/**
 * @brief is a wrapper around a T object.
 * PoolObject<T> creates a static MemoryPoolContainer for each different type T.
 * This MemoryPoolContainer is used to allocate and deallocate objects of type T (1 object at a time).
 * PoolObject<T> overrides the default operator new and operator delete to use the container.
 *
 * @tparam T the type of the object to wrap.
 */
template<typename T>
class PoolObject : public T
{
public:
	template <typename ...Args>
	PoolObject( Args && ...args ) : T( std::forward<Args>( args )... ) {}

	void* operator new( size_t size )
	{
		auto ptr{ container.allocate() };
		return ptr;
	}

	void operator delete( void* e )
	{
		if ( e == nullptr ) return;
		container.deallocate( e );
	}

	template <typename ...Args>
	static PoolObject<T>* Create( Args && ...args )
	{
		return new PoolObject<T>( std::forward<Args>( args )... );
	}

private:
	static MemoryPoolContainer<T> container;
};

template<typename T>
MemoryPoolContainer<T> PoolObject<T>::container;

/**
 * @brief This class is a RAII wrapper around PoolObject<T>.
 * This behaviour is similar to an unique_ptr.
 *
 * @tparam T the type of the object to wrap.
 */
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