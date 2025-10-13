export module Aurion.Plugin:Context;

import <macros/AurionExport.h>;

export namespace Aurion
{
	// Plugin Context is aimed to be used to allow plugins to 
	// 'hook' into application functionality based on application
	// needs.
	class AURION_API IPluginContext
	{
	public:
		virtual ~IPluginContext() = default;
	};
}