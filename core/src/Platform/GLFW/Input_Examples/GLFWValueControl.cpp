#include <macros/AurionLog.h>

import Aurion.GLFW;

namespace Aurion
{
	GLFWValueControl::GLFWValueControl()
		: GLFWInputControl()
	{

	}

	GLFWValueControl::GLFWValueControl(uint8_t* device_state_block, const InputControlInfo& info, const InputControlLayout& layout)
		: GLFWInputControl(device_state_block, info, layout)
	{

	}

	GLFWValueControl::~GLFWValueControl()
	{

	}

	bool GLFWValueControl::Update(void* value, const uint16_t& value_size)
	{
		if (value == nullptr)
		{
			AURION_ERROR("[GLFWValueControl] Error: Failed to set control value. Reason: Invalid value pointer");
			return false;
		}

		if (value_size > m_layout.size_in_bytes)
		{
			AURION_ERROR("[GLFWValueControl] Error: Failed to set control value. Reason: Invalid value size");
			return false;
		}

		if (value_size == sizeof(int) && (m_info.type & GLFW_INPUT_CONTROL_INT))
		{
			*((int*)m_state_block) = *((int*)value);
			return true;
		}
		else if (value_size == sizeof(float) && (m_info.type & GLFW_INPUT_CONTROL_FLOAT))
		{
			*((float*)m_state_block) = *((float*)value);
			return true;
		}
		else if (value_size == sizeof(double) && (m_info.type & GLFW_INPUT_CONTROL_DOUBLE))
		{
			*((double*)m_state_block) = *((double*)value);
			return true;
		}

		AURION_ERROR("[GLFWValueControl] Error: Failed to set control value. Reason: Mismatched values. Control size: %d, provided: %d", m_layout.size_in_bytes, value_size);

		return false;
	}

	void GLFWValueControl::ReadValue(int* out_value)
	{
		if (out_value == nullptr)
		{
			AURION_ERROR("[GLFWValueControl] Error: Failed to read control value. Reason: Invalid output variable");
			return;
		}

		if (m_info.type & GLFW_INPUT_CONTROL_INT)
		{
			*out_value = *((int*)m_state_block);
		}
		else if (m_info.type & GLFW_INPUT_CONTROL_FLOAT)
		{
			*out_value = static_cast<int>(*((float*)m_state_block));
		}
		else if (m_info.type & GLFW_INPUT_CONTROL_DOUBLE)
		{
			*out_value = static_cast<int>(*((double*)m_state_block));
		}
	}

	void GLFWValueControl::ReadValue(float* out_value)
	{
		if (out_value == nullptr)
		{
			AURION_ERROR("[GLFWValueControl] Error: Failed to read control value. Reason: Invalid output variable");
			return;
		}

		if (m_info.type & GLFW_INPUT_CONTROL_INT)
		{
			*out_value = static_cast<float>(*((float*)m_state_block));
		}
		else if (m_info.type & GLFW_INPUT_CONTROL_FLOAT)
		{
			*out_value = *((float*)m_state_block);
		}
		else if (m_info.type & GLFW_INPUT_CONTROL_DOUBLE)
		{
			*out_value = static_cast<float>(*((double*)m_state_block));
		}
	}

	void GLFWValueControl::ReadValue(double* out_value)
	{
		if (out_value == nullptr)
		{
			AURION_ERROR("[GLFWValueControl] Error: Failed to read control value. Reason: Invalid output variable");
			return;
		}

		if (m_info.type & GLFW_INPUT_CONTROL_INT)
		{
			*out_value = static_cast<double>(*((int*)m_state_block));
		}
		else if (m_info.type & GLFW_INPUT_CONTROL_FLOAT)
		{
			*out_value = static_cast<double>(*((float*)m_state_block));
		}
		else if (m_info.type & GLFW_INPUT_CONTROL_DOUBLE)
		{
			*out_value = *((double*)m_state_block);
		}
	}
}