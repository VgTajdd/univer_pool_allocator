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
		m_head = new Chunk(); // Preallocate a chunk. This is optional.
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
			allocated = (T*) m_current->allocate();
		}
		if ( allocated == nullptr )
		{
			while ( m_current->next() && allocated == nullptr )
			{
				allocated = (T*) m_current->next()->allocate();
				m_current = m_current->next();
				if ( allocated ) break;
			}
		}
		if ( allocated == nullptr )
		{
			auto newAllocator{ new Chunk() };
			m_current->setNext( newAllocator );
			m_current = newAllocator;
			allocated = (T*) m_current->allocate();
		}
		report( m_current, allocated, true );
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
				return;
			}
			chunk = chunk->next();
		}
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
	Chunk* m_current = nullptr;
	Chunk* m_head = nullptr;
};
}
