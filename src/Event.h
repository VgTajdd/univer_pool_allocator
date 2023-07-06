#pragma once

#include "univer_pool_allocator/MemoryPoolContainer.h"

// #include <iostream>
// #include <cassert>

using univer::memory::MemoryPoolContainer;

class EventPimpl
{
public:
	EventPimpl()
	{}

	void* operator new( size_t size )
	{
		auto ptr{ allocator.allocate() };
		// std::cout << "allocating in the pool:    " << ptr << std::endl;
		return ptr;
	}

	void operator delete( void* e )
	{
		// assert( e != nullptr );
		if ( e == nullptr ) return;
		// std::cout << "deallocating to the pool:  " << e << std::endl;
		allocator.deallocate( e );
	}

private:
	bool m_handled = false;
	const EventPimpl* m_parent;
	bool m_isMouseEvent = false;
	bool m_isKeyboardEvent = false;

	static MemoryPoolContainer<EventPimpl> allocator;
};

MemoryPoolContainer<EventPimpl> EventPimpl::allocator;
