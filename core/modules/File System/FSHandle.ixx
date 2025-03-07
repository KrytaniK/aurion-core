module;

#include <macros/AurionExport.h>
#include <cstdint>

export module Aurion.FileSystem:Handle;

import :Info;

export namespace Aurion
{
	typedef AURION_API uint64_t FSHandle;
}