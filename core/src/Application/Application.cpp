#include <macros/AurionLog.h>

import Aurion.Application;

namespace Aurion 
{
	Application::Application()
	{
		// Default implementation does nothing.
	}

	Application::~Application()
	{
		// Default implementation does nothing.
	}

	// Starts the application and runs it until m_shouldClose is set to true.
	void Application::StartAndRun()
	{
		AURION_INFO("Application Initializing...");
		this->Initialize();

		AURION_INFO("Application Running...");
		while (!m_shouldClose)
		{
			this->Run();
		}

		AURION_INFO("Application Shutting Down...");
		this->Shutdown();
	}

	void Application::Initialize()
	{
		// Default implementation does nothing.
	}

	void Application::Run()
	{
		// Default implementation does nothing.
	}

	void Application::Shutdown()
	{
		// Default implementation does nothing.
	}
}