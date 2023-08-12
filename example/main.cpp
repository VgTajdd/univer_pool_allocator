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

#if 0
	{
		auto a{ new PoolObject<Event> };
		auto b{ new PoolObject<Event> };
		auto c{ new PoolObject<Event> };
		delete c;
		delete b;
		delete a;
	}
	{
		auto d{ new PoolObject<Event> };
		auto e{ new PoolObject<Event> };
		delete d;
		delete e;
	}
	{
		// auto e{ std::make_shared<PoolObject<Event>>( new PoolObject<Event> ) }; -> Uses placement new.
		std::shared_ptr<PoolObject<Event>> a( new PoolObject<Event> );
	}
	{
		std::vector < std::shared_ptr<PoolObject<Event>> > pointers;
		for ( int i = 0; i < 500; i++ )
		{
			std::shared_ptr<PoolObject<Event>> a( new PoolObject<Event> );
			pointers.push_back( a );
		}
	}
	{
		MemoryPoolContainer<PoolObject<Event>> allocator;
		std::shared_ptr<PoolObject<Event>> ptr1;
	}
	{
		MemoryPoolChunk<PoolObject<Event>> allocator;
		for ( int i = 0; i < 128; i++ )
		{
			allocator.allocate();
		}
		allocator.deallocate( nullptr );
		float capacity{ (float) allocator.allocatedCount() / allocator.capacity() };
	}
	{
		std::vector<Event, MemoryPoolAllocator<Event>> v;
		v.push_back( Event() );
		v.push_back( Event() );
		v.push_back( Event() );
		v.push_back( Event() );
	}
	{
		std::vector<Event, MemoryPoolAllocator<Event>> v( 8 );
		v.push_back( Event() );
	}
	{
		MemoryPoolAllocator<Event> allocator;
		Event* e{ allocator.allocate( 1 ) };
		allocator.deallocate( e, 1 );
	}
	{
		// std::shared_ptr<PoolObject<Event>> event( new PoolObject<Event> );
		// std::shared_ptr<PoolObject<Event>> event = std::make_shared<PoolObject<Event>>();
		// std::unique_ptr<PoolObject<Event>> event = std::make_unique<PoolObject<Event>>();
		PoolObject<Event>* event = new PoolObject<Event>;
		event->isHandled();
		delete event;
	}
	{
		PoolObjectRAII<Event> event;
		event->isHandled();
	}
	{
		PoolObjectRAII<Event2> event1( false, true );
		PoolObjectRAII<Event2> event2( false );
		event1->isHandled();
		event2->isHandled();
		event2 = std::move( event1 );
	}
	{
		std::vector<PoolObjectRAII<Event2>> v;
		v.push_back( PoolObjectRAII<Event2>( false, true ) );
		v.push_back( PoolObjectRAII<Event2>( false, true ) );
		v.push_back( PoolObjectRAII<Event2>( false, true ) );
		v.push_back( PoolObjectRAII<Event2>( false, true ) );
		const auto& a{ v[0] };
	}
#endif

	return 0;
}
