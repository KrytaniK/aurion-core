module;

#include <macros/AurionExport.h>

#include <cstdint>

export module Aurion.Events:EventBus;

import :Event;

export namespace Aurion
{
	typedef uint16_t EventCategoryID;
	typedef void(*EventRegisterCallback)(EventBase* event, void* context);

	constexpr uint8_t MAX_EVENT_REGISTERS = 16;

	struct EventBusRegister
	{
		EventRegisterCallback callback = nullptr;
		void* context = nullptr;
		EventCategoryID category = 0;
	};

	struct EventBusRegistry
	{
		EventBusRegister registers[MAX_EVENT_REGISTERS];
		uint16_t count = 0;
	};

	class AURION_API EventBus
	{
	public:
		EventBus() = default;
		~EventBus() = default;

		void Register(const EventCategoryID& id, const EventRegisterCallback& callback, void* context = nullptr);
		void UnRegister(const EventCategoryID& id);

		void SwapRegister(const EventCategoryID& id, const EventRegisterCallback& callback, void* context);

		void Dispatch(EventBase* event);
		 
	private:
		EventBusRegistry m_registry;
	};
}