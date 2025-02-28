export module Aurion.Plugin:Context;

export namespace Aurion
{
	// Plugin Context is aimed to be used to allow plugins to 
	// 'hook' into application functionality based on application
	// needs.
	class IPluginContext
	{
	public:
		virtual ~IPluginContext() = default;
	};
}