#include <macros/AurionLog.h>

#include <cstdint>

import Aurion.Window;

namespace Aurion
{
	Window::Window()
		: m_native_handle(nullptr), m_properties({})
	{

	}

	Window::Window(const WindowProperties& properties)
		: m_native_handle(nullptr), m_properties(properties)
	{
		
	}

	void* Window::GetNativeHandle()
	{
		return m_native_handle;
	}

	const WindowProperties& Window::GetProperties()
	{
		return m_properties;
	}

	bool Window::IsOpen()
	{
		return m_native_handle != nullptr;
	}

	bool Window::IsFullscreen(const bool& exclusive)
	{
		// For Fullscreen exclusive modes
		if (exclusive)
			return m_properties.fullscreen && !m_properties.maximized && !m_properties.resizable;

		// All other modes, simply return the property
		return m_properties.fullscreen;
	}

	void Window::ToggleDecoration()
	{
		m_properties.decorated = !m_properties.decorated;
	}

	void Window::SetTitle(const char* title)
	{
		m_properties.title = title;
	}

	void Window::Resize(const uint16_t& width, const uint16_t& height)
	{
		m_properties.width = width;
		m_properties.height = height;
	}

	void Window::MoveTo(const uint16_t& x, const uint16_t& y)
	{
		m_properties.x = x;
		m_properties.y = y;
	}

	void Window::ToggleFocus()
	{
		m_properties.focused = !m_properties.focused;
	}

	void Window::ToggleMinimize()
	{
		m_properties.minimized = !m_properties.minimized;
	}

	void Window::ToggleMaximize()
	{
		m_properties.maximized = !m_properties.maximized;
	}
}