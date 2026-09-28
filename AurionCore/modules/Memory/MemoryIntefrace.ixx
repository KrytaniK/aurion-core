module;

#include <AurionExport.h>
#include <cstddef>

export module Aurion.Memory:Interface;

import Aurion.Types;

export namespace Aurion
{
  typedef u8* MemoryBlock;
  typedef void* MemoryAllocation;
  typedef u32 MemoryAllocationMarker;

  struct AURION_API IMemoryAllocator
  {
    virtual ~IMemoryAllocator() = default;

    // Allocates a block of memory, byte-aligned such that the provided alignment is honored.
    [[nodiscard]] virtual MemoryAllocation Allocate(const u32& size, const u16& alignment) = 0;

    // Resets the state of a memory block within the larger memory allocation
    virtual void Free(MemoryAllocation alloc) = 0;

    // Resets the entire memory allocation to its default state (null)
    virtual void Reset() = 0;
  };
}