#include <macros/AurionLog.h>

#include <cstdint>
#include <memory>
#include <cassert>

import Aurion.Input;

namespace Aurion
{
	InputState::InputState(const uint8_t& size)
		: m_total_size(size)
	{
		assert(size > 0 && "InputState: Total size must be greater than 0");

		// Allocate the state memory block and zero it out
		m_start_ptr = static_cast<uint8_t*>(calloc(size, 1));
	}

	InputState::~InputState()
	{
		free(m_start_ptr);
		m_start_ptr = nullptr;
	}

	InputStateBlock InputState::GetStateBlock(const uint8_t& offset, const uint8_t& size)
	{
		assert(size > 0 && "InputState: Requested state block size must be greater than 0");
		assert(offset + size <= m_total_size && "InputState: Requested state block exceeds total state size");

		return InputStateBlock{offset, size};
	}

	void InputState::Write(const void* data, const uint8_t& offset, const uint8_t& size)
	{
		assert(m_start_ptr && "InputState: State memory block is not allocated");
		assert(offset + size <= m_total_size && "InputState: Write operation exceeds total state size");
		assert(size > 0 && "InputState: Write operation size must be greater than 0");
		assert(data && "InputState: Data pointer is null");

		// Perform the memory copy to update the state
		memcpy(m_start_ptr + offset, data, size);
	}

	bool InputState::IsValidBlock(const InputStateBlock& block) const
	{
		return !(block.offset + block.total_size > m_total_size || block.total_size == 0);
	}

}