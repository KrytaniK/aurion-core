module;

#include <../macros/AurionExport.h>

export module Aurion.WindowPlugin;

import Aurion.Plugin;

export namespace Aurion
{
	class AURION_API WindowPlugin : public IPlugin
	{
	public:
		WindowPlugin();
		virtual ~WindowPlugin() override final;

		virtual void Initialize(IPluginContext* context) override final;
	};

	extern "C" AURION_API IPlugin* CreatePlugin();
	extern "C" AURION_API void DestroyPlugin(const IPlugin* plugin);
}