#include "../macros/AurionExport.h"

#include <iostream>

import Aurion.WindowPlugin;
import Aurion.Window;

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
		std::cout << "[Window Plugin] Initializing!" << std::endl;

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
		std::cout << "[CreatePlugin] Window Plugin" << std::endl;
		return new WindowPlugin();
	}

	AURION_API void DestroyPlugin(const IPlugin* plugin)
	{
		std::cout << "[DestroyPlugin] Window Plugin" << std::endl;
		return;
	}
}