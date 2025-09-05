#include <macros/AurionLog.h>

#include <GLFW/glfw3.h>

import Aurion.GLFW;

namespace Aurion
{
	GLFW_Window::GLFW_Window()
		: Window(), m_native_monitor(nullptr), m_cached_properties({})
	{
		
	}

	GLFW_Window::GLFW_Window(const WindowProperties& properties)
		: Window(properties), m_native_monitor(nullptr), m_cached_properties(properties)
	{
		// Retrieve native monitor (primary is usually fine)
		m_native_monitor = glfwGetPrimaryMonitor();

		// Configure GLFW window
		glfwWindowHint(GLFW_RESIZABLE, properties.resizable);
		glfwWindowHint(GLFW_DECORATED, properties.decorated);

		// Create window
		CreateNativeWindow();

		// Validate window
		if (!m_native_handle)
		{
			AURION_ERROR("Failed to generate native GLFW window handle!");
			return;
		}

		// Retrieve window position
		int x, y;
		glfwGetWindowPos((GLFWwindow*)m_native_handle, &x, &y);
		m_properties.x = (uint16_t)x;
		m_properties.y = (uint16_t)y;
	}

	GLFW_Window::~GLFW_Window()
	{
		if (m_native_handle)
		{
			// Reset window user pointer
			glfwSetWindowUserPointer((GLFWwindow*)m_native_handle, nullptr);

			// Destroy native window
			glfwDestroyWindow((GLFWwindow*)m_native_handle);
		}
	}

	void GLFW_Window::Update(float deltaTime)
	{
		if (!m_native_handle)
			return;

		if (glfwWindowShouldClose((GLFWwindow*)m_native_handle))
		{
			// Reset window user pointer
			glfwSetWindowUserPointer((GLFWwindow*)m_native_handle, nullptr);

			// Destroy native window
			glfwDestroyWindow((GLFWwindow*)m_native_handle);

			// Reset internal state
			m_native_handle = nullptr;
			return;
		}

		glfwPollEvents();
	}

	void GLFW_Window::SetTitle(const char* title)
	{
		if (!m_native_handle)
			return;

		glfwSetWindowTitle((GLFWwindow*)m_native_handle, title);
	}

	void GLFW_Window::SetMode(const WindowMode& mode)
	{
		// Bail if there's not a valid handle or we're already in the
		//	existing mode
		if (!m_native_handle || m_properties.mode == mode)
			return;

		switch (mode)
		{
			case WindowMode::Windowed:
			{
				// Pull cached properties (restoring state)
				m_properties = m_cached_properties;

				// Exit fullscreen
				glfwSetWindowMonitor(
					(GLFWwindow*)m_native_handle, 
					NULL, // Null forces windowed mode
					(int)m_properties.x, 
					(int)m_properties.y, 
					(int)m_properties.width, 
					(int)m_properties.height, 
					NULL // Refresh rate ignored in this scenario
				);

				return;
			}
			case WindowMode::FullscreenExclusive:
			{
				// Cache current state
				m_cached_properties = m_properties;

				// Update the mode via parent method
				Window::SetMode(mode);

				// Enter fullscreen exclusive
				glfwSetWindowMonitor(
					(GLFWwindow*)m_native_handle,
					m_native_monitor, 
					0, 
					0, 
					(int)m_properties.width, 
					(int)m_properties.height, 
					GLFW_DONT_CARE
				);

				return;
			}
			case WindowMode::FullscreenBorderless:
			{
				// Cache current state if we weren't already in fullscreen
				if (!m_properties.fullscreen)
					m_cached_properties = m_properties;

				// Update the mode via parent method
				Window::SetMode(mode);

				// Get monitor video mode
				const GLFWvidmode* vidmode = glfwGetVideoMode(m_native_monitor);

				// Enter fullscreen borderless
				glfwSetWindowMonitor(
					(GLFWwindow*)m_native_handle,
					m_native_monitor, 
					0, 
					0, 
					vidmode->width, 
					vidmode->height, 
					vidmode->refreshRate
				);

				return;
			}
			default:
				return;
		}
	}

	void GLFW_Window::Resize(const uint16_t& width, const uint16_t& height)
	{
		if (!m_native_handle)
			return;

		glfwSetWindowSize((GLFWwindow*)m_native_handle, (int)width, (int)height);
	}

	void GLFW_Window::MoveTo(const uint16_t& xPos, const uint16_t& yPos)
	{
		if (!m_native_handle)
			return;

		glfwSetWindowPos((GLFWwindow*)m_native_handle, (int)xPos, (int)yPos);
	}

	void GLFW_Window::ToggleMinimize()
	{
		if (!m_native_handle)
			return;

		// Update internal properties via parent method
		Window::ToggleMinimize();

		// Toggle minimized state
		if (m_properties.minimized)
			glfwIconifyWindow((GLFWwindow*)m_native_handle);
		else
			glfwRestoreWindow((GLFWwindow*)m_native_handle);
	}

	void GLFW_Window::ToggleMaximize()
	{
		if (!m_native_handle)
			return;

		// Update internal properties via parent method
		Window::ToggleMaximize();

		// Toggle maximized state
		if (m_properties.maximized)
			glfwMaximizeWindow((GLFWwindow*)m_native_handle);
		else
			glfwRestoreWindow((GLFWwindow*)m_native_handle);
	}

	void GLFW_Window::Focus()
	{
		if (!m_native_handle || m_properties.minimized)
			return;

		// Update internal properties via parent method
		Window::Focus();

		glfwFocusWindow((GLFWwindow*)m_native_handle);
	}

	void GLFW_Window::ToggleDecoration()
	{
		if (!m_native_handle)
			return;

		bool decorated = glfwGetWindowAttrib((GLFWwindow*)m_native_handle, GLFW_DECORATED);

		glfwSetWindowAttrib(
			(GLFWwindow*)m_native_handle, 
			GLFW_DECORATED, 
			!decorated
		);
	}

	void GLFW_Window::CreateNativeWindow()
	{
		switch (m_properties.mode)
		{
			case WindowMode::Windowed:
			{
				// Create a windowed window
				m_native_handle = glfwCreateWindow(
					(int)m_properties.width,
					(int)m_properties.height,
					m_properties.title,
					NULL, // Don't enter fullscreen
					NULL // Don't share resources
				);

				if (!m_native_handle)
					AURION_ERROR("Failed to create windowed GLFW window");

				return;
			}
			case WindowMode::FullscreenExclusive:
			{
				// Set the cached width to the default window width
				// This will be used when restoring windowed state
				m_cached_properties.width = 1280;
				m_cached_properties.height = 720;

				// Grab the monitor video mode
				const GLFWvidmode* vidmode = glfwGetVideoMode(m_native_monitor);

				// Update properties
				m_properties.width = static_cast<uint16_t>(vidmode->width);
				m_properties.height = static_cast<uint16_t>(vidmode->height);

				// Create a fullscreen exclusive window
				m_native_handle = glfwCreateWindow(
					vidmode->width,
					vidmode->height,
					m_properties.title,
					m_native_monitor, // Enter fullscreen
					NULL // Don't share resources
				);

				if (!m_native_handle)
					AURION_ERROR("Failed to create fullscreen exclusive GLFW window");

				return;
			}
			case WindowMode::FullscreenBorderless:
			{
				// Set the cached width to the default window width
				// This will be used when restoring windowed state
				m_cached_properties.width = 1280;
				m_cached_properties.height = 720;

				// Get monitor video mode
				const GLFWvidmode* vidmode = glfwGetVideoMode(m_native_monitor);

				// Update properties
				m_properties.width = static_cast<uint16_t>(vidmode->width);
				m_properties.height = static_cast<uint16_t>(vidmode->height);

				// Set GLFW hints
				glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
				glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
				glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
				glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);

				// Create Windowed Fullscreen window
				m_native_handle = glfwCreateWindow(
					vidmode->width, 
					vidmode->height, 
					m_properties.title, 
					m_native_monitor, 
					NULL
				);

				if (!m_native_handle)
					AURION_ERROR("Failed to create fullscreen borderless GLFW window");

				return;
			}
			default:
				return;
		}
	}
}