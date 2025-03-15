#include <cstdint>
#include <stdlib.h>

import Aurion.Input;

namespace Aurion
{
	InputDevice::InputDevice()
		: m_info({}), m_state(nullptr), m_control_count(0), m_controls(nullptr)
	{

	}

	InputDevice::InputDevice(const InputDeviceInfo& info, const InputLayout& layout)
		: m_info(info), m_state(nullptr), m_control_count(layout.control_count), m_controls(nullptr)
	{
		// Determine size of state in memory
		size_t state_size_bits = 0;
		size_t state_size_bytes = 0;
		for (size_t i = 0; i < m_control_count; i++)
		{
			state_size_bits += layout.control_infos[i].size_in_bits;
			state_size_bytes += layout.control_infos[i].size_in_bytes;
		}

		// Reserve enough space for all bits
		state_size_bytes += (state_size_bits / 8) + 1;

		// Allocate state
		m_state = (uint8_t*)calloc(state_size_bytes, sizeof(uint8_t));

		// Allocate enough space for all device controls
		m_controls = (InputControl*)calloc(m_control_count, sizeof(InputControl));

		// Create all controls
		for (size_t i = 0; i < m_control_count; i++)
			m_controls[i] = InputControl(layout.control_infos[i], m_state);
	}

	InputDevice::~InputDevice()
	{
		if (m_state)
			free(m_state);

		if (m_controls)
			free(m_controls);
	}

	const InputDeviceInfo& InputDevice::GetInfo()
	{
		return m_info;
	}

	InputControl* InputDevice::GetControl(const uint16_t& input_code)
	{
		for (size_t i = 0; i < m_control_count; i++)
			if (m_controls[i].IsValid() && m_controls[i].GetInfo().input_code == input_code)
				return &m_controls[i];

		return nullptr;
	}

}