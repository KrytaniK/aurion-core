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

		virtual uint64_t GenerateHandle(const char* path, const bool& force_create) override;

		virtual FSFileHandle OpenFile(const char* path, const bool& force_create) override;

		virtual bool CloseFile(const uint64_t& handle) override;

		virtual void GetFileInfo(const char* path, FSFileInfo& out_info, const bool& force_close = true) override;
		virtual void GetFileInfo(const char* path, FSFileInfo& out_info, const uint64_t& handle, const bool& force_close = true) override;

		virtual void Read(const char* path, FSFileData* out_data) override;
		virtual void Read(const uint64_t& handle, FSFileData* out_data) override;

		virtual bool Write(const char* path, void* buffer, const size_t& size, const size_t& offset) override;
		virtual bool Write(const uint64_t& handle, void* buffer, const size_t& size, const size_t& offset) override;

		virtual bool DirectoryExists(const char* path) override;

		virtual bool FileExists(const char* path) override;

	private:

		virtual bool IsFilePath(const char* path) override;

		virtual bool IsDirPath(const char* path) override;
	};
}

#endif