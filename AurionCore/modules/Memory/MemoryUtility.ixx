module;

#include <AurionExport.h>
#include <cassert>
#include <cstdint>

export module Aurion.Memory:Utility;

import Aurion.Types;

export namespace Aurion
{
  // Shifts the address forward in memory to a byte-boundary relevant to the
  //  provided alignment. Alignment must be a power of 2.
  constexpr AURION_API uintptr_t AlignAddress(const uintptr_t& addr, const u16& align)
  {
    const u64 mask = align - 1;
    assert((align & mask) == 0 && "[AlignAddress] Invalid Address Alignment: Alignment must be a power of 2."); // Alignment must be a power of 2
    return (addr + mask) & ~mask;
  }

  // Shifts a pointer forward in memory to a byte-boundary relevant to the
  //  provided alignment. Alignment must be a power of 2.
  template<typename T>
  constexpr AURION_API T* AlignPointer(const T* ptr, const u16& align)
  {
    const uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    const uintptr_t aligned_addr = AlignAddress(addr, align);
    return reinterpret_cast<T*>(aligned_addr);
  }
}