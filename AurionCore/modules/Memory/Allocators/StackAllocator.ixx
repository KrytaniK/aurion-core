module;

#include <AurionExport.h>
#include <cstddef>

export module Aurion.Memory:StackAllocator;

import Aurion.Types;

import :Interface;

export namespace Aurion
{
	class AURION_API StackAllocator : public IMemoryAllocator
	{
	public:
		explicit StackAllocator(const u32& capacity, const u16& alignment); // Always default constructable
	  ~StackAllocator() override;

	  // No copies
	  StackAllocator(const StackAllocator&) = delete;
	  StackAllocator& operator=(const StackAllocator&) = delete;

	  // No moves
	  StackAllocator(StackAllocator&&) = delete;
	  StackAllocator& operator=(StackAllocator&&) = delete;

		[[nodiscard]] MemoryAllocation Allocate(const u32& size, const u16& alignment) override;

		void Free(MemoryAllocation alloc) override;

		void Reset() override; // Reset state

	  [[nodiscard]] MemoryAllocationMarker GetMarker() const;

	  void FreeToMarker(const MemoryAllocationMarker& marker);

	private:
	  MemoryBlock m_memory;
	  u32 m_capacity;
	  u32 m_offset;
	};
}