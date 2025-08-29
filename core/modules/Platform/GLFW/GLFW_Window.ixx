module;

#include <macros/AurionExport.h>

#include <GLFW/glfw3.h>

export module Aurion.GLFW:Window;

import :InputContext;

import Aurion.Window;

export namespace Aurion
{
	class AURION_API GLFW_Window : public Window
	{
	public:
		GLFW_Window();
		GLFW_Window(const WindowProperties& properties);
		virtual ~GLFW_Window() override;

		virtual void Update(float deltaTime) override;
		
		virtual void ToggleDecoration() override;
		virtual void SetTitle(const char* title) override;
		virtual void SetMode(const WindowMode& mode) override;
		virtual void Resize(const uint16_t& width, const uint16_t& height) override;
		virtual void MoveTo(const uint16_t& x, const uint16_t& y) override;
		virtual void Focus() override;
		virtual void ToggleMinimize() override;
		virtual void ToggleMaximize() override;

	private:
		void CreateNativeWindow();

	protected:
		GLFWmonitor* m_native_monitor;
		WindowProperties m_cached_properties;
	};
}