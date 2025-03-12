module;

#include <macros/AurionExport.h>

export module Aurion.WindowPlugin;

import Aurion.Plugin;
import Aurion.GLFW;

export namespace Aurion
{
	class AURION_API WindowPlugin : public IPlugin
	{
	public:
		WindowPlugin();
		virtual ~WindowPlugin() override final;

		virtual void Initialize(IPluginContext* context) override final;

	private:
		GLFWDriver m_window_driver;
	};

	extern "C" AURION_API IPlugin* CreatePlugin();
	extern "C" AURION_API void DestroyPlugin(const IPlugin* plugin);
}