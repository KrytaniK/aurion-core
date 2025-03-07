
#include <cstdint>
#include <utility>

#include <Windows.h>

import Aurion.FileSystem;

namespace Aurion
{
	WindowsFileSystem::WindowsFileSystem()
	{

	}

	WindowsFileSystem::~WindowsFileSystem()
	{

	}

	bool WindowsFileSystem::FileExists(const char* path)
	{
		return INVALID_FILE_ATTRIBUTES != GetFileAttributesA(path);
	}

	bool WindowsFileSystem::DirExists(const char* path)
	{
		DWORD attr = GetFileAttributesA(path);
		return (INVALID_FILE_ATTRIBUTES  != attr && (attr & FILE_ATTRIBUTE_DIRECTORY));
	}

	FSHandle WindowsFileSystem::OpenFile(const char* path, const FSAccess& access)
	{
		DWORD file_access = (access == FS_ACCESS_WRITE_ONLY) ? GENERIC_WRITE : GENERIC_READ;

		return (FSHandle)CreateFileA(path, file_access, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	void WindowsFileSystem::CloseFile(const FSHandle& handle)
	{
		CloseHandle((HANDLE)handle);
	}

	FSInfo WindowsFileSystem::GetFileInfo(const char* path)
	{
		FSHandle handle = OpenFile(path, FS_ACCESS_READ_ONLY);

		if ((HANDLE)handle == INVALID_HANDLE_VALUE)
		{
			CloseFile(handle);
			return FSInfo{};
		}

		// Extract the extension
		const char* extension = nullptr;
		size_t len = strlen(path);
		for (size_t i = len - 1; i > 0; i++)
		{
			if (path[i] == '.')
			{
				extension = path + (i - 1);
			}
		}

		// Get system object information
		FSInfo info{};
		BY_HANDLE_FILE_INFORMATION handle_info;
		GetFileInformationByHandle((HANDLE)handle, &handle_info);
		info.size = ULARGE_INTEGER{ handle_info.nFileSizeLow, handle_info.nFileSizeHigh }.QuadPart;
		info.creation_time = ULARGE_INTEGER{ handle_info.ftCreationTime.dwLowDateTime, handle_info.ftCreationTime.dwHighDateTime }.QuadPart;
		info.last_accessed_time = ULARGE_INTEGER{ handle_info.ftLastAccessTime.dwLowDateTime, handle_info.ftLastAccessTime.dwHighDateTime }.QuadPart;
		info.last_modified_time = ULARGE_INTEGER{ handle_info.ftLastWriteTime.dwLowDateTime, handle_info.ftLastWriteTime.dwHighDateTime }.QuadPart;
		info.extension = extension;

		CloseFile(handle);

		return info;
	}

	FSInfo WindowsFileSystem::GetDirInfo(const char* path)
	{
		HANDLE handle = CreateFileA(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_DIRECTORY, NULL);

		if (handle == INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
			return FSInfo{};
		}

		// Get system object information
		FSInfo info{};
		BY_HANDLE_FILE_INFORMATION handle_info;
		GetFileInformationByHandle(handle, &handle_info);
		info.size = ULARGE_INTEGER{ handle_info.nFileSizeLow, handle_info.nFileSizeHigh }.QuadPart;
		info.creation_time = ULARGE_INTEGER{ handle_info.ftCreationTime.dwLowDateTime, handle_info.ftCreationTime.dwHighDateTime }.QuadPart;

		CloseHandle(handle);

		return info;
	}

	uint64_t WindowsFileSystem::GetFileSize(const char* path)
	{
		// Create handle
		HANDLE handle = CreateFileA(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		// Get file size
		LARGE_INTEGER file_size;
		if (!GetFileSizeEx(handle, &file_size))
		{
			CloseHandle(handle);
			return 0;
		}

		// Close handle
		CloseHandle(handle);

		// Return size
		return file_size.QuadPart;
	}

	uint64_t WindowsFileSystem::GetDirSize(const char* path)
	{
		// Create handle
		HANDLE handle = CreateFileA(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_DIRECTORY, NULL);

		// Get file size
		LARGE_INTEGER file_size;
		if (!GetFileSizeEx(handle, &file_size))
		{
			CloseHandle(handle);
			return 0;
		}

		// Close handle
		CloseHandle(handle);

		// Return size
		return file_size.QuadPart;
	}

	FSFileData WindowsFileSystem::Read(const char* path)
	{
		// Create handle
		HANDLE handle = CreateFileA(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (handle == INVALID_HANDLE_VALUE)
			return FSFileData(0);

		// Get file size
		LARGE_INTEGER file_size;
		if (!GetFileSizeEx(handle, &file_size))
		{
			CloseHandle(handle);
			return FSFileData(0);
		}

		// Convert file size to 64-bit uinteger
		uint64_t size = file_size.QuadPart;

		// Read file in chunks
		FSFileData file_data(size + 1); // Allocate full file size + 1 byte for null termination character
		DWORD bytes_read = 0;
		DWORD total_bytes_read = 0;
		while (total_bytes_read < size)
		{
			DWORD to_read = (size - total_bytes_read > MAXDWORD) ? MAXDWORD : (DWORD)(size - total_bytes_read);
			if (!ReadFile(handle, ((char*)file_data.Get()) + total_bytes_read, to_read, &bytes_read, NULL) || bytes_read == 0)
			{
				CloseHandle(handle);
				return FSFileData(0);
			}

			total_bytes_read += bytes_read;
		}

		// Close file handle
		CloseHandle(handle);

		// Add null terminator
		((char*)file_data.Get())[file_size.QuadPart] = '\0';

		// Move file data
		return std::move(file_data);
	}

	bool WindowsFileSystem::Write(const char* path, void* buffer, const uint64_t& size, const bool& overwrite)
	{
		DWORD creation_mode = overwrite ? CREATE_ALWAYS : OPEN_ALWAYS;
		DWORD access_mode = overwrite ? GENERIC_WRITE : FILE_APPEND_DATA | GENERIC_WRITE;

		HANDLE handle = CreateFileA(
			path,
			access_mode,
			0,
			NULL,
			creation_mode,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (handle == INVALID_HANDLE_VALUE)
			return false;

		if (!overwrite)
		{
			SetFilePointer(handle, 0, NULL, FILE_END);
		}

		uint8_t* data = (uint8_t*)buffer;
		uint64_t remaining = size;
		DWORD bytes_written = 0;

		// Write in chunks for large files
		while (remaining > 0)
		{
			DWORD chunk_size = (remaining > MAXDWORD) ? MAXDWORD : static_cast<DWORD>(remaining);

			if (!WriteFile(handle, data, chunk_size, &bytes_written, NULL) || bytes_written != chunk_size)
			{
				CloseHandle(handle);
				return false; // Error occurred
			}

			data += chunk_size;
			remaining -= chunk_size;
		}

		CloseHandle(handle);
		return true;
	}

}