module;

#include <cstdlib>
#include <cassert>
#include <cstddef>
#include <cstdint>

module Aurion.Memory;

import Aurion.Types;

namespace Aurion
{
  StackAllocator::StackAllocator()
    : m_memory(nullptr), m_capacity(0), m_offset(0)
  {
  }

  StackAllocator::StackAllocator(const u32& capacity, const u16& alignment)
    : m_memory(nullptr), m_capacity(capacity + alignment), m_offset(0)
  {
    // Allocate the initial memory block
    MemoryBlock raw_alloc = static_cast<MemoryBlock>(calloc(m_capacity, sizeof(u8)));

    // Align the memory address to the desired alignment
    uintptr_t alloc_addr = reinterpret_cast<uintptr_t>(raw_alloc);
    uintptr_t aligned_alloc = AlignAddress(alloc_addr, alignment);
    m_memory = reinterpret_cast<MemoryBlock>(aligned_alloc);

    // If the aligned allocation is in the same location,
    //  shift by the full alignment amount to store the shift
    //  offset.
    if (m_memory == raw_alloc)
      m_memory += alignment;

    // Then, determine how much the memory shifted
    ptrdiff_t shift = m_memory - raw_alloc;
    assert((shift > 0 && shift < 256) && "[Stack Allocator] Invalid memory alignment.");

    // And store the shift amount between the raw allocation and the aligned allocation
    m_memory[-1] = static_cast<u8>(shift & 0xFF);
  }

  StackAllocator::~StackAllocator()
  {
    /// We need to figure out the shift amount from the allocation 'header'.
    // This shift amount is always at location (p - 1) and is guaranteed to be between 1-255.
    const u8 shift = m_memory[-1];
    u8* raw_alloc = m_memory - shift;
    free(raw_alloc);

    m_memory = nullptr;
  }

  MemoryAllocation StackAllocator::Allocate(const u32& size, const u16& alignment)
  {
    // Allocate extra space to ensure memory can be aligned. In the worst case,
    //  memory will require shifting (alignment - 1) bytes.
    const u32 alloc_size = size + alignment - 1;

    assert((m_memory != nullptr) && "[Stack Allocator] Invalid memory block.");
    assert(((m_offset + alloc_size) < m_capacity) && "[Stack Allocator] Not enough memory.");

    const u8* raw = m_memory + m_offset;
    m_offset += alloc_size;

    // Note: The shift amount doesn't need to be stored here, because freeing individual allocations
    //    is forbidden for this allocator.
    return AlignPointer<u8>(raw, alignment);
  }

  void StackAllocator::Free(MemoryAllocation alloc)
  {
    // No-op for stack allocators. Only free from Marker objects
  }

  void StackAllocator::Reset()
  {
    m_offset = 0;
  }

  MemoryAllocationMarker StackAllocator::GetMarker() const
  {
    return m_offset;
  }

  void StackAllocator::FreeToMarker(const MemoryAllocationMarker& marker)
  {
    m_offset = marker;
  }
}
