#include <cstdlib>
#include <cstdint>

import Aurion.Memory;

namespace Aurion
{
	StackAllocator::StackAllocator(const size_t& size)
		: m_start(nullptr), m_offset(0), m_max_offset(size)
	{
		m_start = malloc(size);
	}

	StackAllocator::~StackAllocator()
	{
		free(m_start);
		m_start = nullptr;
	}

	void* StackAllocator::Allocate(const size_t& size, const size_t& alignment)
	{
		if (!m_start)
			return nullptr;

		// Get the current address
		const size_t current_address = (size_t)(m_start) + m_offset;

		// If alignment differs from what the current alignment is, padding
		//	is required.
		const size_t padding = (alignment - (current_address % alignment)) % alignment;

		// Enforce explicit upper bound (with header)
		if (m_offset + sizeof(AllocationHeader) + padding + size > m_max_offset)
			return nullptr;

		// Calculate aligned address (with header)
		size_t aligned_address = current_address + padding + sizeof(AllocationHeader);

		// Store padding information in the allocation header
		AllocationHeader* header = (AllocationHeader*)(aligned_address - sizeof(AllocationHeader));
		header->padding = (uint8_t)padding;

		// Move stack pointer forward
		m_offset += padding + sizeof(AllocationHeader) + size;

		// return aligned address
		return (void*)(aligned_address);
	}

	void StackAllocator::Free(void* ptr)
	{
		// ptr parameter is ignored (LIFO)

		// If the stack pointer hasn't moved, there'snothing to free
		if (m_offset == 0)
			return;

		// Get pointer to the end of the last allocated block
		size_t current_address = (size_t)(m_start) + m_offset;

		// Pull header information of the previous block
		AllocationHeader* header = (AllocationHeader*)(current_address - sizeof(AllocationHeader));

		// Move the stack pointer back, accounting for padding + header size
		m_offset -= (header->padding + sizeof(AllocationHeader));
	}

	void StackAllocator::Reset()
	{
		m_offset = 0;
	}
}