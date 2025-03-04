module;

#include <macros/AurionExport.h>
#include <cstdint>

export module Aurion.Memory:Allocator;

export namespace Aurion
{
	struct AURION_API AllocationHeader
	{
		uint8_t padding = 0;
	};

	class AURION_API IMemoryAllocator
	{
	public:
		virtual ~IMemoryAllocator() = default;

		virtual void* Allocate(const size_t& size, const size_t& alignment = 16) = 0;

		virtual void Free(void* ptr = nullptr) = 0;

		virtual void Reset() = 0;
	};
}