module;

#include <macros/AurionExport.h>
#include <cstdint>

export module Aurion.GLFW:Driver;

import Aurion.Window;

import :Window;

export namespace Aurion
{
	class AURION_API GLFWDriver : public IWindowDriver
	{
	public:
		GLFWDriver(const int& client_api = 0);
		GLFWDriver(const int& client_api, const size_t& max_window_count);
		virtual ~GLFWDriver();

		WindowHandle OpenWindow(const WindowProperties& properties) override;
		bool CloseWindow(const WindowHandle& handle) override;
		bool CloseWindow(const char* title) override;

		virtual WindowHandle GetWindow(const char* title) override;
		virtual WindowHandle GetWindow(const uint64_t& id) override;

	private:
		size_t m_max_window_count;
		GLFW_Window* m_windows;
	};
}