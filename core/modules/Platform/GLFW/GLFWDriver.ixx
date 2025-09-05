module;

#include <macros/AurionExport.h>
#include <cstdint>

export module Aurion.GLFW:Driver;

import Aurion.Window;
import Aurion.Events;

import :Window;

export namespace Aurion
{
	class AURION_API GLFWDriver : public IWindowDriver
	{
	public:
		static void HandleEvent(EventBase* event, void* context);

	public:
		GLFWDriver(const int& client_api = 0);
		GLFWDriver(const int& client_api, const size_t& max_window_count);
		virtual ~GLFWDriver();

		virtual WindowHandle OpenWindow(const WindowProperties& properties) override;
		virtual bool CloseWindow(const WindowHandle& handle) override;
		virtual bool CloseWindow(const char* title) override;
		virtual bool CloseWindow(const uint64_t& id) override;

		virtual WindowHandle GetWindow(const char* title) override;
		virtual WindowHandle GetWindow(const uint64_t& id) override;

	private:
		size_t m_max_window_count;
		GLFW_Window* m_windows;
	};
}