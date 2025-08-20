module;

#include <macros/AurionExport.h>

export module Aurion.Application;

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

	class AURION_API ApplicationBase : public IApplication
	{
	public:
		ApplicationBase();
		virtual ~ApplicationBase() override;

		virtual void StartAndRun() override final;

	private:
		virtual void Initialize() override;
		virtual void Run() override;
		virtual void Shutdown() override;

	protected:
		bool m_shouldClose = true;
	};
}