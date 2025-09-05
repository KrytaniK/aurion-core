module;

#include <macros/AurionExport.h>

#include <cstdint>

export module Aurion.Window:Driver;

import :Window;

export namespace Aurion
{
	struct AURION_API WindowHandle
	{
		uint64_t id = 0;
		Window* window = nullptr;
	};

	class AURION_API IWindowDriver
	{
	public:
		virtual ~IWindowDriver() = default;

		virtual WindowHandle OpenWindow(const WindowProperties& properties) = 0;
		virtual bool CloseWindow(const WindowHandle& handle) = 0;
		virtual bool CloseWindow(const char* title) = 0;
		virtual bool CloseWindow(const uint64_t& id) = 0;

		virtual WindowHandle GetWindow(const char* title) = 0;
		virtual WindowHandle GetWindow(const uint64_t& id) = 0;
	};
}