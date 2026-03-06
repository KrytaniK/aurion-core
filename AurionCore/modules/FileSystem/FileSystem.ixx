module;

#include <AurionExport.h>

export module Aurion.FileSystem:FS;

import :File;
import :Directory;

import Aurion.Memory;

export namespace Aurion
{
    class AURION_API FileSystem
    {
    public:
        FileSystem();
        ~FileSystem();

        FSFile* GetFile(const char* path);
        FSDirectory* GetDirectory(const char* path);

        bool RemoveEntry(const FSEntry* entry);

    private:
        Vector<FSFile> m_files;
        Vector<FSDirectory> m_directories;
    };
}
