module Aurion.Events;

import Aurion.Types;
import <macros/AurionLog.h>;

namespace Aurion
{
	void EventBus::Register(const EventCategoryID& id, const EventRegisterCallback& callback, void* context)
	{
		if (m_registry.count == MAX_EVENT_REGISTERS)
		{
			AURION_ERROR("Failed to register event callback: Out of memory!");
			return;
		}

		// Ensure no duplicate callbacks per event category
		for (u16 i = 0; i < m_registry.count; i++)
		{
			if (m_registry.registers[i].category == id)
			{
				AURION_ERROR("Failed to register event callback: Callback for category [%d] already exists!", id);
				return;
			}
		}

		// Assign register callback, if available
		m_registry.registers[m_registry.count++] = EventBusRegister{ callback, context, id };
	}

	void EventBus::UnRegister(const EventCategoryID& id)
	{
		// Find the callback in the registry
		for (u16 i = 0; i < m_registry.count; i++)
		{
			const EventBusRegister& ev_register = m_registry.registers[i];

			if (ev_register.category == id)
			{
				// Replace the current register with the last register in the array.
				// This creates a copy
				m_registry.registers[i] = m_registry.registers[--m_registry.count];

				// Then null the original register
				m_registry.registers[m_registry.count] = EventBusRegister{ nullptr, nullptr, 0 };
				return;
			}
		}

		AURION_ERROR("Failed to unregister event callback: Event category [%d] not found!", id);
	}

	void EventBus::SwapRegister(const EventCategoryID& id, const EventRegisterCallback& callback, void* context)
	{
		// Find the register in the array
		for (u16 i = 0; i < m_registry.count; i++)
		{
			EventBusRegister& ev_register = m_registry.registers[i];

			if (ev_register.category == id)
			{
				// Swap the callback if found
				ev_register.callback = callback;
				return;
			}
		}

		AURION_ERROR("Failed to swap event callback: Event category [%d] not found!", id);
	}

	void EventBus::Dispatch(EventBase* event)
	{
		// Find the register in the array
		for (u16 i = 0; i < m_registry.count; i++)
		{
			EventBusRegister& ev_register = m_registry.registers[i];
			
			if (ev_register.category == event->GetCategory()) {
				ev_register.callback(event, ev_register.context);
			}
		}
	}

}