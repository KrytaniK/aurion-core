module;

#include <macros/AurionExport.h>

export module Aurion.Memory:Allocator;

export namespace Aurion
{
	class AURION_API IMemoryAllocator
	{
	public:
		virtual ~IMemoryAllocator() = default;

		virtual void* Allocate(const size_t& size, const size_t& alignment = 0) = 0;

		virtual void Free(void* ptr) = 0;

		virtual void Reset() = 0;
	};
}