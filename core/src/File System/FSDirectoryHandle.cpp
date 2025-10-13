module Aurion.FileSystem;

import <iostream>;
import <cstring>;

namespace Aurion
{
	FSDirectoryHandle::FSDirectoryHandle()
		: m_file_system(nullptr), m_info({}), m_files(nullptr), m_subdirs(nullptr)
	{

	}

	FSDirectoryHandle::FSDirectoryHandle(IFileSystem* fs, const char* path, const bool& force_create)
		: m_file_system(fs), m_info({}), m_files(nullptr), m_subdirs(nullptr)
	{
		this->Register(fs, path, force_create);
	}

	FSDirectoryHandle::~FSDirectoryHandle()
	{
		if (m_info.name)
			delete[] m_info.name;

		if (m_files)
			delete[] m_files;

		if (m_subdirs)
			delete[] m_subdirs;
	}

	void FSDirectoryHandle::Register(IFileSystem* fs, const char* path, const bool& force_create)
	{
		if (!fs || !path)
			return;

		m_file_system = fs;

		if (!m_file_system->DirectoryExists(path))
		{
			if (!force_create)
				return;

			m_system_handle = m_file_system->GenerateHandle(path, true);
		}
		else
			m_system_handle = m_file_system->GenerateHandle(path, false);

		// Extract directory name
		size_t length = strlen(path);
		size_t cpy_start = length - 1;

		// Walk backward until the delimiter is hit, or the start of the path
		while (cpy_start > 0 && path[cpy_start--] != '/');

		size_t cpy_size = length - cpy_start;

		// If the path is equal to the name, add another character for null terminator
		if (cpy_start == 0)
			cpy_size++;
		else
			cpy_start += 2;

		// Copy the name into the info structure
		m_info.name = new char[cpy_size];
		strncpy_s(m_info.name, cpy_size, path + cpy_start, cpy_size - 1);

		// Append null terminator
		m_info.name[cpy_size - 1] = '\0';

		// Retrieve all files and directories
		m_file_system->GetAllFiles(path, m_files, m_info.file_count);
		m_file_system->GetAllDirectories(path, m_subdirs, m_info.dir_count);
	}

	const FSDirectoryInfo& FSDirectoryHandle::GetInfo()
	{
		return m_info;
	}

	FSFileHandle* FSDirectoryHandle::FindFile(const char* file_name, const bool& recursive)
	{
		// Search all files in this directory
		const FSFileInfo* file_info = nullptr;
		for (size_t i = 0; i < m_info.file_count; i++)
		{
			file_info = &m_files[i].GetInfo();
			size_t size = strlen(file_info->name) + strlen(file_info->extension) + 1;
			char* full_name = new char[size];
			strncpy_s(full_name, size, file_info->name, _TRUNCATE);
			strncat_s(full_name, size, file_info->extension, _TRUNCATE);

			if (strcmp(file_name, full_name) == 0)
				return &m_files[i];

			delete[] full_name;
		}

		// Bail if we aren't searching child directories
		if (!recursive)
			return nullptr;

		// Otherwise, recursively search all child directories
		FSFileHandle* handle = nullptr;
		for (size_t i = 0; i < m_info.dir_count; i++)
			if (handle = m_subdirs[i].FindFile(file_name, recursive))
				return handle;

		return nullptr;
	}

	FSDirectoryHandle* FSDirectoryHandle::FindDirectory(const char* dir_name, const bool& recursive)
	{
		// Search all sub-directories of this directory
		FSDirectoryHandle* handle = nullptr;
		for (size_t i = 0; i < m_info.dir_count; i++)
		{
			handle = &m_subdirs[i];
			if (strcmp(dir_name, handle->GetInfo().name) == 0)
				return handle;

			if (recursive && (handle = handle->FindDirectory(dir_name, recursive)))
				return handle;
		}

		return nullptr;
	}

	FSFileCollection FSDirectoryHandle::FindAll(const char* extension, const bool& recursive)
	{
		FSFileCollection collection;
		collection.handles = nullptr;
		collection.count = 0;

		// If we're recursively searching
		if (recursive)
		{
			size_t created = 0;
			// First find out how many exist
			FindAll_Recursive(extension, collection, created);

			// Bail if none were found
			if (collection.count == 0)
				return collection;

			// Allocate the required memory
			collection.handles = new FSFileHandle[collection.count];

			// Then populate the array
			FindAll_Recursive(extension, collection, created);

			// and return the result
			return collection;
		}
		
		// Otherwise, figure out how many exist within this directory alone
		for (size_t i = 0; i < m_info.file_count; i++)
			if (strcmp(m_files[i].GetInfo().extension, extension) == 0)
				collection.count++;

		// Bail if none were found
		if (collection.count == 0)
			return collection;

		// Allocate
		collection.handles = new FSFileHandle[collection.count];

		// Then populate
		size_t created_count = 0;
		for (size_t i = 0; i < m_info.file_count; i++)
			if (m_files[i].GetInfo().extension == extension)
				collection.handles[created_count++] = m_files[i];

		return collection;
	}

	FSFileHandle FSDirectoryHandle::CreateFile(const char* relative_path)
	{
		return FSFileHandle();
	}

	FSDirectoryHandle* FSDirectoryHandle::CreateDirectory(const char* relative_path)
	{
		return nullptr;
	}

	void FSDirectoryHandle::FindAll_Recursive(const char* extension, FSFileCollection& out_collection, size_t& created_count)
	{
		// Figure out how many files exist in this directory with the extension
		for (size_t i = 0; i < m_info.file_count; i++)
		{
			if (strcmp(m_files[i].GetInfo().extension, extension) == 0)
			{
				// If an output array was specified, copy the handle into the array
				if (out_collection.handles != nullptr)
					out_collection.handles[created_count++] = m_files[i];
				else
					out_collection.count++; // Otherwise, simply report that one was found
			}
		}

		// Then, search/populate all subdirectories
		for (size_t i = 0; i < m_info.dir_count; i++)
			m_subdirs[i].FindAll_Recursive(extension, out_collection, created_count);
	}

}