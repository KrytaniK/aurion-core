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

	const InputControlState& InputControl::GetState()
	{
		return m_info.input_state;
	}

	const uint8_t& InputControl::GetInputCode()
	{
		return m_info.input_code;
	}

	bool InputControl::SetFromArray(const int*& values, const size_t& count)
	{
		if (!this->IsValid() || values == nullptr || count == 0)
			return false;

		size_t max_writes = m_info.size_in_bytes / sizeof(int);

		// If this control isn't sized for floats return.
		if (max_writes == 0)
			return false;

		// Never attempt to write more data than is available
		if (count < max_writes)
			max_writes = count;

		// Write each float value into this control. This may overwrite all control data
		for (size_t i = 0; i < max_writes; i++)
			*((int*)m_state_block + i) = values[i];

		return true;
	}

	bool InputControl::SetFromArray(const float*& values, const size_t& count)
	{
		if (!this->IsValid() || values == nullptr || count == 0)
			return false;

		size_t max_writes = m_info.size_in_bytes / sizeof(float);

		// If this control isn't sized for floats return.
		if (max_writes == 0)
			return false;

		// Never attempt to write more data than is available
		if (count < max_writes)
			max_writes = count;

		// Write each float value into this control. This may overwrite all control data
		for (size_t i = 0; i < max_writes; i++)
			*((float*)m_state_block + i) = values[i];

		return true;
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
			{
				// Assign value in memory
				*m_state_block |= (1 << m_info.bit); // Set bit to 1

				// Update control state
				if (m_info.input_state > INPUT_STATE_IDLE)
					m_info.input_state = INPUT_STATE_HELD;
				else
					m_info.input_state = INPUT_STATE_ACTIVE;
			}
			else
			{
				// Assign value in memory
				*m_state_block &= ~(1 << m_info.bit); // Clear the bit

				// Clear control state
				m_info.input_state = INPUT_STATE_IDLE;
			}

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