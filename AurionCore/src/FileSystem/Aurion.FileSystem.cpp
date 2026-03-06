module;

#include <cstring>
#include <stdint.h>

#include "AurionLog.h"
#include <new>

module Aurion.FileSystem;

namespace Aurion
{
    FileSystem::FileSystem()
        : m_files(16), m_directories(16)
    {

    }

    FileSystem::~FileSystem()
    {
        m_files.Clear();
        m_directories.Clear();
    }

    FSFile* FileSystem::GetFile(const char* path)
    {
        return &m_files.EmplaceBack(path);
    }

    FSDirectory* FileSystem::GetDirectory(const char* path)
    {
        return &m_directories.EmplaceBack(path);
    }

    bool FileSystem::RemoveEntry(const FSEntry* entry)
    {
        // First search through files
        size_t index = SIZE_MAX;
        for (size_t i = 0; i < m_files.Size(); i++)
            if (&m_files[i] == entry)
                index = i;

        if (index != SIZE_MAX)
        {
            m_files.Erase(index);
            return true;
        }

        // Then search through directories
        for (size_t i = 0; i < m_directories.Size(); i++)
            if (&m_directories[i] == entry)
                index = i;

        if (index != SIZE_MAX)
        {
            m_directories.Erase(index);
            return true;
        }

        return false;
    }
}
