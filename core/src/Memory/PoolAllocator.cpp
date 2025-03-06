#include <cstdlib>
#include <cstdint>
#include <iostream>

import Aurion.Memory;

namespace Aurion
{
	PoolAllocator::PoolAllocator(const size_t& chunk_count, const size_t& chunk_size)
		: m_start(nullptr), m_free_list(nullptr), m_chunk_count(chunk_count), m_chunk_size(chunk_size)
	{
		// Allocate the required amount of memory (minimum of 8 bytes per chunk for pointer storage)
		m_start = calloc(m_chunk_count, m_chunk_size);

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

	PoolAllocator::~PoolAllocator()
	{
		free(m_start);
		m_start = nullptr;
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
			return nullptr;

		void* allocation = m_free_list;

		m_free_list = (void**)(*m_free_list);

		// return the allocated memory
		return allocation;
	}

	void PoolAllocator::Free(void* ptr)
	{
		// Bounds Check
		if (ptr <= m_start || (size_t)ptr >= (size_t)m_start + (m_chunk_count * m_chunk_size))
			return;

		// Store the current free list in this chunk
		*((void**)ptr) = m_free_list;

		// Update the free list to point to the freed chunk
		m_free_list = (void**)ptr;
	}

	void PoolAllocator::Reset()
	{
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
}