module;

#include <macros/AurionExport.h>

#include <cstdint>

export module Aurion.FileSystem:Interface;

import :Handle;
import :Info;

export namespace Aurion
{
	class AURION_API IFileSystem
	{
	public:
		virtual ~IFileSystem() = default;

		virtual bool FileExists(const char* path) = 0;

		virtual bool DirExists(const char* path) = 0;

		virtual FSHandle OpenFile(const char* path, const FSAccess& access) = 0;

		virtual void CloseFile(const FSHandle& handle) = 0;

		virtual FSInfo GetFileInfo(const char* path) = 0;

		virtual FSInfo GetDirInfo(const char* path) = 0;

		virtual uint64_t GetFileSize(const char* path) = 0;

		virtual uint64_t GetDirSize(const char* path) = 0;

		// File Ops

		virtual FSFileData Read(const char* path) = 0;

		virtual bool Write(const char* path, void* buffer, const uint64_t& size, const bool& overwrite) = 0;
	};
}