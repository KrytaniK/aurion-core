#include "../macros/AurionExport.h"

#include <iostream>
#include <GLFW/glfw3.h>

import Aurion.WindowPlugin;
import Aurion.Window;

namespace Aurion
{
	WindowPlugin::WindowPlugin()
	{
		if (!glfwInit())
		{
			std::cout << "[Window Plugin] GLFW Initialization Failed!" << std::endl;
		}
		else
			std::cout << "[Window Plugin] GLFW Initialization Succeeded!" << std::endl;

		// Create a test window
		GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
		if (!window)
		{
			std::cout << "[Window Plugin] GLFW Window Creation or OpenGL Context Creation Failed!" << std::endl;
		}
		else
		{
			std::cout << "[Window Plugin] Window Created Successfully!!!!" << std::endl;
			// Destroy Test window
			glfwDestroyWindow(window);
		}

		
	}

	WindowPlugin::~WindowPlugin()
	{
		glfwTerminate();
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