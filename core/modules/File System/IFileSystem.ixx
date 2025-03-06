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

		// File Ops

		virtual FSHandle GetFile(const char* path) = 0;

		virtual FSAttributes GetFileAttributes(const char* path) = 0;

		virtual FSInfo GetFileInfo(const char* path) = 0;

		virtual uint64_t GetFileSize(const char* path) = 0;

		virtual void* ReadFile(const char* path) = 0;

		virtual bool WriteFile(const char* path, void* buffer) = 0;

		// Directory Ops

		virtual FSHandle GetDirectory(const char* path) = 0;

		virtual FSAttributes GetDirAttributes(const char* path) = 0;

		virtual FSInfo GetDirectoryInfo(const char* path) = 0;

		virtual uint64_t GetDirSize(const char* path) = 0;
	};
}