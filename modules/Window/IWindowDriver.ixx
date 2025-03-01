module;

#include "../../macros/AurionExport.h"

#include <cstdint>

export module Aurion.Window:WindowDriver;

import :Window;

export namespace Aurion
{
	class AURION_API IWindowDriver
	{
	public:
		virtual ~IWindowDriver() = default;

		virtual uint16_t GetDriverType() = 0;

		virtual IWindow* GetWindow(const uint16_t& id) = 0;

		virtual IWindow* AddWindow(const WindowConfig& config) = 0;

		virtual bool RemoveWindow(const uint16_t& id) = 0;

		virtual bool RemoveWindow(const IWindow* window) = 0;
	};
}