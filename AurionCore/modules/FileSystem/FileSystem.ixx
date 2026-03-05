module;

#include <AurionExport.h>

export module Aurion.FileSystem:FileSystem;

import Aurion.Memory;
import :File;
import :Directory;

export namespace Aurion
{
    class AURION_API FileSystem
    {
    public:
        FileSystem();
        ~FileSystem();

        FSFile* GetFile(const char* path, const FSFlags& flags = FS_FLAGS_NONE);
        FSDirectory* GetDirectory(const char* path, const FSFlags& flags = FS_FLAGS_NONE);

        void RemoveFile(const char* path);
        void RemoveFile(FSFile* file);

        void RemoveDirectory(const char* path);
        void RemoveDirectory(FSDirectory* dir);
    };
}
