module;

#include <macros/AurionExport.h>

#include <cstdint>
#include <Windows.h>

export module Aurion.FileSystem:WindowsImpl;

import :Interface;

#ifdef AURION_PLATFORM_WINDOWS

export namespace Aurion
{
	class AURION_API WindowsFileSystem : public IFileSystem
	{
	public:
		WindowsFileSystem();
		virtual ~WindowsFileSystem() override;

		bool FileExists(const char* path) override;

		bool DirExists(const char* path) override;

		virtual FSHandle OpenFile(const char* path, const FSAccess& access) override;

		virtual void CloseFile(const FSHandle& handle) override;

		virtual FSInfo GetFileInfo(const char* path) override;

		virtual FSInfo GetDirInfo(const char* path) override;

		virtual uint64_t GetFileSize(const char* path) override;

		virtual uint64_t GetDirSize(const char* path) override;

		FSFileData Read(const char* path) override;

		bool Write(const char* path, void* buffer, const uint64_t& size, const bool& overwrite) override;
	};
}

#endif