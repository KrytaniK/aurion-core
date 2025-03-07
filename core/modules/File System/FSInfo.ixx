module;

#include <macros/AurionExport.h>
#include <cstdint>
#include <cstdlib>
#include <utility>

export module Aurion.FileSystem:Info;

export namespace Aurion
{
	class AURION_API FSFileData
	{
	public:
		FSFileData(const size_t& size) : m_data(nullptr), m_size(size) { 
			if (m_size == 0)
				return; 

			m_data = calloc(1, m_size);
		};

		~FSFileData() 
		{
			free(m_data);
			m_data = nullptr;
		};

		void* Get() { return m_data; };

		const size_t& Size() { return m_size; };

		// Move constructors
		FSFileData(FSFileData&& other) 
		{ 
			m_data = std::move(other.m_data); 
			other.m_data = nullptr;
		};

		FSFileData& operator=(FSFileData&& other)
		{
			m_data = std::move(other.m_data);
			other.m_data = nullptr;
			return *this;
		};

		// Remove copy constructors
		FSFileData(FSFileData&) = delete;
		FSFileData(const FSFileData&) = delete;
		FSFileData& operator=(FSFileData&) = delete;
		FSFileData& operator=(const FSFileData&) = delete;

	private:
		void* m_data;
		size_t m_size;
	};

	struct AURION_API FSInfo
	{
		uint64_t size = 0;
		uint64_t creation_time = 0;
		uint64_t last_modified_time = 0;
		uint64_t last_accessed_time = 0;
		const char* extension = nullptr;
	};
	
	typedef enum AURION_API FSAccess
	{
		FS_ACCESS_READ_ONLY = 0x00,
		FS_ACCESS_WRITE_ONLY = 0x01,
	} FSAccess;

	typedef enum AURION_API FSAttributes
	{
		FS_ATTRIBUTE_NORMAL = 0x00,		// Normal File, no attributes set
		FS_ATTRIBUTE_READ_ONLY = 0x01,	// Read-Only file
		FS_ATTRIBUTE_HIDDEN = 0x02,		// File is hidden (Windows only)
		FS_ATTRIBUTE_DIRECTORY = 0x04,	// Directory tag
	} FSAttributes;
}