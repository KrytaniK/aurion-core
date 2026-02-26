module;

#include <cassert>

module Aurion.Events;

import Aurion.Types;

namespace Aurion
{
	EventBus::EventBus(const u8& category_count)
		: m_max_count(category_count), m_registry_count(0)
	{
		m_registry = new EventCategoryRegistry[category_count];
	}

	EventBus::~EventBus()
	{
		delete[] m_registry;
	}

	void EventBus::Register(const EventHandler& handler)
	{
		// Placement New Example:
		//	new (&window) GLFW_Window(properties);

		// If no handlers exist for any category, add this one.
		if (m_registry_count == 0)
		{
			EventCategoryRegistry& reg = m_registry[m_registry_count++];
			reg.category = handler.category;
			reg.handlers[reg.count++] = handler;
			return;
		}

		// Otherwise, we need to search for the correct handler based on category
		EventCategoryRegistry* temp = nullptr;
		for (u8 i = 0; i < m_registry_count; i++)
		{
			temp = &m_registry[i];
			if (temp->category == handler.category)
			{
				// No handlers beyond the max
				assert(temp->count < MAX_EVENT_HANDLERS);

				temp->handlers[temp->count++] = handler;
				return;
			}
		}

		assert(m_registry_count < m_max_count);

		// If we've made it this far, the category has no handlers
		EventCategoryRegistry& reg = m_registry[m_registry_count++];
		reg.category = handler.category;
		reg.handlers[reg.count++] = handler;
	}

	void EventBus::UnRegister(const EventCategoryID& category, EventCallback callback)
	{
		// Search for callback
		EventCategoryRegistry* temp = nullptr;
		for (u8 i = 0; i < m_registry_count; i++)
		{
			// Don't check if categories don't match
			if (m_registry[i].category != category)
				continue;

			u8 found_index = u8(-1);
			temp = &m_registry[i];

			// If the callback is found, remove it
			for (u8 j = 0; j < temp->count; j++)
			{
				if (temp->handlers[j].callback != callback)
					continue;

				found_index = j;
				break;
			}

			if (found_index == u8(-1))
				return;

			// Shrink the array
			for (u8 j = found_index; j < temp->count - 1; j++)
				temp->handlers[j] = temp->handlers[j + 1];

			temp->count--;
			return;
		}
	}

	void EventBus::Dispatch(EventBase* event)
	{
		EventCategoryID category = event->GetCategory();

		// Find the handler registry for the target context
		EventCategoryRegistry* temp = nullptr;
		for (u8 i = 0; i < m_registry_count; i++)
		{
			// Category must match
			if (m_registry[i].category != category)
				continue;

			temp = &m_registry[i];
			break;
		}

		// Don't dispatch if no handlers exist
		if (!temp) return;

		// Execute all handlers (if any)
		EventHandler* handler = nullptr;
		for (u8 i = 0; i < temp->count; i++)
		{
			handler = &temp->handlers[i];
			handler->callback(event, handler->context);
		}
	}

}