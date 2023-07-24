#pragma once

#include "MemoryPoolChunk.h"

#ifdef DEBUG
#define PRINT_ACTIVITY
#endif

#ifdef PRINT_ACTIVITY
#include <iostream>
#endif

namespace univer::memory
{
/**
* @brief This class implements a memory pool container (contains chunks).
*
* @tparam T Template type.
* @tparam ChunkCapacity The capacity of the chunk allocator (number of elements, not bytes).
*/
template<typename T, size_t ChunkCapacity = 256>
class MemoryPoolContainer
{
	typedef MemoryPoolChunk<T, ChunkCapacity> Chunk;

public:
	MemoryPoolContainer()
	{
		m_head = createNewChunk(); // Preallocate a chunk. This is optional.
		m_current = m_head;
	}

	~MemoryPoolContainer()
	{
		Chunk* nextToDelete = nullptr;
		while ( m_head != nullptr )
		{
			nextToDelete = m_head->next();
			delete m_head;
			m_head = nextToDelete;
		}
	}

	T* allocate()
	{
		T* allocated = nullptr;
		if ( m_current != nullptr )
		{
			allocated = static_cast<T*>( m_current->allocate() );
		}
		if ( allocated == nullptr )
		{
			auto allocator{ m_head };
			while ( allocator != nullptr )
			{
				allocated = static_cast<T*>( allocator->allocate() );
				if ( allocated ) break;
				allocator = allocator->next();
			}
		}
		if ( allocated == nullptr )
		{
			auto newAllocator{ createNewChunk() };
			m_current->setNext( newAllocator );
			m_current = newAllocator;
			allocated = static_cast<T*>( m_current->allocate() );
		}
		report( m_current, allocated, true );
		if ( allocated != nullptr )
		{
			m_allocatedCount++;
		}
		return allocated;
	}

	void deallocate( void* object )
	{
		auto chunk = m_head;
		while ( chunk != nullptr )
		{
			if ( chunk->contains( object ) )
			{
				chunk->deallocate( object );
				m_current = chunk;
				report( chunk, object, false );
				m_allocatedCount--;
				return;
			}
			chunk = chunk->next();
		}
	}

	size_t allocatedCount() const
	{
		return m_allocatedCount;
	}

	size_t capacity() const
	{
		return m_capacity;
	}


private:
	void report( Chunk* chunk, void* object, bool alloc ) const
	{
#ifdef PRINT_ACTIVITY
		std::cout << ( alloc ? "MemoryPoolContainer::allocate:  " : "MemoryPoolContainer::deallocate:" )
			<< " " << typeid( T ).name() << " ["
			<< chunk->beginAddress() << "] " << chunk->indexObject( object )
			<< "|" << ( 100.f ) * chunk->allocatedCount() / ChunkCapacity << std::endl;
#endif
	}

private:
	Chunk* createNewChunk() noexcept
	{
#ifdef PRINT_ACTIVITY
		std::cout << "MemoryPoolContainer::createNewChunk: " << typeid( T ).name() << std::endl;
#endif
		m_capacity += ChunkCapacity;
		return new Chunk();
	}

private:
	Chunk* m_current = nullptr;
	Chunk* m_head = nullptr;
	size_t m_allocatedCount = 0;
	size_t m_capacity = 0;
};
}
