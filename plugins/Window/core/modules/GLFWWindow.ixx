module;

#include <../macros/AurionExport.h>
#include <GLFW/glfw3.h>

export module Aurion.WindowPlugin:GLFWWindow;

import Aurion.Window;

export namespace Aurion
{
	class AURION_API GLFWWindow : public IWindow
	{
	public:
		GLFWWindow();
		virtual ~GLFWWindow() override;
		
		virtual void Open(const WindowConfig& config) override;

		virtual void Close() override;

		virtual void Update(float deltaTime = 0) override;

		virtual void SetTitle(const char* title) override;

		virtual void SetMode(const WindowMode& mode) override;

		virtual void Resize(const uint16_t& width, const uint16_t& height) override;

		virtual void SetPos(const uint16_t& xPos, const uint16_t& yPos) override;

		virtual bool Minimize() override;

		virtual bool Maximize() override;

		virtual bool Focus() override;

		virtual bool ToggleDecoration() override;

		virtual const char* GetTitle() override;

		virtual uint16_t GetWidth() override;

		virtual uint16_t GetHeight() override;

		virtual void* GetNativeHandle() override;

		virtual const WindowProperties& GetProperties() const override;

		virtual bool IsFullscreen() override;

	private:
		void CreateNativeWindow();
		void SetGLFWCallbacks();

	private:
		GLFWwindow* m_native_window;
		GLFWmonitor* m_native_monitor;
		WindowProperties m_state;
		WindowProperties m_state_cached;
	};
}