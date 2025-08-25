module;

#include <macros/AurionExport.h>

export module Aurion.Application;

import Aurion.Events;

export namespace Aurion
{
	// Defines an interface for an application, specifying lifecycle methods that must be implemented by derived classes.
	class AURION_API IApplication
	{
	public:
		virtual ~IApplication() = default;

		virtual void StartAndRun() = 0;

	private:
		virtual void Initialize() = 0;
		virtual void Run() = 0;
		virtual void Shutdown() = 0;
	};

	class AURION_API Application : public IApplication
	{
	public:
		inline static Application* s_instance = nullptr;

	public:
		inline static void BroadcastEvent(const EventBase* event);

	public:
		Application();
		virtual ~Application() override;

		virtual void StartAndRun() override final;

	private:
		virtual void Initialize() override;
		virtual void Run() override;
		virtual void Shutdown() override;

		virtual void OnEvent(const EventBase* event);

	protected:
		EventBus m_event_bus;
		bool m_shouldClose = true;
	};
}