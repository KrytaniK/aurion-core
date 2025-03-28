#include <macros/AurionLog.h>

import Aurion.GLFW;

namespace Aurion
{
	GLFWInputControl::GLFWInputControl()
		: m_info({}), m_layout({}), m_state_block(nullptr)
	{

	}

	GLFWInputControl::GLFWInputControl(uint8_t* device_state_block, const InputControlInfo& info, const InputControlLayout& layout)
		: m_info({}), m_layout({}), m_state_block(nullptr)
	{
		this->Initialize(device_state_block, info, layout);
	}

	GLFWInputControl::~GLFWInputControl()
	{

	}

	const InputControlInfo& GLFWInputControl::GetInfo()
	{
		return m_info;
	}

	const InputControlLayout& GLFWInputControl::GetLayout()
	{
		return m_layout;
	}

	const uint64_t& GLFWInputControl::GetInputCode()
	{
		return m_info.input_code;
	}

	const uint64_t& GLFWInputControl::GetControlType()
	{
		return m_info.type;
	}

	void GLFWInputControl::Initialize(uint8_t* device_state_block, const InputControlInfo& info, const InputControlLayout& layout)
	{
		m_info = info;
		m_layout = layout;

		m_state_block = device_state_block + m_layout.byte_offset;
	}


}