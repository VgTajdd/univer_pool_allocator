#pragma once

#include "univer_pool_allocator/MemoryPoolContainer.h"
#include "Event.h"

using univer::memory::MemoryPoolContainer;

class EventFromPool : public Event
{
public:
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

private:
	static MemoryPoolContainer<EventFromPool> allocator;
};

MemoryPoolContainer<EventFromPool> EventFromPool::allocator;
