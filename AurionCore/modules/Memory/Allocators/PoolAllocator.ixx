module;

#include <AurionExport.h>
#include <cstddef>

export module Aurion.Memory:PoolAllocator;

import Aurion.Types;

import :Interface;

export namespace Aurion
{
	class AURION_API PoolAllocator : public IMemoryAllocator
	{
	  // Enforce a minimum chunk size of 4 bytes, allowing the allocator to track free
	  //  allocations by index, up to (2^32 - 1) = (4,294,967,295) unique chunks, when
	  //  the size of a chunk is less than the size of a void*.
	  constexpr size_t MINIMUM_CHUNK_SIZE = 4u;

	public:
		explicit PoolAllocator(const u32& chunk_count, const u32& chunk_size, const u16& alignment);
	  ~PoolAllocator() override;

	  // No copies
	  PoolAllocator(const PoolAllocator&) = delete;
	  PoolAllocator& operator=(const PoolAllocator&) = delete;

	  // No moves
	  PoolAllocator(PoolAllocator&&) = delete;
	  PoolAllocator& operator=(PoolAllocator&&) = delete;

	  // Retrieve a region of the allocated memory block. Parameters are ignored, as this allocator
	  //  is chunked based on initialization.
	  [[nodiscard]] MemoryAllocation Allocate();
	  [[nodiscard]] MemoryAllocation Allocate(const u32& size, const u16& alignment) override;

	  void Free(MemoryAllocation alloc) override;

	  void Reset() override;

	private:
	  MemoryBlock m_memory;
	  MemoryAllocation m_next_free;
		u32 m_capacity;
	  u32 m_chunk_size;
	};
}