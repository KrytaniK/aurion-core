module;

#include <AurionLog.h>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <cstddef>
#include <algorithm>
#include <cstring>

module Aurion.Memory;

import Aurion.Types;

namespace Aurion
{
	PoolAllocator::PoolAllocator(const u32& chunk_count, const u32& chunk_size, const u16& alignment)
		: m_memory(nullptr), m_next_free(nullptr)
	{
	  // Enforce a minimum chunk size, based on the provided chunk size.
	  //  In the worst case, each chunk will contain 3 bytes of empty space (chunk size == 1).
	  //  This is a non-issue for the majority of use cases, but allows for efficient tracking
	  //  of freed allocations.
	  size_t min_chunk_size = std::min(MINIMUM_CHUNK_SIZE, sizeof(void*));
	  m_chunk_size = std::max(static_cast<size_t>(chunk_size), min_chunk_size);

	  if (m_chunk_size > chunk_size)
	    AURION_WARN("[Pool Allocator] Provided chunk size (%d) is less than the minimum (%d). %d bytes of padding will be applied to each chunk.", chunk_size, min_chunk_size, min_chunk_size - chunk_size);

	  // Ensure the capacity is always a multiple of the chunk size.
	  m_capacity = chunk_count * m_chunk_size;

	  // Allocate the initial memory block
	  MemoryBlock raw_alloc = static_cast<MemoryBlock>(calloc(m_capacity, sizeof(u8)));

	  // Align the memory address to the desired alignment
	  uintptr_t alloc_addr = reinterpret_cast<uintptr_t>(raw_alloc);
	  uintptr_t aligned_alloc = AlignAddress(alloc_addr, alignment);
	  m_memory = reinterpret_cast<MemoryBlock>(aligned_alloc);

	  // If the aligned allocation is in the same location,
	  //  shift to the next alignment boundary.
	  if (m_memory == raw_alloc)
	    m_memory += alignment;

	  // Then, determine how much the memory shifted
	  ptrdiff_t shift = m_memory - raw_alloc;
	  assert((shift > 0 && shift < 256) && "[Pool Allocator] Invalid memory alignment.");

	  // And store the shift amount between the raw allocation and the aligned allocation
	  m_memory[-1] = static_cast<u8>(shift & 0xFF);
	  m_next_free = m_memory;

	  // At each chunk address, store the offset of the next free allocation block
	  for (u32 i = 0; i < chunk_count - 1; i++)
	    *reinterpret_cast<u32*>(&m_memory[i * m_chunk_size]) = (i + 1) * m_chunk_size;

	  // The last chunk should point to an invalid index
	  *reinterpret_cast<u32*>(&m_memory[m_capacity - m_chunk_size]) = UINT32_MAX;
	}

	PoolAllocator::~PoolAllocator()
	{
	  // We need to figure out the shift amount from the allocation 'header'.
	  // This shift amount is always at location (p - 1).
	  const u8 shift = m_memory[-1];
	  const u8 shift_amt = shift == 0 ? 256 : shift == 0;

	  u8* raw_alloc = m_memory - shift_amt;
	  free(raw_alloc);

	  m_memory = nullptr;
	}

  MemoryAllocation PoolAllocator::Allocate()
  {
	  return Allocate(m_chunk_size, 0); // size/alignment are ignored.
  }

	MemoryAllocation PoolAllocator::Allocate(const u32& size, const u16& alignment)
	{
	  // NOTE: Size and alignment are ignored.

	  if (m_next_free == nullptr)
	  {
	    AURION_ERROR("[Pool Allocator] Failed to allocate memory: Out of memory.");
	    return nullptr;
	  }

	  // Pull the address of the next free block of memory
	  MemoryAllocation allocation = m_next_free;

	  // The offset of the next available free memory block is stored
	  //  inside the 'empty' allocation. Read and forward the next-free
	  //  list to this location. This offset value is guaranteed to be 4 bytes.
	  u32 next_offset = *static_cast<u32*>(allocation);
	  m_next_free = (next_offset == UINT32_MAX) ? nullptr : m_memory + next_offset;

	  return allocation;
	}

	void PoolAllocator::Free(MemoryAllocation alloc)
	{
	  // Bounds check to ensure this allocation came from this allocator
	  ptrdiff_t diff = static_cast<MemoryBlock>(alloc) - m_memory;
	  if (diff < 0 || (diff + m_chunk_size) > m_capacity)
	  {
	    AURION_ERROR("[Pool Allocator] Failed to free allocation: Out of bounds.");
	    return;
	  }

	  // For safety, null the entire allocation
	  std::memset(alloc, 0, m_chunk_size);

	  // Calculate the offset of the next free chunk, and
	  //  write this offset into the provided allocation. When
	  //  the allocator is full, write an invalid index.
	  *static_cast<u32*>(alloc) = (m_next_free == nullptr) ? UINT32_MAX : *static_cast<u32*>(m_next_free);

	  // Then, pre-pend this allocation to the front of the next-free list
	  m_next_free = alloc;
	}

	void PoolAllocator::Reset()
	{
	  // Determine chunk count
	  const u32 chunk_count = m_capacity % m_chunk_size;

	  // null entire allocation
	  std::memset(m_memory, 0, m_capacity);

	  // At each chunk address, store the offset of the next free allocation block
	  for (u32 i = 0; i < chunk_count - 1; i++)
	    *reinterpret_cast<u32*>(&m_memory[i * m_chunk_size]) = (i + 1) * m_chunk_size;

	  // The last chunk should point to an invalid index
	  *reinterpret_cast<u32*>(&m_memory[m_capacity - m_chunk_size]) = UINT32_MAX;
	}
}