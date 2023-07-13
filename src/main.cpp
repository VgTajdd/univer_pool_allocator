#include "pkg/pkg.h"

#include "univer_pool_allocator/MemoryPoolContainer.h"
#include "univer_pool_allocator/MemoryPoolAllocator.h"
#include "EventFromPool.h"

#include <memory>
#include <vector>

int main( int, char** )
{
	pkg::printHelloWorld();
	pkg::printNumber( pkg::getFibonacci( 5 ) );

	{
		{
			auto a{ new EventFromPool };
			auto b{ new EventFromPool };
			auto c{ new EventFromPool };
			delete c;
			delete b;
			delete a;
		}
		{
			auto d{ new EventFromPool };
			auto e{ new EventFromPool };
			delete d;
			delete e;
		}
		{
			// auto e{ std::make_shared<EventFromPool>( new EventFromPool ) }; -> Uses placement new.
			std::shared_ptr<EventFromPool> a( new EventFromPool );
		}
		{
			std::vector < std::shared_ptr<EventFromPool> > pointers;
			for ( int i = 0; i < 500; i++ )
			{
				std::shared_ptr<EventFromPool> a( new EventFromPool );
				pointers.push_back( a );
			}
		}
		{
			MemoryPoolContainer<EventFromPool> allocator;
			std::shared_ptr<EventFromPool> ptr1;
		}
		{
			univer::memory::MemoryPoolChunk<EventFromPool> allocator;
			for ( int i = 0; i < 128; i++ )
			{
				allocator.allocate();
			}
			allocator.deallocate( nullptr );
			float capacity{ (float) allocator.allocatedCount() / allocator.count() };
		}
		{
			std::vector<Event, univer::memory::MemoryPoolAllocator<Event>> v;
			v.push_back( Event() );
		}
		{
			std::vector<Event, univer::memory::MemoryPoolAllocator<Event>> v( 8 );
			v.push_back( Event() );
		}
		{
			univer::memory::MemoryPoolAllocator<Event> allocator;
			Event* e{ allocator.allocate( 1 ) };
			allocator.deallocate( e, 1 );
		}
	}

	return 0;
}
