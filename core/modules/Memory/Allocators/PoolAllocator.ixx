module;

#include <macros/AurionExport.h>

export module Aurion.Memory:PoolAllocator;

import :Allocator;

export namespace Aurion
{
	class AURION_API PoolAllocator : public IMemoryAllocator
	{
	public:
		PoolAllocator(const size_t& chunk_count, const size_t& chunk_size);
		virtual ~PoolAllocator() override;

		virtual void* Allocate(const size_t& size, const size_t& alignment = 16) override;

		void* Allocate();

		virtual void Free(void* ptr = nullptr) override;

		virtual void Reset() override; // Reset state

	private:
		void* m_start;
		void** m_free_list;
		size_t m_chunk_count;
		size_t m_chunk_size;
	};
}