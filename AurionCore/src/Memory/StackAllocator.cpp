module Aurion.Memory;

#include <cstdlib>
#include <cstdint>

namespace Aurion
{
	StackAllocator::StackAllocator()
		: m_start(nullptr), m_offset(0), m_max_offset(0)
	{
		
	}

	StackAllocator::~StackAllocator()
	{
		free(m_start);
		m_start = nullptr;
	}

	StackAllocator::StackAllocator(StackAllocator&& other)
	{
		m_start = other.m_start;
		m_offset = other.m_offset;
		m_max_offset = other.m_max_offset;
	}

	StackAllocator& StackAllocator::operator=(StackAllocator&& other)
	{
		m_start = other.m_start;
		m_offset = other.m_offset;
		m_max_offset = other.m_max_offset;

		return *this;
	}

	void StackAllocator::Initialize(const size_t& chunk_size, const size_t& chunk_count)
	{
		if (chunk_size == 0 || chunk_count == 0)
			return;

		// Set the max offset
		m_max_offset = chunk_size * chunk_count;

		// Use calloc to grab memory and initialize it to 0
		m_start = calloc(chunk_count, chunk_size);
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
		if (!m_start || m_offset == 0)
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
		if (!m_start)
			return;

		m_offset = 0;
	}

	bool StackAllocator::IsMapped(void* ptr)
	{
		// Ensure the allocator has been initialized
		if (!m_start)
			return false;

		// If the pointer is within the bounds of what's been allocated, then it has been
		//	mapped.
		return ((size_t)ptr >= (size_t)m_start) && ((size_t)ptr < ((size_t)m_start + m_offset));
	}
}