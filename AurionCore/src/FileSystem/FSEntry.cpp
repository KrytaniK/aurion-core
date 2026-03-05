module;

#include "AurionLog.h"
#include "string.h"
#include "stdlib.h"

module Aurion.FileSystem;
import Aurion.Types;

namespace Aurion
{
    FSEntry::FSEntry(const char* path)
    {
        const size_t size = strlen(m_path);
        if (!m_path || size == 0)
        {
            AURION_ERROR("Failed to create FSEntry: Invalid path");
            return;
        }

        m_path = static_cast<char*>(calloc(strlen(path) + 1, sizeof(char)));
        for (u64 i = 0; i < size; i++)
            m_path[i] = path[i];

        m_path[size] = '\0';
    }

    const char* FSEntry::GetPath()
    {
        return m_path;
    }
}
