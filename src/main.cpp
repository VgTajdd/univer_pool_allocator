#include "pkg/pkg.h"

#include "univer_pool_allocator/PoolAllocator.h"
#include "Event.h"

#include <memory>
#include <vector>

int main( int, char** )
{
	pkg::printHelloWorld();
	pkg::printNumber( pkg::getFibonacci( 5 ) );

	{
		{
			auto a{ new EventPimpl };
			auto b{ new EventPimpl };
			auto c{ new EventPimpl };
			delete c;
			delete b;
			delete a;
		}
		{
			auto d{ new EventPimpl };
			auto e{ new EventPimpl };
			delete d;
			delete e;
		}
		{
			// auto e{ std::make_shared<EventPimpl>( new EventPimpl ) }; -> Uses placement new.
			std::shared_ptr<EventPimpl> a( new EventPimpl );
		}
		{
			std::vector < std::shared_ptr<EventPimpl> > pointers;
			for ( int i = 0; i < 500; i++ )
			{
				std::shared_ptr<EventPimpl> a( new EventPimpl );
				pointers.push_back( a );
			}
		}
		{
			PoolAllocator<EventPimpl> allocator;
			std::shared_ptr<EventPimpl> ptr1;
		}
	}

	return 0;
}
