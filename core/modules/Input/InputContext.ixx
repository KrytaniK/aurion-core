module;

#include <macros/AurionExport.h>

#include <cstdint>

export module Aurion.Input:Context;

import :Device;
import :Layout;

export namespace Aurion
{
	class AURION_API IInputContext
	{
	public:
		virtual ~IInputContext() = default;

		virtual const InputDevice& CreateDevice(const InputDeviceInfo& info, const uint16_t& layout_id) = 0;

		virtual bool AddDevice(const InputDevice& device) = 0;

		virtual void AddLayout(const InputLayout& layout) = 0;

		virtual bool IsKeyPressed(const uint16_t& key_code) = 0;
		

	};
}