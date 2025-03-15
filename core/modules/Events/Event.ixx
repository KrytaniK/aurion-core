module;

#include <macros/AurionExport.h>

#include <cstdint>

export module Aurion.Events:Event;

export namespace Aurion
{
	struct AURION_API IEvent
	{
		uint64_t type = UINT64_MAX;
		bool propagates = true;
	};

	class AURION_API Event
	{
	public:
		virtual ~Event() = default;

		virtual uint64_t GetType() = 0;

		virtual bool ShouldPropagate() = 0;

		virtual void StopPropagation() = 0;
	};
}