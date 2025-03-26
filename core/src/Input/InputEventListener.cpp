#include <macros/AurionLog.h>

#include <cstdint>

import Aurion.Input;

namespace Aurion
{
	InputEventListener::InputEventListener()
		: m_event_type(0), m_base_callback(nullptr), m_event_callback(nullptr)
	{

	}

	InputEventListener::InputEventListener(const uint64_t& event_type)
		: m_event_type(event_type), m_base_callback(nullptr), m_event_callback(nullptr)
	{

	}

	InputEventListener::~InputEventListener()
	{

	}

	uint64_t InputEventListener::GetEventType()
	{
		return m_event_type;
	}

	void InputEventListener::OnEvent(IEvent* event)
	{
		if (!m_event_callback && !m_base_callback)
		{
			AURION_WARN("[InputEventListener] Listener for event type {%d} has no bound function!", m_event_type);
			return;
		}

		if (m_event_callback)
			m_event_callback((InputEvent*)event);

		if (m_base_callback)
			m_base_callback();
	}

	bool InputEventListener::Bind(const InputVoidCallback& callback)
	{
		if (!callback)
			return false;

		m_base_callback = callback;

		return true;
	}

	bool InputEventListener::Bind(const InputEventCallback& callback)
	{
		if (!callback)
			return false;

		m_event_callback = callback;

		return true;
	}


}