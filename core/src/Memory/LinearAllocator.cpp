#include <cstdlib>

import Aurion.Memory;

namespace Aurion
{
	LinearAllocator::LinearAllocator(const size_t& size)
		: m_start(nullptr), m_offset(0), m_max_offset(size)
	{
		m_start = malloc(size);
	}

	LinearAllocator::~LinearAllocator()
	{
		free(m_start);
		m_start = nullptr;
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
	}

	void LinearAllocator::Reset()
	{
		m_offset = 0;
	}
}