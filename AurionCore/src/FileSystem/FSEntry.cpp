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
        if (!path || path == nullptr) return;

        if (strlen(path) == 0)
        {
            AURION_ERROR("Failed to create FSEntry: Invalid path");
            return;
        }

        const u64 str_len = strlen(path) + 1;
        m_path = static_cast<char*>(calloc(str_len, sizeof(char)));
        strncpy(m_path, path, str_len);
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
