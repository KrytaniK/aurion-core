#include <Windows.h>
#include <iostream>

import Aurion.Application;
import Aurion.Plugin;

namespace Aurion
{
	class TestApp : public IApplication
	{
	public:
		TestApp() : m_plugin_loader(2)
		{
			m_plugin_loader.LoadPlugin("plugins/WindowPlugin.dll");
		};

		virtual ~TestApp() override final 
		{
		
		};

		// Inherited via IApplication
		void StartAndRun() override final
		{

		}

	private:
		WindowsPluginLoader m_plugin_loader;
	};
}

int main()
{
	Aurion::TestApp app;
	app.StartAndRun();
}