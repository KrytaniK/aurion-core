module Aurion.FileSystem;

namespace Aurion
{
	FSFileHandle::FSFileHandle() 
		: m_file_system(nullptr), m_path(nullptr), m_file_data(new FSFileData()), m_system_handle(-1), m_reference_count(0), m_info({})
	{

	}

	FSFileHandle::FSFileHandle(IFileSystem* fs, const char* path, const bool& force_create)
		: m_file_system(fs), m_path(path), m_file_data(new FSFileData()), m_system_handle(-1), m_reference_count(0), m_info({})
	{
		this->Register(fs, path, force_create);
	}

	FSFileHandle::~FSFileHandle()
	{
		// Manually free if no more external references exist
		if (m_reference_count == 0)
		{
			// Close the file handle if necessary
			if (m_file_system && m_system_handle != 0)
				m_file_system->CloseFile(m_system_handle);

			// Free path buffer
			if (m_path)
				delete[] m_path;

			// Free name buffer
			if (m_info.name)
				delete[] m_info.name;

			// Free extension buffer
			if (m_info.extension)
				delete[] m_info.extension;
			
			// Free allocated file data
			delete m_file_data;
		}
	}

	FSFileHandle::FSFileHandle(FSFileHandle&& other)
	{
		m_file_system = other.m_file_system;
		m_path = other.m_path;
		m_file_data = other.m_file_data;
		m_reference_count = other.m_reference_count;
		m_system_handle = other.m_system_handle;

		other.m_file_system = nullptr;
		other.m_path = nullptr;
		other.m_file_data = nullptr;
		other.m_system_handle = 0;
	}

	FSFileHandle& FSFileHandle::operator=(FSFileHandle&& other)
	{
		m_file_system = other.m_file_system;
		m_path = other.m_path;
		m_file_data = other.m_file_data;
		m_reference_count = other.m_reference_count;
		m_system_handle = other.m_system_handle;

		other.m_file_system = nullptr;
		other.m_path = nullptr;
		other.m_file_data = nullptr;
		other.m_system_handle = 0;

		return *this;
	}

	FSFileHandle::FSFileHandle(FSFileHandle& other)
	{
		m_file_system = other.m_file_system;
		m_path = other.m_path;
		m_file_data = other.m_file_data;
		m_reference_count = ++other.m_reference_count;
		m_system_handle = other.m_system_handle;
	}

	FSFileHandle& FSFileHandle::operator=(FSFileHandle& other)
	{
		m_file_system = other.m_file_system;
		m_path = other.m_path;
		m_file_data = other.m_file_data;
		m_reference_count = ++other.m_reference_count;
		m_system_handle = other.m_system_handle;

		return *this;
	}

	void FSFileHandle::Register(IFileSystem* fs, const char* path, const bool& force_create)
	{
		m_file_system = fs;
		m_path = path;

		// Get the system handle, force create the object if needed
		m_system_handle = m_file_system->GenerateHandle(m_path, force_create);

		// Get File Information
		m_file_system->GetFileInfo(m_path, m_info, m_system_handle, false);
	}

	void* FSFileHandle::Read()
	{
		// Can't read data if the file system reference is invalid.
		if (!m_file_system)
			return nullptr;

		// If the data is valid and hasn't changed, return it. (Automatically revalidates file info)
		if (m_file_data->Get())
		{
			uint64_t last_modified = m_info.last_modified_time;
			m_file_system->GetFileInfo(m_path, m_info, m_system_handle, false);
			if (last_modified == m_info.last_modified_time)
				return m_file_data->Get();
		}

		// Otherwise, file data needs to be revalidated
		delete m_file_data;
		m_file_data = new FSFileData();

		// Request OS file read and return the result.
		m_file_system->Read(m_system_handle, m_file_data);
		return m_file_data->Get();
	}

	bool FSFileHandle::Write(void* buffer, const size_t& size, const size_t& offset)
	{
		// Can't write data if the file system reference is invalid;
		if (!m_file_system || m_system_handle == (uint64_t)(-1))
			return false;

		// Trigger the write from the current path
		if (!m_file_system->Write(m_system_handle, buffer, size, offset))
			return false;

		// Revalidate the file data
		delete m_file_data; // delete old reference to avoid mem leak
		m_file_data = new FSFileData(); // allocate a new data object
		m_file_system->Read(m_system_handle, m_file_data); // Read the new contents

		// If the new data is valid, the write was a success
		return m_file_data->Size() != 0;
	}

	const size_t& FSFileHandle::GetSize()
	{
		return m_file_data->Size();
	}

	const FSFileInfo& FSFileHandle::GetInfo()
	{
		return m_info;
	}
}