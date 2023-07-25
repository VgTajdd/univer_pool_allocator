#include <gtest/gtest.h>

#include "univer_pool_allocator/MemoryPoolContainer.h"
#include "univer_pool_allocator/MemoryPoolAllocator.h"
#include "univer_pool_allocator/PoolObject.h"
#include "Event.h"

using univer::memory::MemoryPoolChunk;
using univer::memory::MemoryPoolContainer;
using univer::memory::MemoryPoolAllocator;
using univer::memory::PoolObject;
using univer::memory::PoolObjectRAII;

TEST( TestMemoryPoolChunk, Capacity )
{
	constexpr std::size_t chunkSize{ 256 };
	constexpr std::size_t halfChunkSize{ chunkSize / 2 };
	MemoryPoolChunk<intptr_t, chunkSize> allocator;

	for ( int i = 0; i < halfChunkSize; i++ )
	{
		allocator.allocate();
	}

	float capacity{ (float) allocator.allocatedCount() / allocator.capacity() };

	EXPECT_EQ( capacity, 0.5f );
}

TEST( TestMemoryPoolChunk, Exhaustion )
{
	constexpr std::size_t chunkSize{ 256 };
	MemoryPoolChunk<intptr_t, chunkSize> allocator;

	for ( int i = 0; i < chunkSize; i++ )
	{
		EXPECT_NE( allocator.allocate(), nullptr );
	}

	EXPECT_EQ( allocator.allocate(), nullptr );
}

TEST( TestMemoryPoolAllocator, TestStdVectorEmptyAndPush )
{
	std::vector<Event, MemoryPoolAllocator<Event>> v;
	v.push_back( Event() );
	const size_t size{ v.size() };
	EXPECT_EQ( size, 1 );
}

TEST( TestMemoryPoolAllocator, TestStdVectorPush )
{
	std::vector<Event, MemoryPoolAllocator<Event>> v( 8 );
	v.push_back( Event() );
	const size_t size{ v.size() };
	EXPECT_EQ( size, 9 );
}

TEST( TestMemoryPoolContainer, TestTwoChunks )
{
	constexpr std::size_t chunkSize{ 256 };
	MemoryPoolContainer<Event, chunkSize> container;
	std::vector<Event*> v1, v2;
	for ( int i = 0; i < chunkSize; i++ )
	{
		auto a{ container.allocate() };
		v1.push_back( a );
	}
	EXPECT_EQ( container.capacity(), chunkSize );
	EXPECT_EQ( container.allocatedCount(), chunkSize );
	for ( int i = 0; i < chunkSize; i++ )
	{
		auto a{ container.allocate() };
		v2.push_back( a );
	}
	EXPECT_EQ( container.capacity(), 2 * chunkSize );
	EXPECT_EQ( container.allocatedCount(), 2 * chunkSize );
	for ( int i = 0; i < chunkSize; i++ )
	{
		container.deallocate( v1[i] );
	}
	EXPECT_EQ( container.capacity(), 2 * chunkSize );
	EXPECT_EQ( container.allocatedCount(), chunkSize );
	v1.clear();
	container.deallocate( v2[0] );
	EXPECT_EQ( container.allocatedCount(), chunkSize - 1 );
	for ( int i = 0; i < chunkSize; i++ )
	{
		auto a{ container.allocate() };
		v1.push_back( a );
	}
	EXPECT_EQ( container.capacity(), 2 * chunkSize );
	EXPECT_EQ( container.allocatedCount(), -1 + 2 * chunkSize );
}

TEST( PoolObjectRAII, TestWrapper )
{
	PoolObjectRAII<Event> event;
	EXPECT_TRUE( event ? true : false );
}
