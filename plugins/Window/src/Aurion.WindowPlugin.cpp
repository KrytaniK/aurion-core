#include "../macros/AurionExport.h"

#include <iostream>

import Aurion.WindowPlugin;

namespace Aurion
{
	WindowPlugin::WindowPlugin()
	{
		std::cout << "Hello World! From Window Plugin" << std::endl;
	}

	WindowPlugin::~WindowPlugin()
	{
		std::cout << "Goodbye World! From Window Plugin" << std::endl;
	}

	void WindowPlugin::Initialize(IPluginContext* context)
	{

	}

	AURION_API IPlugin* CreatePlugin()
	{
		std::cout << "[CreatePlugin] From Window Plugin" << std::endl;
		return new WindowPlugin();
	}

	AURION_API void DestroyPlugin(const IPlugin* plugin)
	{
		std::cout << "[DestroyPlugin] From Window Plugin" << std::endl;
		return;
	}
}