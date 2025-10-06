module;

#include <macros/AurionExport.h>

#include <cstdint>

export module Aurion.FileSystem:Events;

import Aurion.Events;
import :FileHandle;

// TODO: Change FSFileHandle to be FSHandle.

export namespace Aurion
{
	typedef enum AURION_API FileEventTypes : uint16_t
	{
		AC_FILE_EVENT_NONE				= 0,
		AC_FILE_EVENT_OPEN				= 1 << 0,
		AC_FILE_EVENT_CLOSE				= 1 << 1,
		AC_FILE_EVENT_CREATE			= 1 << 2,
		AC_FILE_EVENT_DELETE			= 1 << 3,
		AC_FILE_EVENT_RENAME			= 1 << 4,
	} FileEventTypes;

	struct AURION_API FileEvent : public EventBase
	{
		FileEvent() { this->category = AC_EVENT_CATEGORY_FILE; };
		FileEvent(const FileEventTypes& type) : FileEvent() { this->type = type; };
		
		uint64_t handle = 0; // OS handle of the file or directory that triggered the event
		const char* path = nullptr; // Path of the file or directory that triggered the event
		bool is_directory = false; // True if the event is for a directory, false if it's for a file
	};

	struct AURION_API FileOpenEvent : public FileEvent
	{
		FileOpenEvent() : FileEvent(AC_FILE_EVENT_OPEN) {};
	
		FSFileHandle* out_handle = nullptr;
	};

	struct AURION_API FileCloseEvent : public FileEvent
	{
		FileCloseEvent() : FileEvent(AC_FILE_EVENT_CLOSE) {};
	};

	struct AURION_API FileCreateEvent : public FileEvent
	{
		FileCreateEvent() : FileEvent(AC_FILE_EVENT_CREATE) {};

		FSFileHandle* out_handle = nullptr;
	};

	struct AURION_API FileDeleteEvent : public FileEvent
	{
		FileDeleteEvent() : FileEvent(AC_FILE_EVENT_DELETE) {};
	};

	struct AURION_API FileRenameEvent : public FileEvent
	{
		FileRenameEvent() : FileEvent(AC_FILE_EVENT_RENAME) {};

		const char* new_path = nullptr; // Path of the file or directory
	};
}