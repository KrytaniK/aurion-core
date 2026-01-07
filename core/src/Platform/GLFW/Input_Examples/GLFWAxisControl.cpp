#include <macros/AurionLog.h>

import Aurion.GLFW;

namespace Aurion
{
	GLFWAxisControl::GLFWAxisControl()
		: GLFWInputControl()
	{

	}

	GLFWAxisControl::GLFWAxisControl(uint8_t* device_state_block, const InputControlInfo& info, const InputControlLayout& layout)
		: GLFWInputControl(device_state_block, info, layout)
	{

	}

	GLFWAxisControl::~GLFWAxisControl()
	{

	}

	bool GLFWAxisControl::Update(void* value, const uint16_t& value_size)
	{
		if (value == nullptr)
		{
			AURION_ERROR("[GLFWAxisControl] Error: Failed to set control value. Reason: Invalid value pointer");
			return false;
		}

		if (value_size > m_layout.size_in_bytes)
		{
			AURION_ERROR("[GLFWAxisControl] Error: Failed to set control value. Reason: Invalid value size");
			return false;
		}

		// Determine which type of array values we're working with, and 
		//	write to at least the first two elements
		if (m_info.type & GLFW_INPUT_CONTROL_INT)
		{
			// Bounds check for 2D axis
			if (value_size < (2 * sizeof(int)))
			{
				AURION_ERROR("[GLFWAxisControl] Error: Failed to set control value. Reason: Invalid array size of %d. Expected: %d", value_size, m_layout.size_in_bytes);
				return false;
			}

			// Always write the first two values
			int* arr = (int*)m_state_block;
			int* value_arr = (int*)value;

			arr[0] = value_arr[0];
			arr[1] = value_arr[1];

			// Bounds check for 3D axis. No error
			if (value_size < (3 * sizeof(int)))
				return true;
			
			// If this is a 3D or 4D axis control, write the third value
			if ((m_info.type & GLFW_INPUT_CONTROL_AXIS_3D) || (m_info.type & GLFW_INPUT_CONTROL_AXIS_4D))
				arr[2] = value_arr[2];

			// Bounds check for 4D axis. No error
			if (value_size < (4 * sizeof(int)))
				return true;

			// If this is a 4D axis control, write the fourth value
			if (m_info.type & GLFW_INPUT_CONTROL_AXIS_4D)
				arr[3] = value_arr[3];

			return true;
		}
		else if (m_info.type & GLFW_INPUT_CONTROL_FLOAT)
		{
			// Bounds check for 2D axis
			if (value_size < (2 * sizeof(float)))
			{
				AURION_ERROR("[GLFWAxisControl] Error: Failed to set control value. Reason: Invalid array size of %d. Expected: %d", value_size, m_layout.size_in_bytes);
				return false;
			}

			// Always write the first two values
			float* arr = (float*)m_state_block;
			float* value_arr = (float*)value;

			arr[0] = value_arr[0];
			arr[1] = value_arr[1];

			// Bounds check for 3D axis. No error
			if (value_size < (3 * sizeof(float)))
				return true;

			// If this is a 3D or 4D axis control, write the third value
			if ((m_info.type & GLFW_INPUT_CONTROL_AXIS_3D) || (m_info.type & GLFW_INPUT_CONTROL_AXIS_4D))
				arr[2] = value_arr[2];

			// Bounds check for 4D axis. No error
			if (value_size < (4 * sizeof(float)))
				return true;

			// If this is a 4D axis control, write the fourth value
			if (m_info.type & GLFW_INPUT_CONTROL_AXIS_4D)
				arr[3] = value_arr[3];

			return true;
		}
		else if (m_info.type & GLFW_INPUT_CONTROL_DOUBLE)
		{
			// Bounds check for 2D axis
			if (value_size < (2 * sizeof(double)))
			{
				AURION_ERROR("[GLFWAxisControl] Error: Failed to set control value. Reason: Invalid array size of %d. Expected: %d", value_size, m_layout.size_in_bytes);
				return false;
			}

			// Always write the first two values
			double* arr = (double*)m_state_block;
			double* value_arr = (double*)value;

			arr[0] = value_arr[0];
			arr[1] = value_arr[1];

			// Bounds check for 3D axis. No error
			if (value_size < (3 * sizeof(double)))
				return true;

			// If this is a 3D or 4D axis control, write the third value
			if ((m_info.type & GLFW_INPUT_CONTROL_AXIS_3D) || (m_info.type & GLFW_INPUT_CONTROL_AXIS_4D))
				arr[2] = value_arr[2];

			// Bounds check for 4D axis. No error
			if (value_size < (4 * sizeof(double)))
				return true;

			// If this is a 4D axis control, write the fourth value
			if (m_info.type & GLFW_INPUT_CONTROL_AXIS_4D)
				arr[3] = value_arr[3];

			return true;
		}

		AURION_ERROR("[GLFWAxisControl] Error: Failed to set control value. Reason: Unsupported axis control type. Is [%d]", m_info.type);

		return false;
	}

	void GLFWAxisControl::ReadValues(bool* out_values, const uint8_t& read_count, uint8_t* out_count)
	{
		// Bail early if read count is 0
		if (read_count == 0)
		{
			AURION_ERROR("[GLFWAxisControl] Error: Failed to read control value. Reason: Invalid Read Count.");
			return;
		}

		// Set the number of ouput values
		uint8_t value_count = 0;
		if (m_info.type & GLFW_INPUT_CONTROL_AXIS_2D)
			value_count = 2;
		else if (m_info.type & GLFW_INPUT_CONTROL_AXIS_3D)
			value_count = 3;
		else if (m_info.type & GLFW_INPUT_CONTROL_AXIS_4D)
			value_count = 4;

		if (out_count != nullptr)
			*out_count = value_count;

		// Bail if all output params are bad, or if only the count was requested
		if (out_values == nullptr)
		{
			if (out_count == nullptr)
				AURION_ERROR("[GLFWAxisControl] Error: Failed to read control value. Reason: Invalid output parameters.");

			return;
		}

		// Offset state block pointer
		uint8_t* byte = m_state_block;

		// Determine the maximum number of reads
		uint8_t max_reads = read_count > value_count ? value_count : read_count;

		// If this axis is bit controlled, read the relevant bits
		if (m_layout.size_in_bits > 0)
		{
			// Only read up to the maximum supported/requested bits. This assumes child
			//	controls start at the same byte and bit offset as this control
			for (uint8_t i = 0; i < max_reads; i++)
			{
				// Generate a mask to isolate the desired bit
				uint8_t mask = (1U << (m_layout.bit_offset + i));

				// Apply the mask and convert to boolean
				out_values[i] = (*byte & mask) != 0;
			}

			return;
		}

		// Otherwise, treat like a boolean array
		bool* arr = (bool*)byte;

		// Only read up to the maximum supported / requested amount
		for (uint8_t i = 0; i < max_reads; i++)
			out_values[i] = arr[i];
	}

	void GLFWAxisControl::ReadValues(int* out_values, const uint8_t& read_count, uint8_t* out_count)
	{
		// Bail early if read count is 0
		if (read_count == 0)
		{
			AURION_ERROR("[GLFWAxisControl] Error: Failed to read control value. Reason: Invalid Read Count.");
			return;
		}
		
		// Set the number of ouput values
		uint8_t value_count = 0;
		if (m_info.type & GLFW_INPUT_CONTROL_AXIS_2D)
			value_count = 2;
		else if (m_info.type & GLFW_INPUT_CONTROL_AXIS_3D)
			value_count = 3;
		else if (m_info.type & GLFW_INPUT_CONTROL_AXIS_4D)
			value_count = 4;

		if (out_count != nullptr)
			*out_count = value_count;

		// Bail if all output params are bad, or if only the count was requested
		if (out_values == nullptr)
		{
			if (out_count == nullptr)
				AURION_ERROR("[GLFWAxisControl] Error: Failed to read control value. Reason: Invalid output parameters.");

			return;
		}

		// Offset state block pointer
		uint8_t* byte = m_state_block;

		int* arr = (int*)byte;

		// Determine the maximum number of reads
		uint8_t max_reads = read_count > value_count ? value_count : read_count;

		// Only read up to the maximum supported/requested amount
		for (uint8_t i = 0; i < max_reads; i++)
			out_values[i] = arr[i];
	}

	void GLFWAxisControl::ReadValues(float* out_values, const uint8_t& read_count, uint8_t* out_count)
	{
		// Bail early if read count is 0
		if (read_count == 0)
		{
			AURION_ERROR("[GLFWAxisControl] Error: Failed to read control value. Reason: Invalid Read Count.");
			return;
		}

		// Set the number of ouput values
		uint8_t value_count = 0;
		if (m_info.type & GLFW_INPUT_CONTROL_AXIS_2D)
			value_count = 2;
		else if (m_info.type & GLFW_INPUT_CONTROL_AXIS_3D)
			value_count = 3;
		else if (m_info.type & GLFW_INPUT_CONTROL_AXIS_4D)
			value_count = 4;

		if (out_count != nullptr)
			*out_count = value_count;

		// Bail if all output params are bad, or if only the count was requested
		if (out_values == nullptr)
		{
			if (out_count == nullptr)
				AURION_ERROR("[GLFWAxisControl] Error: Failed to read control value. Reason: Invalid output parameters.");

			return;
		}

		// Offset state block pointer
		uint8_t* byte = m_state_block;

		float* arr = (float*)byte;

		// Determine the maximum number of reads
		uint8_t max_reads = read_count > value_count ? value_count : read_count;

		// Only read up to the maximum supported/requested amount
		for (uint8_t i = 0; i < max_reads; i++)
			out_values[i] = arr[i];
	}

	void GLFWAxisControl::ReadValues(double* out_values, const uint8_t& read_count, uint8_t* out_count)
	{
		// Bail early if read count is 0
		if (read_count == 0)
		{
			AURION_ERROR("[GLFWAxisControl] Error: Failed to read control value. Reason: Invalid Read Count.");
			return;
		}

		/// Set the number of ouput values
		uint8_t value_count = 0;
		if (m_info.type & GLFW_INPUT_CONTROL_AXIS_2D)
			value_count = 2;
		else if (m_info.type & GLFW_INPUT_CONTROL_AXIS_3D)
			value_count = 3;
		else if (m_info.type & GLFW_INPUT_CONTROL_AXIS_4D)
			value_count = 4;

		if (out_count != nullptr)
			*out_count = value_count;

		// Bail if all output params are bad, or if only the count was requested
		if (out_values == nullptr)
		{
			if (out_count == nullptr)
				AURION_ERROR("[GLFWAxisControl] Error: Failed to read control value. Reason: Invalid output parameters.");

			return;
		}

		// Offset state block pointer
		uint8_t* byte = m_state_block;

		double* arr = (double*)byte;

		// Determine the maximum number of reads
		uint8_t max_reads = read_count > value_count ? value_count : read_count;

		// Only read up to the maximum supported/requested amount
		for (uint8_t i = 0; i < max_reads; i++)
			out_values[i] = arr[i];
	}
}