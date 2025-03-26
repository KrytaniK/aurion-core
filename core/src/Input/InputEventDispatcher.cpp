#include <cstdint>

import Aurion.Input;

namespace Aurion
{
	InputEventDispatcher::InputEventDispatcher()
		: m_event_type(0), m_listener_count(0), m_capacity(0), m_listeners(nullptr)
	{

	}

	InputEventDispatcher::InputEventDispatcher(const uint64_t& event_type)
		: m_event_type(event_type), m_listener_count(0), m_capacity(10), m_listeners(nullptr)
	{
		// Allocate enough memory for 10 event listeners initially
		m_listeners = new InputEventListener*[m_capacity];
	}

	InputEventDispatcher::~InputEventDispatcher()
	{
		if (m_listeners)
			delete[] m_listeners;

		m_listeners = nullptr;
	}

	uint64_t InputEventDispatcher::GetEventType()
	{
		return m_event_type;
	}

	bool InputEventDispatcher::AddEventListener(IEventListener* listener)
	{
		// If we've reached capacity, resize
		if (m_listener_count == m_capacity)
		{
			// Allocate a new array
			m_capacity += (m_capacity / 2);
			InputEventListener** new_listeners = new InputEventListener*[m_capacity];

			// Copy each pointer into the new array
			for (size_t i = 0; i < m_listener_count; i++)
				new_listeners[i] = m_listeners[i];

			// delete the old array
			delete[] m_listeners;

			// Assign new array
			m_listeners = new_listeners;
		}

		// Iterate through the array and assign to the first invalid slot
		for (size_t i = 0; i < m_capacity; i++)
		{
			if (m_listeners[i] == nullptr)
			{
				m_listeners[i] = (InputEventListener*)listener;
				m_listener_count++;
				return true;
			}
		}

		return false;
	}

	bool InputEventDispatcher::RemoveEventListener(IEventListener* listener)
	{
		// Iterate through the array and search for a matching pointer
		for (size_t i = 0; i < m_capacity; i++)
		{
			if (m_listeners[i] && m_listeners[i] == (InputEventListener*)listener)
			{
				m_listener_count--;
				m_listeners[i] = nullptr;
				return true;
			}
		}

		return false;
	}

	void InputEventDispatcher::Dispatch(IEvent* event)
	{
		for (size_t i = 0; i < m_capacity; i++)
			if (m_listeners[i] != nullptr)
				m_listeners[i]->OnEvent(event);
	}

}