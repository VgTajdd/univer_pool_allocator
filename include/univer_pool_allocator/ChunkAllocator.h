#pragma once

#include <cstdint>
#include <cstdlib>
#ifdef DEBUG
#include <cassert>
#endif

// TODO: Change typename T to size_t ElementSize.

namespace univer::memory
{

/**
 * @brief
 *
 * @tparam T Template type.
 * @tparam Capacity The capacity of the chunk allocator (number of elements, not bytes).
 */
template<typename T, size_t Capacity = 256>
class ChunkAllocator
{
public:
	ChunkAllocator()
	{
#ifdef DEBUG
		assert( sizeof( T ) >= sizeof( uintptr_t ) );
#endif
		if ( sizeof( T ) < sizeof( uintptr_t ) )
		{
			std::cout << "[ChunkAllocator] Error: sizeof( T ) < sizeof( uintptr_t ). T:" << typeid( T ).name() << " is smaller than uintptr_t." << std::endl;
			return;
		}

		m_begin = malloc( sizeof( T ) * Capacity );

		if ( m_begin == nullptr ) return;

		m_head = static_cast<T*>( m_begin ) + Capacity - 1;
		uintptr_t* ptr{ reinterpretPointer( m_head ) };
		if ( ptr != nullptr ) *ptr = 0;

		for ( size_t i = 0; i < Capacity - 1; i++ )
		{
			auto new_ptr = static_cast<T*>( m_head ) - 1;
			deallocate( new_ptr );
		}

		m_allocatedCount = 0;
	}

	~ChunkAllocator()
	{
		if ( m_begin != nullptr )
		{
			free( m_begin );
		}
	}

	/**
	 * @brief This function allocates memory from the chunk allocator.
	 * This function is thread unsafe.
	 * Allocates 1 element of type T.
	 *
	 * @return void* The pointer to the allocated memory.
	 */
	void* allocate()
	{
		if ( m_head == nullptr ) return nullptr;
		void* head = m_head;
		m_head = next( m_head );
		m_allocatedCount++;
		return head;
	}

	/**
	 * @brief This function returns the ChunkAllocator object that comes next in the linked list.
	 *
	 * @return ChunkAllocator* A pointer to the next ChunkAllocator object.
	 */
	ChunkAllocator* next() const { return m_next; }

	/**
	 * @brief This function sets the next ChunkAllocator object in the linked list.
	 *
	 * @param next A pointer next to the next ChunkAllocator object.
	 */
	void setNext( ChunkAllocator* next ) { m_next = next; }

	/**
	 * @brief This function deallocates memory from the chunk allocator.
	 *
	 * @param object The pointer to the memory to be deallocated.
	 */
	void deallocate( void* object )
	{
		if ( object == nullptr ) return;
		uintptr_t* ptr{ reinterpretPointer( object ) };
		*ptr = toAddress( m_head );
		m_head = object;
		m_allocatedCount--;
	}

	uintptr_t beginAddress() const
	{
		return toAddress( m_begin );
	}

	size_t allocatedCount() const
	{
		return m_allocatedCount;
	}

	size_t count() const
	{
		return Capacity;
	}

	uintptr_t addressOffset( void* object ) const
	{
		auto objectPos = toAddress( object );
		auto beginPos = toAddress( m_begin );
		return objectPos - beginPos;
	}

	size_t indexObject( void* object ) const
	{
		return addressOffset( object ) / sizeof( T );
	}

	bool contains( void* object ) const
	{
		return addressOffset( object ) <= sizeof( T ) * Capacity;
	}

private:
	uintptr_t* reinterpretPointer( void* ptr )
	{
		return reinterpret_cast<uintptr_t*>( ptr );
	}

	void* next( void* object )
	{
		uintptr_t* ptr{ reinterpretPointer( object ) };
		return toPointer( *ptr );
	}

	uintptr_t toAddress( void* ptr ) const
	{
		return reinterpret_cast<uintptr_t>( ptr );
	}

	void* toPointer( uintptr_t address ) const
	{
		return reinterpret_cast<void*>( address );
	}

private:
	void* m_head = nullptr;
	void* m_begin = nullptr;
	size_t m_allocatedCount = 0;
	ChunkAllocator* m_next = nullptr; // TODO: Remove this.
};
}
