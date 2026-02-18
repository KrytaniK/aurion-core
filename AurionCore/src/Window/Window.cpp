module Aurion.Window;

import Aurion.Types;

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
		// Windows are open so long as a native handle exists
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

	void Window::SetMode(const WindowMode& mode)
	{
		m_properties.mode = mode;
	}

	void Window::Resize(const u16& width, const u16& height)
	{
		m_properties.width = width;
		m_properties.height = height;
	}

	void Window::MoveTo(const u16& x, const u16& y)
	{
		m_properties.x = x;
		m_properties.y = y;
	}

	void Window::Focus()
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