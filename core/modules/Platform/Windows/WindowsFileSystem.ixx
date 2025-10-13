export module Aurion.FileSystem:WindowsImpl;

import <macros/AurionExport.h>;

import Aurion.Types;
import :Interface;
import :DirectoryHandle;
import :FileHandle;
import :FileData;

#ifdef AURION_PLATFORM_WINDOWS

export namespace Aurion
{
	class AURION_API WindowsFileSystem : public IFileSystem
	{
	public:
		WindowsFileSystem();
		virtual ~WindowsFileSystem() override;

		virtual u64 GenerateHandle(const char* path, const bool& force_create) override;

		virtual FSFileHandle OpenFile(const char* path, const bool& force_create) override;

		virtual bool GetAllFiles(const char* path, FSFileHandle*& out_files, size_t& out_count) override;

		virtual bool GetAllDirectories(const char* path, FSDirectoryHandle*& out_dirs, size_t& out_count) override;

		virtual bool CloseFile(const u64& handle) override;

		virtual void GetFileInfo(const char* path, FSFileInfo& out_info, const bool& force_close = true) override;
		virtual void GetFileInfo(const char* path, FSFileInfo& out_info, const u64& handle, const bool& force_close = true) override;

		virtual void Read(const char* path, FSFileData* out_data) override;
		virtual void Read(const u64& handle, FSFileData* out_data) override;

		virtual bool Write(const char* path, void* buffer, const size_t& size, const size_t& offset) override;
		virtual bool Write(const u64& handle, void* buffer, const size_t& size, const size_t& offset) override;

		virtual bool DirectoryExists(const char* path) override;

		virtual bool FileExists(const char* path) override;

	private:

		virtual bool IsFilePath(const char* path) override;

		virtual bool IsDirPath(const char* path) override;
	};
}

#endif