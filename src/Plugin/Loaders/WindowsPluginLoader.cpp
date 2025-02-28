#ifdef AURION_PLATFORM_WINDOWS

#include <Windows.h>;

import Aurion.Plugin;

namespace Aurion
{
	WindowsPluginLoader::WindowsPluginLoader(const size_t& plugin_count)
		: m_max_plugin_count(plugin_count), m_loaded_count(0)
	{
		// Validate plugin_count
		if (plugin_count == -1)
			return;

		// Allocate enough space for the plugins
		m_plugins = new IPlugin* [m_max_plugin_count];
		m_libs = new HMODULE[m_max_plugin_count];
	}

	WindowsPluginLoader::~WindowsPluginLoader()
	{
		// Unload all plugins before freeing memory
		for (size_t i = 0; i < m_max_plugin_count; i++)
			UnloadPlugin(m_plugins[i]);
		
		// Free allocated resources
		delete[] m_plugins;
		delete[] m_libs;
	}

	IPlugin* WindowsPluginLoader::LoadPlugin(const char* source)
	{
		// Can't load a plugin if we've reached the max allowance, or if the source is invalid
		if (source == nullptr || m_loaded_count == m_max_plugin_count)
			return nullptr;

		// Also can't load a plugin if the arrays haven't been created
		if (m_plugins == nullptr || m_libs == nullptr)
			return nullptr;

		// Find the first available plugin index. Since the number of loaded
		//	plugins is tracked, it's guaranteed that at least one slot will
		//	be available.
		size_t insert_index = 0;
		for (; insert_index < m_max_plugin_count; insert_index++)
			if (!m_plugins[insert_index])
				break;

		// Ensure an empty library slot is available for this plugin
		if (m_libs[insert_index] != NULL)
		{
			// Attempt to free the DLL Module (This CAN fail) and null the library entry
			FreeLibrary(m_libs[insert_index]);
			m_libs[insert_index] = NULL;
		}

		// Get a handle to the DLL module
		m_libs[insert_index] = LoadLibraryA(source);

		// If the handle isn't valid, unload and bail
		if (m_libs[insert_index] == NULL)
		{
			FreeLibrary(m_libs[insert_index]);
			return nullptr;
		}

		// Attempt to get the function address for CreatePlugin
		auto create_plugin_fun = (FnCreatePluginAlias) GetProcAddress(m_libs[insert_index], "CreatePlugin");

		// Unload and bail if the function can't be retrieved
		if (create_plugin_fun == NULL)
		{
			FreeLibrary(m_libs[insert_index]);
			return nullptr;
		}

		// Otherwise, call the function and store the plugin
		m_plugins[insert_index] = create_plugin_fun();

		// Ensure a valid plugin was created. If not, unload and bail
		if (m_plugins[insert_index] == nullptr)
		{
			FreeLibrary(m_libs[insert_index]);
			return nullptr;
		}

		// Increment loaded plugin count
		m_loaded_count++;

		// Return the stored plugin
		return m_plugins[insert_index];
	}

	void WindowsPluginLoader::UnloadPlugin(const IPlugin* plugin)
	{
		// Can't unload an invalid plugin, and can't unload if invalid arrays exist
		if (plugin == nullptr || m_plugins == nullptr || m_libs == nullptr)
			return;

		// Search for the plugin
		for (size_t i = 0; i < m_max_plugin_count; i++)
		{
			if (m_plugins[i] != plugin)
				continue;

			// Attempt to get the function address for DestroyPlugin
			auto destroy_plugin_fun = (FnDestroyPluginAlias)GetProcAddress(m_libs[i], "DestroyPlugin");

			// If the attempt succeeded, call the function
			if (destroy_plugin_fun != NULL)
				destroy_plugin_fun(plugin);
			else // Otherwise, manually call destructor to free plugin resources
				plugin->~IPlugin();

			// Decrement loaded plugin count
			m_loaded_count--;

			// Free the memory allocated for the plugin and null the pointer in the array
			delete plugin;
			m_plugins[i] = NULL;

			// Unload the DLL module and reset the library handle's state
			FreeLibrary(m_libs[i]);
			m_libs[i] = NULL;

			// Return, since no more work needs to be done
			return;
		}
	}
}

#endif