module;

#include "../../macros/AurionExport.h"
#include <cstdint>

export module Aurion.Window:Window;

export namespace Aurion
{
	struct AURION_API WindowConfig
	{
		const char* title = nullptr;
		uint16_t width = 1280;
		uint16_t height = 760;
		bool fullscreen = false;
		bool resizable = true;
		bool alwaysOnTop = false;
		bool borderless = false;
	};

	struct AURION_API WindowProperties
	{
		const char* title = nullptr;
		uint16_t width = 1280;
		uint16_t height = 760;
		uint16_t xPos = 0;
		uint16_t yPos = 0;
		bool minimized = false;
		bool focused = false;
		bool fullscreen = false;
		bool resizable = true;
		bool visible = true;
		bool alwaysOnTop = false;
		bool vsyncEnabled = true;
		bool borderless = false;
	};

	// Base interface for interacting with application 'windows'.
	class AURION_API IWindow
	{
	public:
		virtual ~IWindow() = default;

		virtual void SetTitle(const char* title) = 0;

		virtual uint16_t GetWidth() = 0;
		virtual uint16_t GetHeight() = 0;

		virtual void* GetNativeHandle() = 0;
		virtual const WindowProperties& GetProperties() const = 0;

		virtual void Update(float deltaTime = 0) = 0;

		virtual void SetFullscreen(bool fullscreen) = 0;
		virtual bool IsFullscreen() = 0;

		virtual void Resize(const uint16_t& width, const uint16_t& height) = 0;
		virtual void SetPos(const uint16_t& xPos, const uint16_t& yPos) = 0;

		virtual bool SetMinimized(bool minimize) = 0;
		virtual bool SetFocus(bool focused) = 0;
	};

	struct AURION_API WindowHandle
	{
		uint16_t id; 
		const IWindow* window;
	};
}