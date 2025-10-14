module Aurion.Application;

namespace Aurion 
{
	// Starts the application and runs it until m_shouldClose is set to true.
	void Application::StartAndRun(int argc, char* argv[])
	{
		this->Initialize(argc, argv);
		this->Run();
		this->Shutdown();
	}
}