#include <gtest/gtest.h>

#include "univer_pool_allocator/MemoryPoolContainer.h"

TEST( TestChunkAllocator, Capacity )
{
	constexpr std::size_t chunkSize{ 256 };
	constexpr std::size_t halfChunkSize{ chunkSize / 2 };
	univer::memory::ChunkAllocator<intptr_t, chunkSize> allocator;

	for ( int i = 0; i < halfChunkSize; i++ )
	{
		allocator.allocate();
	}

	float capacity{ (float) allocator.allocatedCount() / allocator.count() };

	EXPECT_EQ( capacity, 0.5f );
}

TEST( TestChunkAllocator, Exhaustion )
{
	constexpr std::size_t chunkSize{ 256 };
	univer::memory::ChunkAllocator<intptr_t, chunkSize> allocator;

	for ( int i = 0; i < chunkSize; i++ )
	{
		EXPECT_NE( allocator.allocate(), nullptr );
	}

	EXPECT_EQ( allocator.allocate(), nullptr );
}
