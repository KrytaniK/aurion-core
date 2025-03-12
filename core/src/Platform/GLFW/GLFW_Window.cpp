#include <GLFW/glfw3.h>

import Aurion.GLFW;

namespace Aurion
{
	GLFW_Window::GLFW_Window()
		: m_native_monitor(nullptr), m_native_window(nullptr), m_state({}), m_state_cached({})
	{

	}

	GLFW_Window::~GLFW_Window()
	{
		Close();
	}

	void GLFW_Window::Open(const WindowConfig& config)
	{

		// Copy Config data to window state
		m_state.title = config.title;
		m_state.width = config.width;
		m_state.height = config.height;
		m_state.mode = config.windowMode;

		// Retrieve native monitor (primary is almost always fine)
		m_native_monitor = glfwGetPrimaryMonitor();

		// Configure GLFW window
		glfwWindowHint(GLFW_RESIZABLE, config.resizable);
		glfwWindowHint(GLFW_DECORATED, config.decorated);

		// GLFW Client API (None for now)
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		// Create window
		CreateNativeWindow();

		// Validate window
		if (!m_native_window)
			return;

		// Retrieve window position
		int x, y;
		glfwGetWindowPos(m_native_window, &x, &y);
		m_state.xPos = (uint16_t)x;
		m_state.yPos = (uint16_t)y;

		// Set user pointer
		glfwSetWindowUserPointer(m_native_window, this);

		// Set GLFW Callbacks
		this->SetGLFWCallbacks();
	}

	void GLFW_Window::Close()
	{
		if (m_native_window)
		{
			// Reset window user pointer
			glfwSetWindowUserPointer(m_native_window, nullptr);

			// Destroy native window
			glfwDestroyWindow(m_native_window);
		}

		// Null internal references
		m_native_window = nullptr;
		m_native_monitor = nullptr;

		// Reset window state
		m_state = {};
		m_state_cached = {};
	}

	void GLFW_Window::Update(float deltaTime)
	{
		if (!m_native_window)
			return;

		glfwPollEvents();
	}

	void GLFW_Window::SetTitle(const char* title)
	{
		if (!m_native_window)
			return;

		m_state.title = title;
		glfwSetWindowTitle(m_native_window, m_state.title);
	}

	void GLFW_Window::SetMode(const WindowMode& mode)
	{
		if (!m_native_window)
			return;

		switch (mode)
		{
			case WINDOW_MODE_WINDOWED:
			{
				if (m_state.mode == WINDOW_MODE_WINDOWED)
					return;

				// Pull cached state
				m_state = m_state_cached;

				// Exit fullscreen
				glfwSetWindowMonitor(m_native_window, NULL, (int)m_state.xPos, (int)m_state.yPos, (int)m_state.width, (int)m_state.height, NULL);

				return;
			}
			case WINDOW_MODE_FULLSCREEN_EXCLUSIVE:
			{
				if (m_state.mode == WINDOW_MODE_FULLSCREEN_EXCLUSIVE)
					return;

				// Cache current state
				m_state_cached = m_state;

				// Set new state mode
				m_state.mode = mode;

				// Enter fullscreen exclusive
				glfwSetWindowMonitor(m_native_window, m_native_monitor, 0, 0, (int)m_state.width, (int)m_state.height, NULL);

				return;
			}
			case WINDOW_MODE_FULLSCREEN_BORDERLESS:
			{
				if (m_state.mode == WINDOW_MODE_FULLSCREEN_BORDERLESS)
					return;

				// Cache current state if we weren't already in fullscreen
				if (m_state.mode == WINDOW_MODE_WINDOWED)
					m_state_cached = m_state;

				// Set new state mode
				m_state.mode = mode;

				// Get monitor video mode
				const GLFWvidmode* vidmode = glfwGetVideoMode(m_native_monitor);

				// Enter fullscreen borderless
				glfwSetWindowMonitor(m_native_window, m_native_monitor, 0, 0, vidmode->width, vidmode->height, vidmode->refreshRate);

				return;
			}
			default:
				return;
		}
	}

	void GLFW_Window::Resize(const uint16_t& width, const uint16_t& height)
	{
		if (!m_native_window)
			return;

		glfwSetWindowSize(m_native_window, (int)width, (int)height);
	}

	void GLFW_Window::SetPos(const uint16_t& xPos, const uint16_t& yPos)
	{
		if (!m_native_window)
			return;

		glfwSetWindowPos(m_native_window, (int)xPos, (int)yPos);
	}

	bool GLFW_Window::Minimize()
	{
		if (!m_native_window)
			return false;

		// Toggle minimized state
		if (!m_state.minimized)
			glfwIconifyWindow(m_native_window);
		else
			glfwRestoreWindow(m_native_window);

		// Swap state
		m_state.minimized = !m_state.minimized;
		m_state.maximized = false;

		// Return state value
		return m_state.minimized;
	}

	bool GLFW_Window::Maximize()
	{
		if (!m_native_window)
			return false;

		// Toggle maximized state
		if (!m_state.maximized)
			glfwMaximizeWindow(m_native_window);
		else
			glfwRestoreWindow(m_native_window);

		// Swap state
		m_state.maximized = !m_state.maximized;
		m_state.minimized = false;

		// Return state value
		return m_state.maximized;
	}

	bool GLFW_Window::Focus()
	{
		if (!m_native_window || m_state.minimized)
			return false;

		glfwFocusWindow(m_native_window);

		return true;
	}

	bool GLFW_Window::ToggleDecoration()
	{
		if (!m_native_window)
			return false;

		bool decorated = glfwGetWindowAttrib(m_native_window, GLFW_DECORATED);

		glfwSetWindowAttrib(m_native_window, GLFW_DECORATED, !decorated);

		return !decorated;
	}

	const char* GLFW_Window::GetTitle()
	{
		return m_state.title;
	}

	uint16_t GLFW_Window::GetWidth()
	{
		return m_state.width;
	}

	uint16_t GLFW_Window::GetHeight()
	{
		return m_state.height;
	}

	void* GLFW_Window::GetNativeHandle()
	{
		return m_native_window;
	}

	const WindowProperties& GLFW_Window::GetProperties() const
	{
		return m_state;
	}

	bool GLFW_Window::IsOpen()
	{
		return m_native_window != nullptr;
	}

	bool GLFW_Window::IsFullscreen()
	{
		return m_state.mode == WINDOW_MODE_FULLSCREEN_EXCLUSIVE || m_state.mode == WINDOW_MODE_FULLSCREEN_BORDERLESS;
	}

	void GLFW_Window::CreateNativeWindow()
	{
		if (!m_native_monitor)
			return;

		switch (m_state.mode)
		{
		case WINDOW_MODE_WINDOWED:
		{
			// Simply create the window
			m_native_window = glfwCreateWindow((int)m_state.width, (int)m_state.height, m_state.title, NULL, NULL);

			return;
		}
		case WINDOW_MODE_FULLSCREEN_EXCLUSIVE:
		{
			// Cache current state
			m_state_cached = m_state;

			// Create Fullscreen Exclusive window
			m_native_window = glfwCreateWindow((int)m_state.width, (int)m_state.height, m_state.title, m_native_monitor, NULL);

			return;
		}
		case WINDOW_MODE_FULLSCREEN_BORDERLESS:
		{
			// Cache current state if we weren't already in fullscreen
			if (m_state.mode == WINDOW_MODE_WINDOWED)
				m_state_cached = m_state;

			// Cache current state
			m_state_cached = m_state;

			// Get monitor video mode
			const GLFWvidmode* vidmode = glfwGetVideoMode(m_native_monitor);

			// Set GLFW hints
			glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);

			// Create Windowed Fullscreen window
			m_native_window = glfwCreateWindow(vidmode->width, vidmode->height, m_state.title, m_native_monitor, NULL);

			return;
		}
		default:
			return;
		}
	}

	void GLFW_Window::SetGLFWCallbacks()
	{
		glfwSetWindowCloseCallback(m_native_window, [](GLFWwindow* window) {
			GLFW_Window* _this = static_cast<GLFW_Window*>(glfwGetWindowUserPointer(window));

			if (!_this || !_this->m_native_window)
				return;

			// Close the GLFWWindow wrapping object
			_this->Close();
		});
	}
}