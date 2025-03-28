#include <macros/AurionLog.h>

import Aurion.GLFW;

namespace Aurion
{
	GLFWButtonControl::GLFWButtonControl()
		: GLFWInputControl()
	{

	}

	GLFWButtonControl::GLFWButtonControl(uint8_t* device_state_block, const InputControlInfo& info, const InputControlLayout& layout)
		: GLFWInputControl(device_state_block, info, layout)
	{

	}

	GLFWButtonControl::~GLFWButtonControl()
	{

	}

	bool GLFWButtonControl::Update(void* value, const uint16_t& value_size)
	{
		if (value == nullptr)
		{
			AURION_ERROR("[GLFWButtonControl] Error: Failed to set control value. Reason: Invalid value pointer");
			return false;
		}

		// Value size is ignored here

		uint8_t* byte = m_state_block;

		bool new_value = *(bool*)value;

		// Update the bit, if needed
		if (m_layout.size_in_bits == 1)
		{
			// Offset the state pointer to write to the correct byte
			byte = m_state_block + (m_layout.bit_offset / 8);

			// Generate a mask to isolate the desired bit
			uint8_t mask = (1U << (m_layout.bit_offset % 8));

			// If there's any mismatch between the desired and actual values, toggle the bit
			if ((new_value && (*byte & mask) == 0) || (!new_value && (*byte & mask) != 0))
				*byte ^= mask;
		}
		else
		{
			*byte = new_value;
		}

		return true;
	}

	void GLFWButtonControl::ReadValue(bool* out_value)
	{
		if (out_value == nullptr)
		{
			AURION_ERROR("[GLFWButtonControl] Error: Failed to read control value. Reason: Invalid output variable");
			return;
		}

		// Offset state block pointer
		uint8_t* byte = m_state_block + m_layout.byte_offset;

		// If this control manages a single bit value, read the bit.
		if (m_layout.size_in_bits == 1)
		{
			// Offset the state pointer to write to the correct byte
			byte = byte + (m_layout.bit_offset / 8);

			// Generate a mask to isolate the desired bit
			uint8_t mask = (1U << (m_layout.bit_offset % 8));

			// Apply the mask and convert to boolean
			*out_value = (*byte & mask) != 0;
			return;
		}

		// Otherwise, simply read the data at the control state block as a boolean
		*out_value = *(bool*)byte;
	}
}