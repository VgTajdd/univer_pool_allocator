// ========================================================================= //
// Copyright (c) 2023 Agustin Jesus Durand Diaz.                             //
// This code is licensed under the Apache License 2.0.                       //
// main.cpp                                                                  //
// ========================================================================= //

#include "univer_pool_allocator/MemoryPoolContainer.h"
#include "univer_pool_allocator/MemoryPoolAllocator.h"
#include "univer_pool_allocator/PoolObject.h"
#include "Event.h"

#include <memory>
#include <vector>

using univer::memory::MemoryPoolChunk;
using univer::memory::MemoryPoolContainer;
using univer::memory::MemoryPoolAllocator;
using univer::memory::PoolObject;
using univer::memory::PoolObjectRAII;

int main( int, char** )
{
	//! Usage of MemoryPoolChunk.
	{
		MemoryPoolChunk<Event> chunk;
		Event* a{ (Event*) chunk.allocate() }; // Allocates a new object but does not construct it.
		a = new( a )Event(); // Constructs the object.
	}

	//! Usage of MemoryPoolContainer.
	{
		MemoryPoolContainer<Event> container;
		Event* a{ container.allocate() }; // Allocates a new object but does not construct it.
		a = new( a )Event(); // Constructs the object.
	}

	//! Usage of MemoryPoolAllocator.
	{
		std::vector<Event, MemoryPoolAllocator<Event>> v;
		v.push_back( Event() ); // This allocates a new object using the MemoryPoolContainer.
		v.push_back( Event() ); // From this point, the std::malloc is used
		v.push_back( Event() );
		v.push_back( Event() );
	}
	// In this example the internal MemoryPoolContainer is never used.
	{
		std::vector<Event, MemoryPoolAllocator<Event>> v( 8 );
		v.push_back( Event() );
	}
	// In this example we are using directly the allocator to allocate/deallocate a single object.
	{
		MemoryPoolAllocator<Event> allocator;
		Event* e{ allocator.allocate( 1 ) };
		allocator.deallocate( e, 1 );
	}

	//! Usage of PoolObject<T>.
	{
		PoolObject<Event>* event = new PoolObject<Event>; // Allocates a new object in the internal memory pool and construct it.
		bool handled = event->isHandled(); // Do your stuff.
		delete event; // Deallocates the object from the container.
	}

	//! Usage of PoolObjectRAII<T>.
	{
		PoolObjectRAII<Event> e;
		e->isHandled();
	}
	// In this example we will move the contents of event1 to event2.
	{
		PoolObjectRAII<Event2> event1( false, true );
		PoolObjectRAII<Event2> event2( false );
		event1->isHandled();
		event2->isHandled();
		event2 = std::move( event1 ); // This calls the destructor of the previous object handled by event2.
	}

	//! Alternative std::vector implementations.
	{
		// Here the vector stores the pointers (integers) to the allocated objects,
		// and the actual Event objects are allocated in the internal memory pool.
		std::vector<PoolObject<Event>*> v;
		v.push_back( new PoolObject<Event> );
		v.push_back( new PoolObject<Event> );
		v.push_back( new PoolObject<Event> );
		for ( auto& e : v ) delete e;
	}

	//! Missuses of PoolObject<T>.
	{
		// This example creates 2 MemoryPoolContainer:
		// a MemoryPoolContainer<Event> and a MemoryPoolContainer<PoolObjectRAII<Event>>.
		std::vector<PoolObjectRAII<Event>, MemoryPoolAllocator<PoolObjectRAII<Event>>> v;
		v.push_back( PoolObjectRAII<Event>() );
		v.push_back( PoolObjectRAII<Event>() );
		v.push_back( PoolObjectRAII<Event>() );
	}

	return 0;
}
