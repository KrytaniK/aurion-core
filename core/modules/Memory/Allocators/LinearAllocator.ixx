module;

#include <macros/AurionExport.h>

export module Aurion.Memory:LinearAllocator;

import :Allocator;

export namespace Aurion
{
	class AURION_API LinearAllocator : public IMemoryAllocator
	{
	public:
		LinearAllocator(const size_t& size);
		virtual ~LinearAllocator() override;

		// Inherited via IMemoryAllocator
		virtual void* Allocate(const size_t& size, const size_t& alignment = 16) override;

		virtual void Free(void* ptr) override;

		virtual void Reset() override;

	private:
		void* m_start;
		size_t m_offset;
		size_t m_max_offset;
	};
}