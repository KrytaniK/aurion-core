#include <macros/AurionLog.h>

import Aurion.Application;

namespace Aurion 
{
	ApplicationBase::ApplicationBase()
	{
		// Default implementation does nothing.
	}

	ApplicationBase::~ApplicationBase()
	{
		// Default implementation does nothing.
	}

	// Starts the application and runs it until m_shouldClose is set to true.
	void ApplicationBase::StartAndRun()
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

	void ApplicationBase::Initialize()
	{
		// Default implementation does nothing.
	}

	void ApplicationBase::Run()
	{
		// Default implementation does nothing.
	}

	void ApplicationBase::Shutdown()
	{
		// Default implementation does nothing.
	}
}