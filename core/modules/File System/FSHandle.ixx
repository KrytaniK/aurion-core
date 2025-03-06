module;

#include <macros/AurionExport.h>
#include <cstdint>

export module Aurion.FileSystem:Handle;

import :Info;

export namespace Aurion
{
	struct AURION_API FSHandle
	{
		uint64_t system_handle;
		FSInfo info;
		FSAttributes attributes;
	};
}