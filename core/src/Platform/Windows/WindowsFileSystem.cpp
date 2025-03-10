
#include <cstdint>
#include <utility>
#include <iostream>

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

	uint64_t WindowsFileSystem::GenerateHandle(const char* path, const bool& force_create)
	{
		DWORD access = IsFilePath(path) ? GENERIC_READ | GENERIC_WRITE : GENERIC_READ;
		DWORD attributes = IsFilePath(path) ? FILE_ATTRIBUTE_NORMAL : FILE_ATTRIBUTE_DIRECTORY;

		// If user requested the file not be force created
		if (!force_create)
			return (uint64_t)CreateFileA(path, access, 0, NULL, OPEN_EXISTING, attributes, NULL);

		// Otherwise, split the path into chunks, and create any required directories
		size_t path_length = strlen(path);
		size_t chunk_end = 0;
		while (chunk_end < path_length)
		{
			// If we've found a valid delimiter
			if (chunk_end > 0 && path[chunk_end] == '/' && path[chunk_end - 1] != '.')
			{
				// Allocate a temporary buffer
				char* buffer = new char[chunk_end + 1];
				buffer[chunk_end] = '\0';

				// Copy the string contents into the buffer
				//strncpy(buffer, path, chunk_end);
				strncpy_s(buffer, chunk_end + 1, path, chunk_end);

				// If the directory doesn't exist, attempt to create it. Bail if this
				//	attempt fails.
				if (!DirectoryExists(buffer) && !CreateDirectoryA(buffer, NULL))
					return (uint64_t)(INVALID_HANDLE_VALUE);

				// Free the temporary buffer
				delete[] buffer;
			}

			chunk_end++;
		}

		// Only create the file once all parent directories exist
		return (uint64_t)CreateFileA(path, access, 0, NULL, CREATE_ALWAYS, attributes, NULL);
	}

	FSFileHandle WindowsFileSystem::OpenFile(const char* path, const bool& force_create)
	{
		return FSFileHandle(this, path, force_create);
	}

	bool WindowsFileSystem::GetAllFiles(const char* path, FSFileHandle*& out_files, size_t& out_count)
	{
		// Bail if the directory is invalid
		if (!DirectoryExists(path))
			return false;

		// Clear out any existing memory
		delete[] out_files;

		// Prep path for searching
		size_t path_length = strlen(path);
		char* search_dir = new char[path_length + 3];
		strncpy_s(search_dir, path_length + 3, path, path_length);
		search_dir[path_length] = '/';
		search_dir[path_length + 1] = '*';
		search_dir[path_length + 2] = '\0';

		// Open a find handle
		WIN32_FIND_DATAA data;
		HANDLE find_handle = FindFirstFileA(search_dir, &data);

		// Bail if the handle is invalid
		if (find_handle == INVALID_HANDLE_VALUE)
			return false;

		out_count = 0;
		while (FindNextFileA(find_handle, &data) != 0)
		{
			// Keep going if the current object is a directory
			if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				continue;

			out_count++;
		}

		// Create file handles
		out_files = new FSFileHandle[out_count];
		find_handle = FindFirstFileA(search_dir, &data);

		size_t created_count = 0;
		size_t padding = path[path_length - 1] == '/' ? 0 : 1;
		size_t file_path_length = 0;
		while (FindNextFileA(find_handle, &data))
		{
			// Keep going if the current object is a directory
			if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				continue;

			// Generate file path
			file_path_length = path_length + padding + strlen(data.cFileName) + 1;
			char* file_path = new char[file_path_length];
			strncpy_s(file_path, file_path_length, path, path_length);
			if (padding != 0)
				file_path[path_length] = '/';
			strncpy_s(file_path + path_length + padding, file_path_length - path_length - padding, data.cFileName, strlen(data.cFileName));
			file_path[file_path_length - 1] = '\0';

			//std::cout << "New File Path: " << file_path << std::endl;

			out_files[created_count] = FSFileHandle();
			out_files[created_count].Register(this, file_path, false);
			created_count++;
		}

		FindClose(find_handle);

		return true;
	}
	
	bool WindowsFileSystem::GetAllDirectories(const char* path, FSDirectoryHandle*& out_dirs, size_t& out_count)
	{
		// Bail if the directory is invalid
		if (!DirectoryExists(path))
			return false;

		// Clear out any existing memory
		delete[] out_dirs;

		// Prep path for searching
		size_t path_length = strlen(path);
		char* search_dir = new char[path_length + 3];
		strncpy_s(search_dir, path_length + 3, path, path_length);
		search_dir[path_length] = '/';
		search_dir[path_length + 1] = '*';
		search_dir[path_length + 2] = '\0';

		// Open a find handle
		WIN32_FIND_DATAA data;
		HANDLE find_handle = FindFirstFileA(search_dir, &data);

		// Bail if the handle is invalid
		if (find_handle == INVALID_HANDLE_VALUE)
			return false;

		out_count = 0;

		while (FindNextFileA(find_handle, &data) != 0)
		{
			// Keep going if the current object is a directory
			if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				continue;

			// Don't attempt to go back a directory
			if (data.cFileName[0] == '.' && data.cFileName[1] == '.')
				continue;

			out_count++;
		}

		// Create file handles
		out_dirs = new FSDirectoryHandle[out_count];
		find_handle = FindFirstFileA(search_dir, &data);

		size_t created_count = 0;
		size_t padding = path[path_length - 1] == '/' ? 0 : 1;
		size_t file_path_length = 0;
		while (FindNextFileA(find_handle, &data))
		{
			// Keep going if the current object is a directory
			if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				continue;

			// Don't attempt to go back a directory
			if (data.cFileName[0] == '.' && data.cFileName[1] == '.')
				continue;

			// Generate file path
			file_path_length = path_length + padding + strlen(data.cFileName) + 1;
			char* file_path = new char[file_path_length];
			strncpy_s(file_path, file_path_length, path, path_length);
			if (padding != 0)
				file_path[path_length] = '/';
			strncpy_s(file_path + path_length + padding, file_path_length - path_length - padding, data.cFileName, strlen(data.cFileName));
			file_path[file_path_length - 1] = '\0';

			out_dirs[created_count] = FSDirectoryHandle();
			out_dirs[created_count].Register(this, file_path, false);
			created_count++;
		}

		FindClose(find_handle);

		return true;
	}

	bool WindowsFileSystem::CloseFile(const uint64_t& handle)
	{
		return CloseHandle((HANDLE)handle);
	}

	void WindowsFileSystem::GetFileInfo(const char* path, FSFileInfo& out_info, const bool& force_close)
	{
		HANDLE system_handle = (HANDLE)(this->GenerateHandle(path, false));

		if (system_handle == INVALID_HANDLE_VALUE)
			return;

		// Deduce the file name and extension
		size_t path_length = strlen(path);
		size_t chunk_start = path_length;

		// Walk backwards until reaching the first slash
		while (chunk_start > 0 && path[--chunk_start] != '/') {}

		// Walk forward until the first '.'
		size_t chunk_end = chunk_start;
		while (chunk_end < path_length && path[chunk_end++] != '.') {}

		// Create name buffer
		size_t buffer_size = chunk_end - chunk_start;
		out_info.name = new char[buffer_size + 1];
		out_info.name[buffer_size] = '\0';

		// Copy file name
		strncpy_s(out_info.name, buffer_size + 1, path + chunk_start, buffer_size);

		// Create the extension buffer
		buffer_size = path_length - chunk_end;
		out_info.extension = new char[buffer_size + 1];
		out_info.extension[buffer_size] = '\0';

		// Copy the extension into the buffer
		strncpy_s(out_info.extension, buffer_size + 1, path + chunk_end, buffer_size);

		// Get system object information and convert 32-bit parts into 64-bit unsigned integers
		BY_HANDLE_FILE_INFORMATION handle_info;
		GetFileInformationByHandle(system_handle, &handle_info);
		out_info.creation_time = ULARGE_INTEGER{ handle_info.ftCreationTime.dwLowDateTime, handle_info.ftCreationTime.dwHighDateTime }.QuadPart;
		out_info.last_accessed_time = ULARGE_INTEGER{ handle_info.ftLastAccessTime.dwLowDateTime, handle_info.ftLastAccessTime.dwHighDateTime }.QuadPart;
		out_info.last_modified_time = ULARGE_INTEGER{ handle_info.ftLastWriteTime.dwLowDateTime, handle_info.ftLastWriteTime.dwHighDateTime }.QuadPart;

		if (force_close)
			CloseHandle(system_handle);
	}

	void WindowsFileSystem::GetFileInfo(const char* path, FSFileInfo& out_info, const uint64_t& handle, const bool& force_close)
	{
		HANDLE system_handle = (HANDLE)handle;

		// Deduce the file name and extension
		size_t path_length = strlen(path);
		size_t chunk_start = path_length;

		// Walk backwards until reaching the first slash
		while (chunk_start > 0 && path[--chunk_start - 1] != '/') {}

		// Walk forward until the first '.'
		size_t chunk_end = chunk_start;
		while (chunk_end < path_length && path[chunk_end++ + 1] != '.') {}

		// Create name buffer
		size_t buffer_size = chunk_end - chunk_start;
		out_info.name = new char[buffer_size + 1];
		out_info.name[buffer_size] = '\0';

		// Copy file name
		strncpy_s(out_info.name, buffer_size + 1, path + chunk_start, buffer_size);

		// Create the extension buffer
		buffer_size = path_length - chunk_end;
		out_info.extension = new char[buffer_size + 1];
		out_info.extension[buffer_size] = '\0';

		// Copy the extension into the buffer
		strncpy_s(out_info.extension, buffer_size + 1, path + chunk_end, buffer_size);

		// Get system object information and convert 32-bit parts into 64-bit unsigned integers
		BY_HANDLE_FILE_INFORMATION handle_info;
		GetFileInformationByHandle(system_handle, &handle_info);
		out_info.creation_time = ULARGE_INTEGER{ handle_info.ftCreationTime.dwLowDateTime, handle_info.ftCreationTime.dwHighDateTime }.QuadPart;
		out_info.last_accessed_time = ULARGE_INTEGER{ handle_info.ftLastAccessTime.dwLowDateTime, handle_info.ftLastAccessTime.dwHighDateTime }.QuadPart;
		out_info.last_modified_time = ULARGE_INTEGER{ handle_info.ftLastWriteTime.dwLowDateTime, handle_info.ftLastWriteTime.dwHighDateTime }.QuadPart;

		if (force_close)
			CloseHandle(system_handle);
	}

	void WindowsFileSystem::Read(const char* path, FSFileData* out_data)
	{
		HANDLE system_handle = CreateFileA(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (system_handle == INVALID_HANDLE_VALUE)
			return;

		// Get file size
		LARGE_INTEGER file_size;
		if (!GetFileSizeEx(system_handle, &file_size))
			return;

		// Convert file size to 64-bit uinteger
		uint64_t size = file_size.QuadPart;

		// Allocate enough space for the data
		if (!out_data->Allocate((size_t)size))
			return;

		// Read file in chunks
		DWORD bytes_read = 0;
		DWORD total_bytes_read = 0;
		while (total_bytes_read < size)
		{
			DWORD to_read = (size - total_bytes_read > MAXDWORD) ? MAXDWORD : (DWORD)(size - total_bytes_read);
			if (!ReadFile(system_handle, ((char*)out_data->Get()) + total_bytes_read, to_read, &bytes_read, NULL) || bytes_read == 0)
				return;

			total_bytes_read += bytes_read;
		}

		// Add null terminator
		((char*)out_data->Get())[size] = '\0';
	}

	void WindowsFileSystem::Read(const uint64_t& handle, FSFileData* out_data)
	{
		// Create handle
		HANDLE system_handle = (HANDLE)handle;

		if (system_handle == INVALID_HANDLE_VALUE)
			return;

		// Get updated file size
		LARGE_INTEGER file_size;
		if (!GetFileSizeEx(system_handle, &file_size))
			return;

		// Convert file size to 64-bit uinteger
		uint64_t size = file_size.QuadPart + 1;

		// Allocate enough space for the data
		if (!out_data->Allocate((size_t)size))
			return;

		// Read file in chunks
		DWORD bytes_read = 0;
		DWORD total_bytes_read = 0;
		while (total_bytes_read < size)
		{
			DWORD to_read = (size - total_bytes_read > MAXDWORD) ? MAXDWORD : (DWORD)(size - total_bytes_read);
			if (!ReadFile(system_handle, ((char*)out_data->Get()) + total_bytes_read, to_read, &bytes_read, NULL) || bytes_read == 0)
				return;

			total_bytes_read += bytes_read;
		}

		// Add null terminator
		((char*)out_data->Get())[size] = '\0';
	}

	bool WindowsFileSystem::Write(const char* path, void* buffer, const size_t& size, const size_t& offset)
	{
		DWORD creation_mode = offset == 0 ? CREATE_ALWAYS : OPEN_ALWAYS;
		DWORD access_mode = offset == 0 ? GENERIC_WRITE : FILE_APPEND_DATA | GENERIC_WRITE;

		// Grab file handle
		HANDLE handle = CreateFileA(path, access_mode, 0, NULL, creation_mode, FILE_ATTRIBUTE_NORMAL, NULL);

		// Validate handle
		if (handle == INVALID_HANDLE_VALUE)
			return false;

		// Adjust file pointer
		if (offset != -1)
		{
			LARGE_INTEGER ptr_offset{};
			ptr_offset.QuadPart = (ULONGLONG)offset;
			SetFilePointer(handle, ptr_offset.LowPart, &ptr_offset.HighPart, FILE_BEGIN);
		}
		else
		{
			SetFilePointer(handle, 1, 0, FILE_END);
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

	bool WindowsFileSystem::Write(const uint64_t& handle, void* buffer, const size_t& size, const size_t& offset)
	{
		// Grab file handle
		HANDLE system_handle = (HANDLE)handle;

		// Validate handle
		if (system_handle == INVALID_HANDLE_VALUE)
			return false;

		// Adjust file pointer
		if (offset != -1)
		{
			LARGE_INTEGER ptr_offset{};
			ptr_offset.QuadPart = (ULONGLONG)offset;
			SetFilePointer(system_handle, ptr_offset.LowPart, &ptr_offset.HighPart, FILE_BEGIN);
		}
		else
		{
			SetFilePointer(system_handle, 0, 0, FILE_END);
		}
		

		uint8_t* data = (uint8_t*)buffer;
		uint64_t remaining = size;
		DWORD bytes_written = 0;

		// Write in chunks for large files
		while (remaining > 0)
		{
			DWORD chunk_size = (remaining > MAXDWORD) ? MAXDWORD : static_cast<DWORD>(remaining);

			if (!WriteFile(system_handle, data, chunk_size, &bytes_written, NULL) || bytes_written != chunk_size)
			{
				// Reset file pointer to the beginning of the file
				SetFilePointer(system_handle, 0, NULL, FILE_BEGIN);
				return false;
			}

			data += chunk_size;
			remaining -= chunk_size;
		}

		// Reset file pointer to the end of the file
		SetFilePointer(system_handle, 0, NULL, FILE_END);
		return true;
	}

	bool WindowsFileSystem::DirectoryExists(const char* path)
	{
		DWORD attr = GetFileAttributesA(path);

		return (attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool WindowsFileSystem::FileExists(const char* path)
	{
		return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
	}

	bool WindowsFileSystem::IsFilePath(const char* path)
	{
		return path[strlen(path) - 1] != '/';
	}

	bool WindowsFileSystem::IsDirPath(const char* path)
	{
		return path[strlen(path) - 1] == '/';
	}

}