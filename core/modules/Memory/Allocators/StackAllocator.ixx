module;

#include <macros/AurionExport.h>

export module Aurion.Memory:StackAllocator;

import :Allocator;

export namespace Aurion
{
	class AURION_API StackAllocator : public IMemoryAllocator
	{
	public:
		StackAllocator(const size_t& size);
		virtual ~StackAllocator() override;

		virtual void* Allocate(const size_t& size, const size_t& alignment = 16) override;

		virtual void Free(void* ptr = nullptr) override; // LIFO deallocation

		virtual void Reset() override; // Reset state

	private:
		void* m_start;
		size_t m_offset;
		size_t m_max_offset;
	};
}