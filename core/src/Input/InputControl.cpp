#include <cstdint>

import Aurion.Input;

namespace Aurion
{
	InputControl::InputControl()
		: m_info({}), m_state_block(nullptr)
	{

	}

	InputControl::InputControl(const InputControlInfo& info, uint8_t* parent_state)
		: m_info(info), m_state_block(nullptr)
	{
		// If this is a container control, don't map this control to the parent state
		if (m_info.input_code== 0)
			return;

		// Offset the state block by the intended byte offset
		m_state_block = parent_state + m_info.byte_offset;
	}

	InputControl::~InputControl()
	{
		m_state_block = nullptr;
	}

	const bool InputControl::IsValid() const
	{
		return m_state_block != nullptr;
	}

	const InputControlInfo& InputControl::GetInfo()
	{
		return m_info;
	}

	InputControl::operator bool() const
	{
		return this->IsValid();
	}

	InputControl::operator int() const
	{
		if (!this->IsValid())
			return 0;

		// First, check if this control operates on a single bit
		if (m_info.size_in_bits == 1)
			return (*((int*)m_state_block) & (1 << m_info.bit)) != 0;

		return *((int*)m_state_block);
	}

	InputControl::operator float() const
	{
		if (!this->IsValid())
			return 0.f;

		return *((float*)m_state_block);
	}

	InputControl::operator float* () const
	{
		if (!this->IsValid())
			return nullptr;

		return (float*)m_state_block;
	}

	InputControl& InputControl::operator=(bool value)
	{
		if (!this->IsValid())
			return *this;

		if (m_info.size_in_bits != 0)
		{
			if (value)
				*m_state_block |= (1 << m_info.bit); // Set bit to 1
			else
				*m_state_block &= ~(1 << m_info.bit); // Clear the bit

			return *this;
		}

		// If this control isn't a bit control, assign 1 or 0 to the current block
		*m_state_block = value;

		return *this;
	}

	InputControl& InputControl::operator=(int value)
	{
		if (!this->IsValid())
			return *this;
		
		// Write the integer to the memory block
		*((int*)m_state_block) = value;

		return *this;
	}

	InputControl& InputControl::operator=(float value)
	{
		if (!this->IsValid())
			return *this;

		// Write the float to the memory block
		*((float*)m_state_block) = value;

		return *this;
	}

	InputControl& InputControl::operator=(float* values)
	{
		if (!this->IsValid() || values == nullptr || m_info.size_in_bytes < sizeof(float))
			return *this;

		// Write each float value into this control, not exceeding the control's size.
		for (size_t i = 0; i < (m_info.size_in_bytes / sizeof(float)); i++)
			*((float*)m_state_block + i) = values[i];
	}

	const InputControl& InputControl::operator[](size_t index) const
	{
		if (!this->IsValid())
			return *this;

		// Safe-guard against invalid indices
		if (m_info.child_count == 0 || index >= m_info.child_count)
			return *this;

		// This only works because InputControls are always stored in a contiguous block.
		//	Child controls always occur directly after the parent control
		return this[index];
	}
}