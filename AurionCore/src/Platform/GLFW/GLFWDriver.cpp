module;

#include <AurionLog.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <string>
#include <cstring>

module Aurion.GLFW;

import Aurion.Application;
import Aurion.Types;
import Aurion.Window;

namespace Aurion
{
	GLFWDriver::GLFWDriver(const int& client_api, const size_t& max_window_count)
		: m_max_window_count(max_window_count), m_windows(nullptr)
	{
		glfwSetErrorCallback([](int err, const char* desc) {
			AURION_ERROR("[GLFW] Error (%d)  %s", err, desc);
		});

		if (glfwInit() != GLFW_TRUE)
		{
			AURION_ERROR("Failed to initialize GLFW!");
		}

		glfwWindowHint(GLFW_CLIENT_API, client_api);

		// Allocate enough space for the max # of windows
		m_windows = new GLFW_Window[m_max_window_count];
	}

	GLFWDriver::~GLFWDriver()
	{
		if (!m_windows)
			return;

		delete[] m_windows;
	}

	WindowHandle GLFWDriver::OpenWindow(const WindowProperties& properties)
	{
		if (!m_windows)
		{
			AURION_ERROR("Failed to open window: Invalid container array");
			return WindowHandle{};
		}

		for (size_t i = 0; i < m_max_window_count; i++)
		{
			GLFW_Window& window = m_windows[i];

			// If a window isn't open, treat is as an open slot
			if (!window.IsOpen()) {
				
				// Destroy existing instance
				window.~GLFW_Window();
				
				// Then re-create window via placement new
				new (&window) GLFW_Window(properties);

				// Generate and return a handle for the created window
				return WindowHandle{
					// Index is always index + 1
					.id = (u64)(i + 1),
					.window = &m_windows[i]
				};
			}
		}

		AURION_ERROR("Failed to open window: Max window count of %d reached!", m_max_window_count);
		return WindowHandle();
	}

	bool GLFWDriver::CloseWindow(const WindowHandle& handle)
	{
		if (handle.id > m_max_window_count)
			return false;

		// Convert the id back into the index
		GLFW_Window& window = m_windows[handle.id - 1];

		// Return if the memory at that location has been reset
		if (!window.IsOpen())
			return true;

		// Destroy existing instance
		window.~GLFW_Window();

		// Then re-create a null window via placement new
		new (&window) GLFW_Window();
		return true;
	}

	bool GLFWDriver::CloseWindow(const char* title)
	{
		if (!title)
			return false;

		for (size_t i = 0; i < m_max_window_count; i++)
		{
			GLFW_Window& window = m_windows[i];

			// When the titles' match, reset that portion of memory
			if (strcmp(title, window.GetProperties().title) == 0)
			{
				// Destroy existing instance
				window.~GLFW_Window();

				// Then re-create a null window via placement new
				new (&window) GLFW_Window();
				return true;
			}
		}

		return false;
	}

	bool GLFWDriver::CloseWindow(const u64& id)
	{
		if (id > m_max_window_count)
			return false;

		// Convert the id back into the index
		GLFW_Window& window = m_windows[id - 1];

		// Return if the memory at that location has been reset
		if (!window.IsOpen())
			return true;

		// Destroy existing instance
		window.~GLFW_Window();

		// Then re-create a null window via placement new
		new (&window) GLFW_Window();
		return true;
	}

	WindowHandle GLFWDriver::GetWindow(const char* title)
	{
		if (!title)
			return WindowHandle();

		for (size_t i = 0; i < m_max_window_count; i++)
		{
			GLFW_Window& window = m_windows[i];

			// When the titles' match, return a handle to that window
			if (strcmp(title, window.GetProperties().title) == 0)
			{
				return WindowHandle{
					// Index is always index + 1
					.id = i + 1,
					.window = &window
				};
			}
		}

		return WindowHandle();
	}

	WindowHandle GLFWDriver::GetWindow(const u64& id)
	{
		if (id >= m_max_window_count || !m_windows[id].IsOpen())
			return WindowHandle();

		return WindowHandle{
			.id = id,
			.window = &m_windows[id]
		};
	}
}