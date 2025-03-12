#include <macros/AurionExport.h>
#include <macros/AurionLog.h>

import Aurion.WindowPlugin;
import Aurion.Window;
import Aurion.GLFW;

namespace Aurion
{
	WindowPlugin::WindowPlugin()
	{
		
	}

	WindowPlugin::~WindowPlugin()
	{
		
	}

	void WindowPlugin::Initialize(IPluginContext* context)
	{
		AURION_INFO("Initializing Window Plugin!");

		// Bail if the context is invalid
		if (!context)
			return;

		// Cast the context to the relevant type (Expects WindowContext)
		WindowContext* winCtx = static_cast<WindowContext*>(context);

		// Set window driver
		winCtx->SetDriver(&m_window_driver);
	}

	AURION_API IPlugin* CreatePlugin()
	{
		AURION_INFO("Creating Window Plugin");
		return new WindowPlugin();
	}

	AURION_API void DestroyPlugin(const IPlugin* plugin)
	{
		AURION_INFO("Destroying Window Plugin");
		return;
	}
}