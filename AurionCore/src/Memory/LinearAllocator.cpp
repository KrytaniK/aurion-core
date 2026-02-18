module;

#include <cstdlib>

module Aurion.Memory;


namespace Aurion
{
	LinearAllocator::LinearAllocator()
		: m_start(nullptr), m_offset(0), m_max_offset(0)
	{
		
	}

	LinearAllocator::~LinearAllocator()
	{
		free(m_start);
		m_start = nullptr;
	}

	LinearAllocator::LinearAllocator(LinearAllocator&& other)
	{
		m_start = other.m_start;
		m_offset = other.m_offset;
		m_max_offset = other.m_max_offset;
	}

	LinearAllocator& LinearAllocator::operator=(LinearAllocator&& other)
	{
		m_start = other.m_start;
		m_offset = other.m_offset;
		m_max_offset = other.m_max_offset;

		return *this;
	}

	void LinearAllocator::Initialize(const size_t& chunk_size, const size_t& chunk_count)
	{
		if (chunk_size == 0 || chunk_count == 0)
			return;

		// Set the max offset
		m_max_offset = chunk_size * chunk_count;

		// Use calloc to grab memory and initialize it to 0
		m_start = calloc(chunk_count, chunk_size);
	}

	void* LinearAllocator::Allocate(const size_t& size, const size_t& alignment)
	{
		if (!m_start)
			return nullptr;

		// Get the current address
		const size_t current_address = (size_t)(m_start) + m_offset;

		// If alignment differs from what the current alignment is, padding
		//	is required.
		const size_t padding = (alignment - (current_address % alignment)) % alignment;
		
		// Enforce explicit upper bound
		if (m_offset + padding + size > m_max_offset)
			return nullptr;

		// Add padding and taret size to current offset
		m_offset += padding + size;

		// Return the padded address
		return (void*)(current_address + padding);
	}

	void LinearAllocator::Free(void* ptr)
	{
		// Not needed, does nothing for the linear allocator
		return;
	}

	void LinearAllocator::Reset()
	{
		// Ensure the allocator has been initialized
		if (!m_start)
			return;

		m_offset = 0;
	}

	bool LinearAllocator::IsMapped(void* ptr)
	{
		// Ensure the allocator has been initialized
		if (!m_start)
			return false;

		// If the pointer is within the bounds of what's been allocated, then it has been
		//	mapped.
		return ((size_t)ptr >= (size_t)m_start) && ((size_t)ptr < ((size_t)m_start + m_offset));
	}
}