#pragma once

#include "ChunkAllocator.h"

#ifdef DEBUG
#define PRINT_ACTIVITY
#endif

#ifdef PRINT_ACTIVITY
#include <iostream>
#endif

namespace univer::memory
{
template<typename T, size_t ChunkCapacity = 256>
class MemoryPoolContainer
{
	typedef ChunkAllocator<T, ChunkCapacity> Chunk;

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
#ifdef PRINT_ACTIVITY
		std::cout << "MemoryPoolContainer::allocate:   [" << m_current->beginAddress() << "] " << m_current->indexObject( allocated )
			<< "|" << ( 100.f ) * m_current->allocatedCount() / ChunkCapacity << std::endl;
#endif
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
#ifdef PRINT_ACTIVITY
				std::cout << "MemoryPoolContainer::deallocate: [" << chunk->beginAddress() << "] " << chunk->indexObject( object )
					<< "|" << ( 100.f ) * chunk->allocatedCount() / ChunkCapacity << std::endl;
#endif
				return;
			}
			chunk = chunk->next();
		}
	}

private:
	Chunk* m_current = nullptr;
	Chunk* m_head = nullptr;
};
}
