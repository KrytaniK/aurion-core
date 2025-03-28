#include <macros/AurionLog.h>

#include <string>
#include <iostream>

#include <GLFW/glfw3.h>

import Aurion.Window;
import Aurion.GLFW;

namespace Aurion
{
	GLFWDriver::GLFWDriver()
		: m_max_window_count(1), m_windows(nullptr), m_window_ids(nullptr)
	{
		glfwSetErrorCallback([](int err, const char* desc) {
			AURION_ERROR("[GLFW] Error (%d)  %s", err, desc);
		});

		glfwInit();
	}

	GLFWDriver::~GLFWDriver()
	{
		if (!m_windows)
			return;

		delete[] m_windows;
	}

	void GLFWDriver::Initialize(const WindowDriverConfig& config)
	{
		m_max_window_count = config.max_window_count;

		// Allocate enough space for the max # of windows
		m_windows = new GLFW_Window[config.max_window_count];

		// Allocate and initialize all IDs to 0
		m_window_ids = new uint64_t[config.max_window_count](-1);
	}

	WindowHandle GLFWDriver::GetWindow(const char* title)
	{
		if (m_windows == nullptr)
			return WindowHandle{};

		// Future Todo: Replace with custom hashing function
		std::hash<const char*> hash; 
		return GetWindow((uint64_t)hash(title));
	}

	WindowHandle GLFWDriver::GetWindow(const uint64_t& id)
	{
		if (m_windows == nullptr || id == -1)
			return WindowHandle{};

		for (size_t i = 0; i < m_max_window_count; i++)
			if (m_window_ids[i] == id)
				return WindowHandle{ id, &m_windows[i] };

		return WindowHandle{};
	}

	WindowHandle GLFWDriver::InitWindow(const WindowConfig& config)
	{
		// Can't initialize a window if no memory was allocated
		if (m_windows == nullptr)
			return WindowHandle{};

		// Future Todo: Replace with custom hashing function
		std::hash<const char*> hash;

		// Find the first empty window, and insert there
		for (size_t i = 0; i < m_max_window_count; i++)
		{
			if (m_window_ids[i] == -1)
			{
				// Hash the window title to get window id
				m_window_ids[i] = (uint64_t)hash(config.title);

				// Open the window with the current config
				m_windows[i].Open(config);

				// Return a handle to the window
				return WindowHandle{ m_window_ids[i], &m_windows[i] };
			}
		}

		return WindowHandle{};
	}

	bool GLFWDriver::RemoveWindow(const char* title)
	{
		// Can't remove a window if no memory was allocated
		if (m_windows == nullptr)
			return false;

		// Future Todo: Replace with custom hashing function
		std::hash<const char*> hash;
		return RemoveWindow((uint64_t)hash(title));
	}

	bool GLFWDriver::RemoveWindow(const uint64_t& id)
	{
		if (m_windows == nullptr || id == -1)
			return false;

		// Search for the window
		for (size_t i = 0; i < m_max_window_count; i++)
		{
			if (id == m_window_ids[i])
			{
				// If found, close the window (Resets state),
				//	reset the corresponding window ID
				m_windows[i].Close();
				m_window_ids[i] = (uint64_t)(-1);
			}
		}

		return true;
	}

	bool GLFWDriver::RemoveWindow(IWindow* window)
	{
		if (m_windows == nullptr || window == nullptr)
			return false;

		// Search for the window
		for (size_t i = 0; i < m_max_window_count; i++)
		{
			if (window == &m_windows[i])
			{
				// If found, close the window (Resets state),
				//	reset the corresponding window ID, and
				//	null any references to it.
				m_windows[i].Close();
				m_window_ids[i] = (uint64_t)(-1);
				window = nullptr;
			}
		}

		return true;
	}
}