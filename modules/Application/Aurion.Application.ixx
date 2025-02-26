module;

#include <string>
#include <memory>

export module Aurion.Application;

export namespace Aurion
{
	class IApplication
	{
	public:
		virtual ~IApplication() = default;

		virtual void StartAndRun() = 0;
	};

	// Overridden by CLIENT implementations.
	std::unique_ptr<IApplication> CreateApplication();
}