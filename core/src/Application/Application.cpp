module Aurion.Application;

import <macros/AurionLog.h>;
import <cassert>;

namespace Aurion 
{
	/// -----------------------------------------------------------------
	/// ------------------------ Static Methods -------------------------
	/// -----------------------------------------------------------------

	void Application::DispatchEvent(EventBase* event)
	{
		assert(s_instance != nullptr && "No valid application exists!");
		assert(event != nullptr && "Event pointer must be valid");

		// Forward the event to the application instance for handling.
		s_instance->OnEvent(event);
	}

	EventBus* Application::Events()
	{
		assert(s_instance != nullptr && "No valid application exists!");

		return &s_instance->m_event_bus;
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

	void Application::OnEvent(EventBase* event)
	{
		m_event_bus.Dispatch(event);
	}
}