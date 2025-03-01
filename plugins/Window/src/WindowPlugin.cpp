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