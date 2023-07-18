#include <gtest/gtest.h>

#include "univer_pool_allocator/MemoryPoolContainer.h"
#include "univer_pool_allocator/MemoryPoolAllocator.h"
#include "Event.h"

TEST( TestMemoryPoolChunk, Capacity )
{
	constexpr std::size_t chunkSize{ 256 };
	constexpr std::size_t halfChunkSize{ chunkSize / 2 };
	univer::memory::MemoryPoolChunk<intptr_t, chunkSize> allocator;

	for ( int i = 0; i < halfChunkSize; i++ )
	{
		allocator.allocate();
	}

	float capacity{ (float) allocator.allocatedCount() / allocator.count() };

	EXPECT_EQ( capacity, 0.5f );
}

TEST( TestMemoryPoolChunk, Exhaustion )
{
	constexpr std::size_t chunkSize{ 256 };
	univer::memory::MemoryPoolChunk<intptr_t, chunkSize> allocator;

	for ( int i = 0; i < chunkSize; i++ )
	{
		EXPECT_NE( allocator.allocate(), nullptr );
	}

	EXPECT_EQ( allocator.allocate(), nullptr );
}

TEST( TestMemoryPoolAllocator, TestStdVectorEmptyAndPush )
{
	std::vector<Event, univer::memory::MemoryPoolAllocator<Event>> v;
	v.push_back( Event() );
	const size_t size{ v.size() };
	EXPECT_EQ( size, 1 );
}

TEST( TestMemoryPoolAllocator, TestStdVectorPush )
{
	std::vector<Event, univer::memory::MemoryPoolAllocator<Event>> v( 8 );
	v.push_back( Event() );
	const size_t size{ v.size() };
	EXPECT_EQ( size, 9 );
}

TEST( TestMemoryPoolContainer, TestTwoChunks )
{
	std::vector<Event, univer::memory::MemoryPoolAllocator<Event>> v( 8 );
	v.push_back( Event() );
	const size_t size{ v.size() };
	EXPECT_EQ( size, 9 );
}

/*
Tests:
- TestMemoryPoolChunk
	- Capacity
	- Exhaustion
	- Allocation
	- Deallocation
	- Reuse
- PoolObjectRAII -> MemoryPoolContainer
	- Allocation
	- Deallocation
	- Reuse
*/
