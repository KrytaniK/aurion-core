#include <macros/AurionLog.h>

#include <cstdlib>
#include <cstdint>
#include <iostream>

import Aurion.Memory;

namespace Aurion
{
	PoolAllocator::PoolAllocator()
		: m_start(nullptr), m_free_list(nullptr), m_chunk_count(0), m_chunk_size(0)
	{
		
	}

	PoolAllocator::~PoolAllocator()
	{
		free(m_start);
		m_start = nullptr;
	}

	PoolAllocator::PoolAllocator(PoolAllocator&& other)
	{
		m_start = other.m_start;
		m_free_list = other.m_free_list;
		m_chunk_count = other.m_chunk_count;
		m_chunk_size = other.m_chunk_size;
	}

	PoolAllocator& PoolAllocator::operator=(PoolAllocator&& other)
	{
		m_start = other.m_start;
		m_free_list = other.m_free_list;
		m_chunk_count = other.m_chunk_count;
		m_chunk_size = other.m_chunk_size;

		return *this;
	}

	void PoolAllocator::Initialize(const size_t& chunk_size, const size_t& chunk_count)
	{
		// Enforce 8 byte minimum (for pointer storage)
		if (chunk_size < 8 || chunk_count == 0)
			return;

		// Set internal chunk data
		m_chunk_count = chunk_count;
		m_chunk_size = chunk_size;

		// Use calloc to grab memory and initialize it to 0
		m_start = calloc(chunk_count, chunk_size);

		// The free list begins at the start of the allocated memory
		m_free_list = (void**)(m_start);

		// Use the first 8 bytes of each uninitialized chunk to store a pointer to the next uninitialized
		//	chunk.
		// A multiplier is required here for indexing, since for loops iterate by the size of the type of container (void*), not
		//	the size of the chunk, and we want to write to the beginning of each chunk.
		size_t multiplier = (m_chunk_size / sizeof(void*));
		for (size_t i = 0; i < m_chunk_count; i++)
			m_free_list[i * multiplier] = (void*)((size_t)m_start + (i + 1) * m_chunk_size);

		// Ensure the last block points to nothing.
		m_free_list[(m_chunk_count - 1) * multiplier] = nullptr;
	}

	void* PoolAllocator::Allocate(const size_t& size, const size_t& alignment)
	{
		if (!m_start || !m_free_list)
			return nullptr;

		// Size and alignment are ignored. Pool operates in fixed chunks
		return Allocate();
	}

	void* PoolAllocator::Allocate()
	{
		if (!m_start || !m_free_list)
		{
			AURION_ERROR("No More Room!");
			return nullptr;
		}

		void* allocation = m_free_list;

		m_free_list = (void**)(*m_free_list);

		// return the allocated memory
		return allocation;
	}

	void PoolAllocator::Free(void* ptr)
	{
		// Ensure the allocator has been initialized
		if (!m_start)
			return;

		// Bounds Check
		if ((size_t)ptr < (size_t)m_start || (size_t)ptr >= ((size_t)m_start + (m_chunk_count * m_chunk_size)))
			return;

		// Store the current free list in this chunk
		*((void**)ptr) = m_free_list;

		// Update the free list to point to the freed chunk
		m_free_list = (void**)ptr;
	}

	void PoolAllocator::Reset()
	{
		// Ensure the allocator has been initialized
		if (!m_start)
			return;

		// Note: If ANY object data still remains in the allocated memory region,
		//	then this function will overwrite the first 8 bytes of that data.

		// Return all memory back to the free list (no reallocations)
		m_free_list = (void**)(m_start);

		// Use the first 8 bytes of each unused chunk to store pointers to
		//	the next unused chunk
		size_t multiplier = (m_chunk_size / sizeof(void*));
		for (size_t i = 0; i < m_chunk_count; i++)
			m_free_list[i * multiplier] = (void*)((size_t)m_start + (i + 1) * m_chunk_size);

		// Ensure the last block points to nothing.
		m_free_list[(m_chunk_count - 1) * multiplier] = nullptr;
	}

	bool PoolAllocator::IsMapped(void* ptr)
	{
		// Ensure the allocator has been initialized
		if (!m_start)
		{
			AURION_ERROR("Pool Allocator not initialized!");
			return false;
		}

		// Ensure the pointer is within the bounds of the allocated memory
		if ((size_t)ptr < (size_t)m_start || (size_t)ptr >= ((size_t)m_start + (m_chunk_count * m_chunk_size)))
		{
			AURION_ERROR("Attempted to access out of bounds memory!");
			return false;
		}

		// Copy and iterate through the free list. If the pointer doesn't exist in the free list,
		//	then that memory is mapped.
		void** current = m_free_list;
		while (current)
		{
			// If the pointer address matches, then the memory is not mapped
			if ((size_t)current == (size_t)ptr)
				return false;

			// Otherwise, continue iteration
			current = (void**)(*current);
		}

		return true;
	}
}