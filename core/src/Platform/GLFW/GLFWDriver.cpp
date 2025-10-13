module Aurion.GLFW;

import Aurion.Application;
import Aurion.Types;
import Aurion.Window;

import <macros/AurionLog.h>;
import <GLFW/glfw3.h>;
import <iostream>;
import <cassert>;
import <string>;

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

		// Attach window event handler if an application instance exists
		if (Application::s_instance)
		{
			Application::Events()->Register(
				EventCategories::AC_EVENT_CATEGORY_WINDOW,
				&GLFWDriver::HandleEvent,
				this
			);
		}
	}

	GLFWDriver::~GLFWDriver()
	{
		if (!m_windows)
			return;

		delete[] m_windows;
	}

	// Static Event Handler
	void GLFWDriver::HandleEvent(EventBase* event, void* context)
	{
		assert(event != nullptr && "[GLFWDriver::HandleEvent] Event pointer must not be null!");
		assert(context != nullptr && "[GLFWDriver::HandleEvent] Context pointer must not be null!");

		// Don't process handled events
		if (event->IsHandled())
			return;

		// The context in this case should ALWAYS be a GLFWDriver instance
		GLFWDriver* _this = static_cast<GLFWDriver*>(context);

		// Handle as a window event, based on the type of event
		switch (event->GetType())
		{
			case AC_WIN_EVENT_CREATE:
			{
				WindowCreateEvent& create_event = *static_cast<WindowCreateEvent*>(event);

				// Open/Create a window with the GLFW driver context
				_this->OpenWindow(create_event.properties);

				// Mark event as handled
				event->Handle();
				break;
			}
			case AC_WIN_EVENT_CLOSE:
			{
				WindowCloseEvent& close_event = *static_cast<WindowCloseEvent*>(event);

				// Close a window with the GLFW driver context
				_this->CloseWindow(close_event.id);

				// Mark event as handled
				event->Handle();
				break;
			}
			case AC_WIN_EVENT_GET:
			{
				WindowGetEvent& get_event = *static_cast<WindowGetEvent*>(event);

				// Search for a window with the GLFW driver context,
				//	either by title or id as a fallback
				if (get_event.title)
				{
					get_event.out_handle = _this->GetWindow(get_event.title);
				}
				else
				{
					get_event.out_handle = _this->GetWindow(get_event.id);
				}

				// Mark event as handled
				event->Handle();
				break;
			}
			default:
			{
				AURION_WARN("[GLFWDriver::HandleEvent] Unhandled Event Type");
				return;
			}
		}
	}

	// Instance methods
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