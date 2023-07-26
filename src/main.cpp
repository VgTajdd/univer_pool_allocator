#include "pkg/pkg.h"

#include "univer_pool_allocator/MemoryPoolContainer.h"
#include "univer_pool_allocator/MemoryPoolAllocator.h"
#include "univer_pool_allocator/PoolObject.h"
#include "EventFromPool.h"

#include <memory>
#include <vector>

using univer::memory::MemoryPoolChunk;
using univer::memory::MemoryPoolContainer;
using univer::memory::MemoryPoolAllocator;
using univer::memory::PoolObject;
using univer::memory::PoolObjectRAII;

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
			MemoryPoolChunk<EventFromPool> allocator;
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
	}

	return 0;
}
