#include <macros/AurionLog.h>

#include <cassert>

import Aurion.Application;

namespace Aurion 
{
	/// -----------------------------------------------------------------
	/// ------------------------ Static Methods -------------------------
	/// -----------------------------------------------------------------

	void Application::BroadcastEvent(const EventBase* event)
	{
		if (!s_instance)
		{
			AURION_ERROR("Application instance is null. Cannot broadcast event.");
			return;
		}

		// Forward the event to the application instance for handling.
		s_instance->OnEvent(event);
	}

	/// -----------------------------------------------------------------
	/// ---------------------- Instance Methods -------------------------
	/// -----------------------------------------------------------------

	Application::Application()
	{
		// Ensure only one instance of Application exists at any time.
		assert(s_instance == nullptr && "Application instance already exists!");

		s_instance = this;
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

	void Application::OnEvent(const EventBase* event)
	{
		m_event_bus.Dispatch(event);
	}
}