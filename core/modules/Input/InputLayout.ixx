module;

#include <macros/AurionExport.h>

#include <cstdint>

export module Aurion.Input:Layout;

import :Control;

export namespace Aurion
{
	// An input layout represents the memory layout of a device's controls (such as the buttons on a keyboard, or the scroll direction of a mouse)
	struct AURION_API InputLayout
	{
		uint64_t id; // ID for this layout
		uint16_t control_count; // The number of controls
		InputControlInfo* control_infos; // All control information for this layout
	};
}