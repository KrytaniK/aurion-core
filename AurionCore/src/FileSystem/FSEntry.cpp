module;

#include "AurionLog.h"
#include <cstring>
#include <cstdlib>

module Aurion.FileSystem;
import Aurion.Types;

namespace Aurion
{
    FSEntry::FSEntry(const char* path)
        : m_path(nullptr)
    {
        if (!path || strlen(path) == 0)
        {
            AURION_ERROR("Failed to create FSEntry: Invalid path");
            return;
        }

        m_path = static_cast<char*>(calloc(strlen(path) + 1, sizeof(char)));
        strcpy(m_path, path);
    }

    FSEntry::~FSEntry()
    {
        if (m_path)
            free(m_path);
    }

    FSEntry::FSEntry(FSEntry&& other) noexcept
        : m_path(other.m_path)
    {
        other.m_path = nullptr;
    }

    FSEntry& FSEntry::operator=(FSEntry&& other) noexcept
    {
        if (this == &other) return *this;

        if (m_path) free(m_path);
        m_path = other.m_path;
        other.m_path = nullptr;
        return *this;
    }

    const char* FSEntry::GetPath()
    {
        return m_path;
    }
}
