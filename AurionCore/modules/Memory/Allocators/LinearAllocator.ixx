module;

#include <AurionExport.h>

export module Aurion.Memory:LinearAllocator;

import Aurion.Types;

import :Interface;

export namespace Aurion
{
	class AURION_API LinearAllocator : public IMemoryAllocator
	{
	public:
		explicit LinearAllocator(const u32& capacity, const u16& alignment);
	  ~LinearAllocator() override;

	  // No copies
	  LinearAllocator(const LinearAllocator&) = delete;
	  LinearAllocator& operator=(const LinearAllocator&) = delete;

	  // No moves
	  LinearAllocator(LinearAllocator&&) = delete;
	  LinearAllocator& operator=(LinearAllocator&&) = delete;

    [[nodiscard]] MemoryAllocation Allocate(const u32& size, const u16& alignment) override;

    void Free(MemoryAllocation alloc) override;

    void Reset() override;

  private:
	  MemoryBlock m_memory;
	  u32 m_capacity;
	  u32 m_offset;
	};
}