module;

#include <macros/AurionExport.h>

#include <cstdint>

export module Aurion.Window:Window;

import Aurion.Input;

export namespace Aurion
{
	// Managed Window Properties
	struct AURION_API WindowProperties
	{
		const char* title = nullptr;
		uint16_t width = 1280;
		uint16_t height = 720;
		uint16_t x = 0;
		uint16_t y = 0;
		bool resizable = true;
		bool minimized = false;
		bool maximized = false;
		bool decorated = true;
		bool focused = true;
		bool fullscreen = true;
	};

	// Core interface declaration
	class AURION_API IWindow
	{
	public:
		virtual ~IWindow() = default;

		// Lifecycle Methods
		virtual void Update(float deltaTime = 0) = 0;

		// Property Access
		virtual void* GetNativeHandle() = 0;
		virtual const WindowProperties& GetProperties() = 0;

		virtual bool IsOpen() = 0;
		virtual bool IsFullscreen(const bool& exclusive = false) = 0;

		// Interactive/Configuration Methods
		virtual void ToggleDecoration() = 0;
		virtual void SetTitle(const char* title) = 0;
		virtual void Resize(const uint16_t& width, const uint16_t& height) = 0;
		virtual void MoveTo(const uint16_t& x, const uint16_t& y) = 0;
		virtual void ToggleFocus() = 0;
		virtual void ToggleMinimize() = 0;
		virtual void ToggleMaximize() = 0;
	};

	// Window utility wrapper class
	class AURION_API Window : public IWindow
	{
	public:
		Window();
		Window(const WindowProperties& properties);
		virtual ~Window() override = default;

		// Update Loop
		virtual void Update(float deltaTime) override = 0;

		virtual void* GetNativeHandle() override final;
		virtual const WindowProperties& GetProperties() override final;

		virtual bool IsOpen() override final;
		virtual bool IsFullscreen(const bool& exclusive) override final;

		virtual void ToggleDecoration() override;
		virtual void SetTitle(const char* title) override;
		virtual void Resize(const uint16_t& width, const uint16_t& height) override;
		virtual void MoveTo(const uint16_t& x, const uint16_t& y) override;
		virtual void ToggleFocus() override;
		virtual void ToggleMinimize() override;
		virtual void ToggleMaximize() override;

	protected:
		void* m_native_handle;
		WindowProperties m_properties;
	};
}