#include <macros/AurionLog.h>

#include <cstdlib>
#include <new>

import Aurion.GLFW;

namespace Aurion
{
	GLFWInputDevice::GLFWInputDevice()
		: m_info({}), m_layout({}), m_state_block(nullptr)
	{

	}

	GLFWInputDevice::GLFWInputDevice(const InputDeviceInfo& info, const InputDeviceLayout& layout)
		: m_info({}), m_layout({}), m_state_block(nullptr)
	{
		this->Initialize(info, layout);
	}

	GLFWInputDevice::~GLFWInputDevice()
	{
		if (m_state_block != nullptr)
			free(m_state_block);
		
		// Call destructors manually on all controls. Allocators do not destruct objects
		for (size_t i = 0; i < m_layout.control_count; i++)
			m_all_controls[i]->~IInputControl();

		// Delete the array holding control references
		if (m_all_controls)
			delete[] m_all_controls;
	}

	const InputDeviceInfo& GLFWInputDevice::GetInfo()
	{
		return m_info;
	}

	const uint32_t& GLFWInputDevice::GetLayoutID()
	{
		return m_layout.id;
	}

	IInputControl* GLFWInputDevice::GetControl(const uint64_t& input_code)
	{
		if (m_all_controls == nullptr)
			return nullptr;

		for (size_t i = 0; i < m_layout.control_count; i++)
			if (m_all_controls[i] && m_all_controls[i]->GetInputCode() == input_code)
				return m_all_controls[i];

		return nullptr;
	}

	bool GLFWInputDevice::Map(const InputDeviceLayout& layout)
	{
		if (m_state_block == nullptr || layout.size > m_layout.size || layout.size == 0)
		{
			AURION_ERROR("[GLFWInputDevice] Error: Failed to map device controls. Reason: Invalid state or layout size");
			return false;
		}
		
		if (layout.control_count == 0 || layout.control_infos == nullptr || layout.control_layouts == nullptr)
		{
			AURION_ERROR("[GLFWInputDevice] Error: Failed to map device controls. Reason: Invalid layout controls");
			return false;
		}

		// Initialize array of control pointer references
		m_all_controls = new IInputControl*[layout.control_count];

		// Count the number of each unique control type
		size_t button_count = 0;
		size_t value_count = 0;
		size_t axis_count = 0;
		for (size_t i = 0; i < layout.control_count; i++)
		{
			uint64_t c_type = layout.control_infos[i].type;
			if (c_type <= GLFW_INPUT_CONTROL_BUTTON)
				button_count++;
			else if (c_type <= GLFW_INPUT_CONTROL_DOUBLE)
				value_count++;
			else if (c_type >= GLFW_INPUT_CONTROL_AXIS_2D)
				axis_count++;
		}

		// Initialize unique control allocator
		m_button_allocator.Initialize(sizeof(GLFWButtonControl), button_count);
		m_value_allocator.Initialize(sizeof(GLFWValueControl), value_count);
		m_axis_allocator.Initialize(sizeof(GLFWAxisControl), axis_count);

		// Initialize Each Control
		for (size_t i = 0; i < layout.control_count; i++)
		{
			uint64_t c_type = layout.control_infos[i].type;

			if (c_type == 0)
			{
				AURION_ERROR("[GLFWInputDevice] Invalid Control Type of 0");
				continue;
			}

			if (c_type <= GLFW_INPUT_CONTROL_BUTTON)
			{

				// Allocate control memory
				GLFWButtonControl* button_ctrl = (GLFWButtonControl*)m_button_allocator.Allocate();

				// Construct via placement new and initialize
				button_ctrl = new (button_ctrl) GLFWButtonControl();
				button_ctrl->Initialize(m_state_block, layout.control_infos[i], layout.control_layouts[i]);

				// Store reference in internal array
				m_all_controls[i] = (IInputControl*)button_ctrl;
			}
			else if (c_type <= GLFW_INPUT_CONTROL_DOUBLE)
			{

				// Allocate control memory
				GLFWValueControl* value_ctrl = (GLFWValueControl*)m_value_allocator.Allocate();

				// Construct via placement new and initialize
				value_ctrl = new (value_ctrl) GLFWValueControl();
				value_ctrl->Initialize(m_state_block, layout.control_infos[i], layout.control_layouts[i]);

				// Store reference in internal array
				m_all_controls[i] = (IInputControl*)value_ctrl;
			}
			else if (c_type >= GLFW_INPUT_CONTROL_AXIS_2D)
			{

				// Allocate control memory
				GLFWAxisControl* axis_ctrl = (GLFWAxisControl*)m_axis_allocator.Allocate();

				// Construct via placement new and initialize
				axis_ctrl = new (axis_ctrl) GLFWAxisControl();
				axis_ctrl->Initialize(m_state_block, layout.control_infos[i], layout.control_layouts[i]);

				// Store reference in internal array
				m_all_controls[i] = (IInputControl*)axis_ctrl;
			}
			else
				AURION_ERROR("[GLFWInputDevice] Failed to create control for {%s}, type: %d", layout.control_infos[i].name, layout.control_infos[i].type);
		}

		return true;
	}

	void GLFWInputDevice::Initialize(const InputDeviceInfo& info, const InputDeviceLayout& layout)
	{
		m_info = info;
		m_layout = layout;

		// Allocate Device memory block
		if (layout.size > 0)
			m_state_block = (uint8_t*)calloc(1, layout.size);

		// Map layout controls to device memory
		this->Map(layout);
	}
}