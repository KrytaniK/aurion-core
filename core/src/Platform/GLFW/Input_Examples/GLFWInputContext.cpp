#include <macros/AurionLog.h>

#include <string>
#include <new>

import Aurion.GLFW;
import Aurion.Input;

namespace Aurion
{
	GLFWInputContext::GLFWInputContext()
		: m_max_device_count(10), m_max_layout_count(10)
	{
		// Initialize pool allocators
		m_device_allocator.Initialize(sizeof(GLFWInputDevice), m_max_device_count);
		m_layout_allocator.Initialize(sizeof(InputDeviceLayout), m_max_layout_count);

		// Map arrays
		m_devices = (GLFWInputDevice*)m_device_allocator.Allocate();
		m_layouts = (InputDeviceLayout*)m_layout_allocator.Allocate();

		// Add array begin back to the free list for each allocator.
		//	NOTE: This does not free the managed memory.
		m_device_allocator.Free(m_devices);
		m_layout_allocator.Free(m_layouts);
	}

	GLFWInputContext::~GLFWInputContext()
	{
		// Manually call device destructors
		for (size_t i = 0; i < m_max_device_count; i++)
		{
			if (m_device_allocator.IsMapped(&m_devices[i]))
				m_devices[i].~GLFWInputDevice();
		}

		// Free arrays in each device layout
		for (size_t i = 0; i < m_max_layout_count; i++)
		{
			if (m_layout_allocator.IsMapped(&m_layouts[i]))
			{
				delete[] m_layouts[i].control_infos;
				delete[] m_layouts[i].control_layouts;
			}
		}
	}

	GLFWInputDevice* GLFWInputContext::CreateDevice(const InputDeviceInfo& info, const uint32_t& layout_id)
	{
		GLFWInputDevice* next_device = (GLFWInputDevice*)m_device_allocator.Allocate();

		if (next_device == nullptr)
		{
			AURION_ERROR("[GLFWInputContext] Error: Failed to add input device. Reason: device limit of %d reached.", m_max_device_count);
			return nullptr;
		}

		InputDeviceLayout* layout = this->GetLayout(layout_id);

		if (layout == nullptr)
		{
			AURION_ERROR("[GLFWInputContext] Error: Failed to add input device. Reason: Specified device layout with id %d does not exist.", layout_id);
			return nullptr;
		}

		// Construct via placement new
		next_device = new (next_device) GLFWInputDevice();
		next_device->Initialize(info, *layout);

		return next_device;
	}

	GLFWInputDevice* GLFWInputContext::CreateDevice(const InputDeviceInfo& info, const char* layout_name)
	{
		GLFWInputDevice* next_device = (GLFWInputDevice*)m_device_allocator.Allocate();

		if (next_device == nullptr)
		{
			AURION_ERROR("[GLFWInputContext] Error: Failed to add input device. Reason: device limit of %d reached.", m_max_device_count);
			return nullptr;
		}

		InputDeviceLayout* layout = this->GetLayout(layout_name);

		if (layout == nullptr)
		{
			AURION_ERROR("[GLFWInputContext] Error: Failed to add input device. Reason: Specified device layout '%s' does not exist.", layout);
			return nullptr;
		}

		// Construct via placement new
		next_device = new (next_device) GLFWInputDevice();
		next_device->Initialize(info, *layout);

		return next_device;
	}

	GLFWInputDevice* GLFWInputContext::CreateDevice(const InputDeviceInfo& info, const InputDeviceLayout& layout)
	{
		GLFWInputDevice* next_device = (GLFWInputDevice*)m_device_allocator.Allocate();

		if (next_device == nullptr)
		{
			AURION_ERROR("[GLFWInputContext] Error: Failed to add input device. Reason: device limit of %d reached.", m_max_device_count);
			return nullptr;
		}

		// Construct via placement new
		next_device = new (next_device) GLFWInputDevice();
		next_device->Initialize(info, layout);

		return next_device;
	}

	GLFWInputDevice* GLFWInputContext::GetDevice(const uint64_t& id)
	{
		// If the beginning of the array isn't mapped, no devices exist
		if (!m_device_allocator.IsMapped(m_devices))
			return nullptr;

		for (size_t i = 0; i < m_max_device_count; i++)
			if (m_device_allocator.IsMapped(m_devices + i))
				if (m_devices[i].GetInfo().id == id)
					return &m_devices[i];

		return nullptr;
	}

	GLFWInputDevice* GLFWInputContext::GetDevice(const char* name)
	{
		// If the beginning of the array isn't mapped, no devices exist
		if (!m_device_allocator.IsMapped(m_devices))
			return nullptr;

		for (size_t i = 0; i < m_max_device_count; i++)
			if (m_device_allocator.IsMapped(m_devices + i))
				if (strcmp(m_devices[i].GetInfo().alias, name) == 0)
					return &m_devices[i];

		return nullptr;
	}

	bool GLFWInputContext::RemoveDevice(const uint64_t& id)
	{
		// If the beginning of the array isn't mapped, no devices exist
		if (!m_device_allocator.IsMapped(m_devices))
			return false;

		GLFWInputDevice* device = this->GetDevice(id);

		if (device == nullptr)
			return false;

		m_device_allocator.Free(device);
		return true;
	}

	bool GLFWInputContext::RemoveDevice(const char* name)
	{
		// If the beginning of the array isn't mapped, no devices exist
		if (!m_device_allocator.IsMapped(m_devices))
			return false;

		GLFWInputDevice* device = this->GetDevice(name);

		if (device == nullptr)
			return false;

		m_device_allocator.Free(device);
		return true;
	}

	bool GLFWInputContext::RemoveDevice(IInputDevice* device)
	{
		// If the beginning of the array isn't mapped, no devices exist
		if (!m_device_allocator.IsMapped(m_devices))
			return false;

		// If the provided device location isn't mapped, it doesn't exist in this context.
		if (!m_device_allocator.IsMapped(device))
			return false;

		m_device_allocator.Free(device);
		return true;
	}

	InputDeviceLayout* GLFWInputContext::GetLayout(const uint32_t& layout_id)
	{
		// If the beginning of the array isn't mapped, no layouts exist
		if (!m_layout_allocator.IsMapped(m_layouts))
			return nullptr;

		for (size_t i = 0; i < m_max_layout_count; i++)
			if (m_layout_allocator.IsMapped(m_layouts + i))
				if (m_layouts[i].id == layout_id)
					return &m_layouts[i];

		return nullptr;
	}

	InputDeviceLayout* GLFWInputContext::GetLayout(const char* name)
	{
		// If the beginning of the array isn't mapped, no layouts exist
		if (!m_layout_allocator.IsMapped(m_layouts))
			return nullptr;

		for (size_t i = 0; i < m_max_layout_count; i++)
			if (m_layout_allocator.IsMapped(m_layouts + i))
				if (strcmp(m_layouts[i].name, name) == 0)
					return &m_layouts[i];

		return nullptr;
	}

	void GLFWInputContext::AddDeviceLayout(const InputDeviceLayout& layout)
	{
		InputDeviceLayout* new_layout = (InputDeviceLayout*)m_layout_allocator.Allocate();

		// If no new region of memory was given, the allocator is out of memory
		if (new_layout == nullptr)
		{
			AURION_ERROR("[GLFWInputContext] Error: Failed to add device layout. Reason: layout limit of %d reached.", m_max_device_count);
			return;
		}

		*new_layout = layout;
	}

	bool GLFWInputContext::RemoveDeviceLayout(const uint32_t& id)
	{
		// If the beginning of the array isn't mapped, no layouts exist
		if (!m_layout_allocator.IsMapped(m_layouts))
			return false;

		InputDeviceLayout* layout = this->GetLayout(id);

		if (layout == nullptr)
			return false;

		m_layout_allocator.Free(layout);
		return true;
	}

	bool GLFWInputContext::RemoveDeviceLayout(const char* name)
	{
		// If the beginning of the array isn't mapped, no layouts exist
		if (!m_layout_allocator.IsMapped(m_layouts))
			return false;

		InputDeviceLayout* layout = this->GetLayout(name);

		if (layout == nullptr)
			return false;

		m_layout_allocator.Free(layout);
		return true;
	}

	bool GLFWInputContext::RemoveDeviceLayout(const InputDeviceLayout& layout)
	{
		// If the beginning of the array isn't mapped, no layouts exist
		if (!m_layout_allocator.IsMapped(m_layouts))
			return false;

		InputDeviceLayout* layout_ptr = this->GetLayout(layout.id);

		if (layout_ptr == nullptr)
			return false;

		m_layout_allocator.Free(layout_ptr);
		return true;
	}
}