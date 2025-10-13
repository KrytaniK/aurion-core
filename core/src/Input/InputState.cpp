module Aurion.Input;

import Aurion.Types;
import <macros/AurionLog.h>;
import <memory>;
import <cassert>;

namespace Aurion
{
	InputState::InputState(const u8& size)
		: m_total_size(size)
	{
		assert(size > 0 && "InputState: Total size must be greater than 0");

		// Allocate the state memory block and zero it out
		m_start_ptr = static_cast<u8*>(calloc(size, 1));
	}

	InputState::~InputState()
	{
		free(m_start_ptr);
		m_start_ptr = nullptr;
	}

	InputStateBlock InputState::GetStateBlock(const u8& offset, const u8& size)
	{
		assert(size > 0 && "InputState: Requested state block size must be greater than 0");
		assert(offset + size <= m_total_size && "InputState: Requested state block exceeds total state size");

		return InputStateBlock{offset, size};
	}

	void InputState::Write(const void* data, const u8& offset, const u8& size)
	{
		assert(m_start_ptr && "InputState: State memory block is not allocated");
		assert(offset + size <= m_total_size && "InputState: Write operation exceeds total state size");
		assert(size > 0 && "InputState: Write operation size must be greater than 0");
		assert(data && "InputState: Data pointer is null");

		// Copy data from in-buffer to input state
		memcpy(m_start_ptr + offset, data, size);
	}

	void InputState::Read(const InputStateBlock& block, void* out_data, const u8& size) const
	{
		if (!this->IsValidBlock(block))
		{
			AURION_ERROR("[InputState] Failed to read from memory: Provided State Block Is Invalid!");
			return;
		}

		// Copy data from input state to out-buffer.
		memcpy(out_data, m_start_ptr + block.offset, size);
	}

	bool InputState::IsValidBlock(const InputStateBlock& block) const
	{
		return !(block.offset + block.total_size > m_total_size || block.total_size == 0);
	}

}