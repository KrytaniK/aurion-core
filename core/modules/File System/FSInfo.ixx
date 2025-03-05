module;

#include <macros/AurionExport.h>
#include <cstdint>

export module Aurion.FileSystem:Info;

export namespace Aurion
{
	struct AURION_API FSInfo
	{
		uint64_t size;
		uint64_t creation_time;
		uint64_t last_modified_time;
		uint64_t last_accessed_time;
		const char* extension;
	};

	typedef enum FSAttributes
	{
		FILE_ATTRIBUTE_NORMAL = 0x00,		// Normal File, no attributes set
		FILE_ATTRIBUTE_READ_ONLY = 0x01,	// Read-Only file
		FILE_ATTRIBUTE_HIDDEN = 0x02,		// File is hidden (Windows only)
		FILE_ATTRIBUTE_DIRECTORY = 0x04,	// Directory tag
	} FSAttributes;
}