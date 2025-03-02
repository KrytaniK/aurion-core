import Aurion.Plugin;

namespace Aurion
{
	IPluginLoader* CreatePluginLoader(const size_t& plugin_count)
	{
	#ifdef AURION_PLATFORM_WINDOWS
		return new WindowsPluginLoader(plugin_count);
	#endif

		// Return null for unsupported platforms
		return nullptr;
	}
}