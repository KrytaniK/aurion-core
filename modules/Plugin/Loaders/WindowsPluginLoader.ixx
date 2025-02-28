export module Aurion.Plugin:WindowsLoader;

#ifdef AURION_PLATFORM_WINDOWS

import <Windows.h>;

import :Loader;

export namespace Aurion
{
	class WindowsPluginLoader : public IPluginLoader
	{
	public:
		WindowsPluginLoader(const size_t& plugin_count);
		virtual ~WindowsPluginLoader() override final;

		// Inherited via IPluginLoader
		IPlugin* LoadPlugin(const char* source) override final;

		void UnloadPlugin(const IPlugin* plugin) override final;

	private:
		size_t m_max_plugin_count;
		size_t m_loaded_count;
		IPlugin** m_plugins;
		HMODULE* m_libs;
	};
}

#endif