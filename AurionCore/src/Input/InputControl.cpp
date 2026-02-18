module Aurion.Input;

namespace Aurion
{
	InputControl::InputControl(InputDevice* device, const InputStateBlock& block)
		: m_device(device), m_state(block)
	{

	}

	void InputControl::Read(void* out_data, const u8& size)
	{
		m_device->Read(m_state, out_data, size);
	}

	void InputControl::Write(void* data, const u8& size)
	{
		m_device->Write(data, m_state.offset, size);
	}

}